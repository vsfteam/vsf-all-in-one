/*****************************************************************************
 *   Copyright(C)2009-2022 by VSF Team                                       *
 *                                                                           *
 *  Licensed under the Apache License, Version 2.0 (the "License");          *
 *  you may not use this file except in compliance with the License.         *
 *  You may obtain a copy of the License at                                  *
 *                                                                           *
 *     http://www.apache.org/licenses/LICENSE-2.0                            *
 *                                                                           *
 *  Unless required by applicable law or agreed to in writing, software      *
 *  distributed under the License is distributed on an "AS IS" BASIS,        *
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
 *  See the License for the specific language governing permissions and      *
 *  limitations under the License.                                           *
 *                                                                           *
 ****************************************************************************/

/*============================ INCLUDES ======================================*/

#include "../../vsf_linux_cfg.h"

#if VSF_USE_LINUX == ENABLED && VSF_LINUX_USE_LIBUSB == ENABLED

#define __VSF_EDA_CLASS_INHERIT__
#define __VSF_LINUX_CLASS_INHERIT__
#define __VSF_LINUX_FS_CLASS_INHERIT__

// libusb is another upper layer for usb host, so it need to access some private members
// for vk_usbh_get_pipe and urb private members
#define __VSF_USBH_CLASS_IMPLEMENT

#include <vsf.h>

#if VSF_LINUX_CFG_RELATIVE_PATH == ENABLED
#   include "../../include/unistd.h"
#   include "../../include/libusb/libusb.h"

#   include "../../include/poll.h"
#   include "../../include/pthread.h"
#   include "../../include/sys/time.h"
#else
#   include <unistd.h>
#   include <libusb.h>

#   include <poll.h>
#   include <pthread.h>
#   include <sys/time.h>
#endif

#if VSF_LINUX_CFG_RELATIVE_PATH == ENABLED && VSF_LINUX_USE_SIMPLE_STDLIB == ENABLED
#   include "../../include/simple_libc/stdlib.h"
#else
#   include <stdlib.h>
#endif

#if VSF_LINUX_CFG_RELATIVE_PATH == ENABLED && VSF_LINUX_USE_SIMPLE_STRING == ENABLED
#   include "../../include/simple_libc/string.h"
#else
#   include <string.h>
#endif

/*============================ MACROS ========================================*/

#if VSF_KERNEL_CFG_SUPPORT_EVT_MESSAGE != ENABLED
#   error VSF_KERNEL_CFG_SUPPORT_EVT_MESSAGE must be enabled
#endif

// workaround for pipe MACRO
#if VSF_LINUX_CFG_WRAPPER == ENABLED
#   undef pipe
#endif

#if VSF_USE_USB_HOST != ENABLED || VSF_USBH_USE_LIBUSB != ENABLED
#   error Please enable VSF_USE_USB_HOST and VSF_USBH_USE_LIBUSB
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

typedef struct vsf_linux_libusb_cb_t {
    vsf_dlist_node_t cbnode;

    libusb_hotplug_callback_handle handle;
    libusb_context *ctx;
    libusb_hotplug_event events;
    libusb_hotplug_flag flags;
    libusb_hotplug_callback_fn cb_fn;

    int vendor_id;
    int product_id;
    int dev_class;

#if VSF_ARCH_USE_THREAD_REG == ENABLED
    vsf_linux_process_t *target_process;
#endif
    void *user_data;
} vsf_linux_libusb_cb_t;

typedef struct vsf_linux_libusb_transfer_t {
    vsf_dlist_node_t transnode;
    vsf_eda_t eda;
    // transfer MUST be the last for variable number of iso_packet_desc
    struct libusb_transfer transfer;
} vsf_linux_libusb_transfer_t;

typedef struct vsf_linux_libusb_pipe_t {
    vk_usbh_urb_t urb;
    vsf_dlist_t translist;
    bool is_submitted;
} vsf_linux_libusb_pipe_t;

typedef struct vsf_linux_libusb_dev_t {
    vsf_dlist_node_t devnode;
    vk_usbh_libusb_dev_t *libusb_dev;
    uint16_t refcnt;
    bool is_in_newlist;
    bool is_to_free;
    union {
        struct {
            vsf_linux_libusb_pipe_t pipe_in[16];
            vsf_linux_libusb_pipe_t pipe_out[16];
        };
        vsf_linux_libusb_pipe_t pipe[32];
    };
} vsf_linux_libusb_dev_t;

typedef struct vsf_linux_libusb_t {
    libusb_hotplug_callback_handle cbhandle;
    vsf_dlist_t cblist;

    int devnum;
    vsf_trig_t dev_trig;
    vsf_dlist_t devlist;
    vsf_dlist_t devlist_new;
    vsf_dlist_t devlist_del;
    vsf_linux_thread_t *core_thread;

    bool is_to_exit;
    vsf_trig_t trans_trig;
    vsf_dlist_t translist_done;
    vsf_linux_thread_t *user_thread;

    int fd;
    struct libusb_pollfd pollfd[1];
} vsf_linux_libusb_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

static int __vsf_linux_libusb_fd_close(vsf_linux_fd_t *sfd);

/*============================ LOCAL VARIABLES ===============================*/

static vsf_linux_libusb_t __vsf_libusb = {
    .fd                 = -1,
    .pollfd[0].fd       = -1,
};

static const vsf_linux_fd_op_t __vsf_linux_libusb_fdop = {
    .fn_close           = __vsf_linux_libusb_fd_close,
};

/*============================ IMPLEMENTATION ================================*/

static vsf_linux_libusb_pipe_t * __vsf_libusb_get_pipe(vsf_linux_libusb_dev_t *ldev, unsigned char endpoint)
{
    unsigned char epaddr = endpoint & 0x7F;
    if (!epaddr) {
        return &ldev->pipe[0];
    }
    return ((endpoint & USB_DIR_MASK) == USB_DIR_IN) ? &ldev->pipe_in[epaddr] : &ldev->pipe_out[epaddr];
}

static void __vsf_linux_libusb_on_event(void *param, vk_usbh_libusb_dev_t *dev, vk_usbh_libusb_evt_t evt)
{
    vsf_linux_libusb_dev_t *ldev;
    vsf_protect_t orig;

    if (VSF_USBH_LIBUSB_EVT_ON_ARRIVED == evt) {
        vk_usbh_libusb_open(dev);
        ldev = vsf_heap_malloc(sizeof(vsf_linux_libusb_dev_t));
        if (ldev != NULL) {
            memset(ldev, 0, sizeof(*ldev));
            ldev->libusb_dev = dev;
            ldev->is_in_newlist = true;
            ldev->pipe[0].urb.pipe = vk_usbh_get_pipe(dev->dev, 0x80, USB_ENDPOINT_XFER_CONTROL, dev->ep0size);
            dev->user_data = ldev;
            orig = vsf_protect_sched();
                vsf_dlist_add_to_tail(vsf_linux_libusb_dev_t, devnode, &__vsf_libusb.devlist_new, ldev);
            vsf_unprotect_sched(orig);
        }
    } else if (VSF_USBH_LIBUSB_EVT_ON_LEFT) {
        ldev = (vsf_linux_libusb_dev_t *)dev->user_data;
        orig = vsf_protect_sched();
        if (ldev->is_in_newlist) {
            vsf_dlist_remove(vsf_linux_libusb_dev_t, devnode, &__vsf_libusb.devlist_new, ldev);
            vsf_unprotect_sched(orig);
            return;
        } else {
            vsf_dlist_remove(vsf_linux_libusb_dev_t, devnode, &__vsf_libusb.devlist, ldev);
            vsf_dlist_add_to_tail(vsf_linux_libusb_dev_t, devnode, &__vsf_libusb.devlist_del, ldev);
            vsf_unprotect_sched(orig);
        }
    } else {
        return;
    }
    vsf_eda_trig_set(&__vsf_libusb.dev_trig);
}

static void __vsf_linux_libusb_process_cb(vsf_linux_libusb_dev_t *ldev, vk_usbh_libusb_evt_t evt)
{
    __vsf_dlist_foreach_unsafe(vsf_linux_libusb_cb_t, cbnode, &__vsf_libusb.cblist) {
        if (    ((_->vendor_id == LIBUSB_HOTPLUG_MATCH_ANY) || (_->vendor_id == ldev->libusb_dev->vid))
            &&  ((_->product_id == LIBUSB_HOTPLUG_MATCH_ANY) || (_->product_id == ldev->libusb_dev->pid))
            &&  ((_->dev_class == LIBUSB_HOTPLUG_MATCH_ANY) || (_->dev_class == ldev->libusb_dev->c))
            &&  (_->cb_fn != NULL)) {

#if VSF_ARCH_USE_THREAD_REG == ENABLED
            uintptr_t reg_orig = vsf_arch_set_thread_reg(_->target_process->reg);
#endif
            switch (evt) {
            case VSF_USBH_LIBUSB_EVT_ON_ARRIVED:
                if (_->events & LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED) {
                    _->cb_fn(_->ctx, (libusb_device *)ldev, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED, _->user_data);
                }
                break;
            case VSF_USBH_LIBUSB_EVT_ON_LEFT:
                if (_->events & LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT) {
                    _->cb_fn(_->ctx, (libusb_device *)ldev, LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT, _->user_data);
                }
                break;
            }
#if VSF_ARCH_USE_THREAD_REG == ENABLED
            vsf_arch_set_thread_reg(reg_orig);
#endif
        }
    }
}

static void __vsf_linux_libusb_fd_trigger(void)
{
    for (int i = 0; i < dimof(__vsf_libusb.pollfd); i++) {
        if (__vsf_libusb.pollfd[i].fd >= 0) {
            vsf_linux_fd_set_events(__vsf_libusb.pollfd[i].sfd->priv, POLLIN, vsf_protect_sched());
        }
    }
}

#if __IS_COMPILER_IAR__
//! statement is unreachable
#   pragma diag_suppress=pe111
#endif

static void * __vsf_libusb_libusb_core_thread(void *param)
{
    vsf_linux_libusb_dev_t *ldev;
    vsf_protect_t orig;
    vk_usbh_libusb_set_evthandler(NULL, __vsf_linux_libusb_on_event);
    while (1) {
        vsf_thread_trig_pend(&__vsf_libusb.dev_trig, -1);

        while (!vsf_dlist_is_empty(&__vsf_libusb.devlist_del)) {
            orig = vsf_protect_sched();
                vsf_dlist_remove_head(vsf_linux_libusb_dev_t, devnode, &__vsf_libusb.devlist_del, ldev);
            vsf_unprotect_sched(orig);

            __vsf_libusb.devnum--;
            __vsf_linux_libusb_process_cb(ldev, VSF_USBH_LIBUSB_EVT_ON_LEFT);

            ldev->is_to_free = true;
            if (!ldev->refcnt) {
                vk_usbh_free_urb(ldev->libusb_dev->usbh, &ldev->libusb_dev->urb);
                vk_usbh_libusb_close(ldev->libusb_dev);
                vsf_heap_free(ldev);
            }
        }
        while (!vsf_dlist_is_empty(&__vsf_libusb.devlist_new)) {
            orig = vsf_protect_sched();
                vsf_dlist_remove_head(vsf_linux_libusb_dev_t, devnode, &__vsf_libusb.devlist_new, ldev);
                vsf_dlist_add_to_tail(vsf_linux_libusb_dev_t, devnode, &__vsf_libusb.devlist, ldev);
                ldev->is_in_newlist = false;
            vsf_unprotect_sched(orig);

            __vsf_libusb.devnum++;
            __vsf_linux_libusb_process_cb(ldev, VSF_USBH_LIBUSB_EVT_ON_ARRIVED);
        }
        __vsf_linux_libusb_fd_trigger();
    }
    return NULL;
}

static void * __vsf_libusb_libusb_user_thread(void *param)
{
    vsf_linux_libusb_transfer_t *ltransfer;
    vsf_protect_t orig;

    while (1) {
        vsf_thread_trig_pend(&__vsf_libusb.trans_trig, -1);

        while (!vsf_dlist_is_empty(&__vsf_libusb.translist_done)) {
            orig = vsf_protect_sched();
                vsf_dlist_remove_head(vsf_linux_libusb_transfer_t, transnode, &__vsf_libusb.translist_done, ltransfer);
            vsf_unprotect_sched(orig);

            if (ltransfer->transfer.callback != NULL) {
                ltransfer->transfer.callback(&ltransfer->transfer);
            }

            orig = vsf_protect_sched();
                ((vsf_linux_libusb_dev_t *)(ltransfer->transfer.dev_handle))->refcnt--;
            vsf_unprotect_sched(orig);

            __vsf_linux_libusb_fd_trigger();
        }

        if (__vsf_libusb.is_to_exit) {
            pthread_exit(0);
        }
    }
    return NULL;
}

void vsf_linux_libusb_startup(void)
{
    if (!__vsf_libusb.cbhandle) {
        __vsf_libusb.cbhandle = 1;
        vsf_eda_trig_init(&__vsf_libusb.dev_trig, false, true);

        pthread_t pthread;
        pthread_create(&pthread, NULL, __vsf_libusb_libusb_core_thread, NULL);
#if VSF_KERNEL_CFG_TRACE == ENABLED
        vsf_linux_thread_t *thread = vsf_linux_get_thread(-1, pthread);
        vsf_kernel_trace_eda_info(&thread->use_as__vsf_eda_t, "libusb_core_task",
                thread->stack, thread->stack_size);
#endif
        __vsf_libusb.core_thread = vsf_linux_get_thread(-1, pthread);
    }
}

static int __vsf_linux_libusb_fd_close(vsf_linux_fd_t *sfd)
{
    __vsf_libusb.fd = -1;
    // TODO: call fini
    return 0;
}

const char * libusb_strerror(int code)
{
    return "unknown";
}

const struct libusb_version * libusb_get_version(void)
{
    static const struct libusb_version __version = {
        .major      = 1,
        .minor      = 0,
    };
    return &__version;
}

int libusb_init(libusb_context **context)
{
    if (__vsf_libusb.fd >= 0) {
        // already initialized
        return LIBUSB_SUCCESS;
    }

    vsf_linux_fd_t *sfd;
    __vsf_libusb.fd = vsf_linux_fd_create(&sfd, &__vsf_linux_libusb_fdop);
    if (__vsf_libusb.fd < 0) {
        return LIBUSB_ERROR_NO_MEM;
    }
    __vsf_libusb.pollfd[0].fd = __vsf_libusb.fd;
    __vsf_libusb.pollfd[0].sfd = sfd;
    __vsf_libusb.pollfd[0].events = POLLIN;

    __vsf_libusb.is_to_exit = false;
    vsf_eda_trig_init(&__vsf_libusb.trans_trig, false, true);
    pthread_t pthread;
    pthread_create(&pthread, NULL, __vsf_libusb_libusb_user_thread, NULL);
#if VSF_KERNEL_CFG_TRACE == ENABLED
        vsf_linux_thread_t *thread = vsf_linux_get_thread(-1, pthread);
        vsf_kernel_trace_eda_info(&thread->use_as__vsf_eda_t, "libusb_user_task",
                thread->stack, thread->stack_size);
#endif
    __vsf_libusb.user_thread = vsf_linux_get_thread(-1, pthread);
    return LIBUSB_SUCCESS;
}

void libusb_exit(libusb_context *ctx)
{
    if (__vsf_libusb.fd >= 0) {
        __vsf_libusb.is_to_exit = true;
        vsf_eda_trig_set(&__vsf_libusb.trans_trig);
        pthread_join(__vsf_libusb.user_thread->tid, NULL);
        __vsf_libusb.user_thread = NULL;

        int fd = __vsf_libusb.fd;
        __vsf_libusb.fd = -1;
        close(fd);
    }
}

void libusb_set_debug(libusb_context *ctx, int level)
{
    // do nothing
}

const char * libusb_error_name(int errcode)
{
    return "unknown";
}

#if __IS_COMPILER_IAR__
//! statement is unreachable
#   pragma diag_suppress=pe111
#endif

int libusb_get_next_timeout(libusb_context *ctx, struct timeval *tv)
{
    return LIBUSB_ERROR_NOT_SUPPORTED;
}

#if __IS_COMPILER_IAR__
//! statement is unreachable
#   pragma diag_warning=pe111
#endif

int libusb_has_capability(uint32_t capability)
{
    switch (capability) {
    case LIBUSB_CAP_HAS_CAPABILITY:
    case LIBUSB_CAP_HAS_HOTPLUG:
        return 1;
    default:
        return 0;
    }
}

int libusb_hotplug_register_callback(libusb_context *ctx,
        libusb_hotplug_event events,
        libusb_hotplug_flag flags,
        int vendor_id,
        int product_id,
        int dev_class,
        libusb_hotplug_callback_fn cb_fn,
        void *user_data,
        libusb_hotplug_callback_handle *callback_handle)
{
    vsf_linux_libusb_cb_t *cb = malloc(sizeof(vsf_linux_libusb_cb_t));
    if (NULL == cb) { return LIBUSB_ERROR_NO_MEM; }

    cb->handle = __vsf_libusb.cbhandle++;
    if (callback_handle != NULL) {
        *callback_handle = cb->handle;
    }
    cb->ctx = ctx;
    cb->events = events;
    cb->flags = flags;
    cb->vendor_id = vendor_id;
    cb->product_id = product_id;
    cb->dev_class = dev_class;
    cb->cb_fn = cb_fn;
    cb->user_data = user_data;
#if VSF_ARCH_USE_THREAD_REG == ENABLED
    cb->target_process = vsf_linux_get_cur_process();
#endif

    vsf_protect_t orig = vsf_protect_sched();
        vsf_dlist_add_to_head(vsf_linux_libusb_cb_t, cbnode, &__vsf_libusb.cblist, cb);
    vsf_unprotect_sched(orig);
    return LIBUSB_SUCCESS;
}

void libusb_hotplug_deregister_callback(libusb_context *ctx,
        libusb_hotplug_callback_handle callback_handle)
{
    vsf_linux_libusb_cb_t *cb = NULL;
    vsf_protect_t orig = vsf_protect_sched();
        __vsf_dlist_foreach_unsafe(vsf_linux_libusb_cb_t, cbnode, &__vsf_libusb.cblist) {
            if (_->handle == callback_handle) {
                vsf_dlist_remove(vsf_linux_libusb_cb_t, cbnode, &__vsf_libusb.cblist, _);
                cb = _;
                break;
            }
        }
    vsf_unprotect_sched(orig);

    if (cb != NULL) {
        free(cb);
    }
}

ssize_t libusb_get_device_list(libusb_context *ctx, libusb_device *** list)
{
    ssize_t devnum = __vsf_libusb.devnum;
    if (list != NULL) {
        *list = NULL;
    }
    if ((devnum > 0) && (list != NULL)) {
        ssize_t i;
        libusb_device **devlist = (libusb_device **)malloc((devnum + 1) * sizeof(libusb_device *));
        if (!devlist) { return 0; }

        vsf_linux_libusb_dev_t *ldev;
        vsf_dlist_peek_head(vsf_linux_libusb_dev_t, devnode, &__vsf_libusb.devlist, ldev);
        for (i = 0; i < devnum; i++) {
            devlist[i] = (libusb_device *)ldev;
            vsf_dlist_peek_next(vsf_linux_libusb_dev_t, devnode, ldev, ldev);
        }
        devlist[i] = NULL;
        *list = devlist;
    }
    return devnum;
}

void libusb_free_device_list(libusb_device **list, int unref_devices)
{
    if (list != NULL) {
        free(list);
    }
}

int libusb_open(libusb_device *dev, libusb_device_handle **dev_handle)
{
    if (dev_handle != NULL) {
        *dev_handle = (libusb_device_handle *)dev;
    }
    return LIBUSB_SUCCESS;
}

libusb_device * libusb_get_device(libusb_device_handle *dev_handle)
{
    return (libusb_device *)dev_handle;
}

int libusb_reset_device(libusb_device_handle *dev_handle)
{
    int result;
    vsf_linux_libusb_dev_t *ldev = (vsf_linux_libusb_dev_t *)libusb_get_device(dev_handle);
    vk_usbh_t *usbh = ldev->libusb_dev->usbh;
    vk_usbh_dev_t *dev = ldev->libusb_dev->dev;

    int config_val;
    if (LIBUSB_SUCCESS != libusb_get_configuration(dev_handle, &config_val)) {
        config_val = -1;
    }

    vk_usbh_reset_dev(usbh, dev);
    while (vk_usbh_is_dev_resetting(usbh, dev)) {
        usleep(20 * 1000);
    }

    // restore original address
    vsf_linux_libusb_pipe_t *pipe = __vsf_libusb_get_pipe(ldev, 0);
    VSF_LINUX_ASSERT(!vk_usbh_urb_is_alloced(&pipe->urb));
    pipe->urb.pipe.address = 0;
    result = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT,
                                USB_REQ_SET_ADDRESS, dev->devnum, 0, NULL, 0, 1000);
    if (LIBUSB_SUCCESS != result) {
        goto failed;
    }
    pipe->urb.pipe.address = dev->devnum;

    // restore original configuration
    if (config_val >= 0) {
        result = libusb_set_configuration(dev_handle, config_val);
    }

failed:
    __vsf_linux_libusb_fd_trigger();
    return result;
}

libusb_device_handle * libusb_open_device_with_vid_pid(libusb_context *ctx,
        uint16_t vendor_id, uint16_t product_id)
{
    struct libusb_device **devs, *dev_match = NULL;
    vsf_linux_libusb_dev_t *dev;
    int i = 0;

    if (libusb_get_device_list(ctx, &devs) <= 0) {
        return NULL;
    }

    while ((dev = (vsf_linux_libusb_dev_t *)devs[i++]) != NULL) {
        if (    (dev->libusb_dev->vid == vendor_id)
            &&  (dev->libusb_dev->pid == product_id)) {
            dev_match = (struct libusb_device *)dev;
            break;
        }
    }

    libusb_free_device_list(devs, 1);
    return (libusb_device_handle *)dev_match;
}

void libusb_close(libusb_device_handle *dev_handle)
{

}

int libusb_attach_kernel_driver(libusb_device_handle *dev_handle, int interface_number)
{
    return LIBUSB_ERROR_NOT_SUPPORTED;
}

int libusb_detach_kernel_driver(libusb_device_handle *dev_handle, int interface_number)
{
    return LIBUSB_ERROR_NOT_SUPPORTED;
}

int libusb_kernel_driver_active(libusb_device_handle *dev_handle, int interface_number)
{
    return LIBUSB_ERROR_NOT_SUPPORTED;
}

uint8_t libusb_get_bus_number(libusb_device *dev)
{
    return 0;
}

uint8_t libusb_get_port_number(libusb_device *dev)
{
    vsf_linux_libusb_dev_t *ldev = (vsf_linux_libusb_dev_t *)dev;
    return ldev->libusb_dev->dev->index;
}

libusb_device * libusb_get_parent(libusb_device *dev)
{
    // hubs are not handled in libusb
    return NULL;
}

libusb_device * libusb_ref_device(libusb_device *dev)
{
    return dev;
}

void libusb_unref_device(libusb_device *dev)
{
}

uint8_t libusb_get_device_address(libusb_device *dev)
{
    vsf_linux_libusb_dev_t *ldev = (vsf_linux_libusb_dev_t *)dev;
    return ldev->libusb_dev->address;
}

int libusb_get_max_packet_size(libusb_device *dev, unsigned char endpoint)
{
    vsf_linux_libusb_dev_t *ldev = (vsf_linux_libusb_dev_t *)dev;
    vsf_linux_libusb_pipe_t *pipe = __vsf_libusb_get_pipe(ldev, endpoint);
    return pipe->urb.pipe.size;
}

int libusb_get_device_speed(libusb_device *dev)
{
    vsf_linux_libusb_dev_t *ldev = (vsf_linux_libusb_dev_t *)dev;
    return ldev->libusb_dev->dev->speed;
}

int libusb_clear_halt(libusb_device_handle *dev_handle, unsigned char endpoint)
{
    return libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_ENDPOINT | LIBUSB_ENDPOINT_OUT,
        USB_REQ_CLEAR_FEATURE, USB_ENDPOINT_HALT, endpoint, NULL, 0, 1000);
}

static void __libusb_transfer_cb(struct libusb_transfer *ltransfer)
{
    vsf_eda_post_evt((vsf_eda_t *)ltransfer->user_data, VSF_EVT_USER);
}

int libusb_control_transfer(libusb_device_handle *dev_handle,
    uint8_t bRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex,
    unsigned char *data, uint16_t wLength, unsigned int timeout)
{
    struct libusb_transfer *ltransfer = libusb_alloc_transfer(0);
    if (NULL == ltransfer) {
        return LIBUSB_ERROR_NO_MEM;
    }

    uint8_t *buffer = malloc(LIBUSB_CONTROL_SETUP_SIZE + wLength);
    if (NULL == buffer) {
        libusb_free_transfer(ltransfer);
        return LIBUSB_ERROR_NO_MEM;
    }

    libusb_fill_control_setup(buffer, bRequestType, bRequest, wValue, wIndex, wLength);
    if ((bRequestType & LIBUSB_ENDPOINT_DIR_MASK) == LIBUSB_ENDPOINT_OUT) {
        memcpy(buffer + LIBUSB_CONTROL_SETUP_SIZE, data, wLength);
    }

    vsf_eda_t *eda = vsf_eda_get_cur();
    VSF_LINUX_ASSERT(eda != NULL);
    libusb_fill_control_transfer(ltransfer, dev_handle, buffer, __libusb_transfer_cb, eda, timeout);
    ltransfer->flags = LIBUSB_TRANSFER_FREE_BUFFER;
    int result = libusb_submit_transfer(ltransfer);
    if (result < 0) {
        libusb_free_transfer(ltransfer);
        return result;
    }

    vsf_thread_wfe(VSF_EVT_USER);
    if ((bRequestType & LIBUSB_ENDPOINT_DIR_MASK) == LIBUSB_ENDPOINT_IN) {
        memcpy(data, libusb_control_transfer_get_data(ltransfer), ltransfer->actual_length);
    }

    result = ltransfer->status == LIBUSB_TRANSFER_COMPLETED ? ltransfer->actual_length : LIBUSB_ERROR_IO;
    libusb_free_transfer(ltransfer);
    return result;
}

static int __libusb_bulk_interrupt_transfer(libusb_device_handle *dev_handle,
    unsigned char endpoint, unsigned char *data, int length,
    int *actual_length, unsigned int timeout, unsigned char type)
{
    struct libusb_transfer *ltransfer = libusb_alloc_transfer(0);
    if (NULL == ltransfer) {
        return LIBUSB_ERROR_NO_MEM;
    }

    vsf_eda_t *eda = vsf_eda_get_cur();
    VSF_LINUX_ASSERT(eda != NULL);
    libusb_fill_bulk_transfer(ltransfer, dev_handle, endpoint, data, length, __libusb_transfer_cb, eda, timeout);
    ltransfer->type = type;

    int result = libusb_submit_transfer(ltransfer);
    if (result < 0) {
        libusb_free_transfer(ltransfer);
        return result;
    }

    vsf_thread_wfe(VSF_EVT_USER);
    if (actual_length) {
        *actual_length = ltransfer->actual_length;
    }

    result = ltransfer->status == LIBUSB_TRANSFER_COMPLETED ? 0 : LIBUSB_ERROR_IO;
    libusb_free_transfer(ltransfer);
    return result;
}

int libusb_bulk_transfer(libusb_device_handle *dev_handle,
    unsigned char endpoint, unsigned char *data, int length,
    int *actual_length, unsigned int timeout)
{
    return __libusb_bulk_interrupt_transfer(dev_handle, endpoint, data, length, actual_length, timeout, LIBUSB_TRANSFER_TYPE_BULK);
}

int libusb_interrupt_transfer(libusb_device_handle *dev_handle,
    unsigned char endpoint, unsigned char *data, int length,
    int *actual_length, unsigned int timeout)
{
    return __libusb_bulk_interrupt_transfer(dev_handle, endpoint, data, length, actual_length, timeout, LIBUSB_TRANSFER_TYPE_INTERRUPT);
}

int libusb_get_device_descriptor(libusb_device *dev, struct libusb_device_descriptor *desc)
{
    int err = libusb_get_descriptor((libusb_device_handle *)dev, USB_DT_DEVICE, 0,
            (unsigned char *)desc, sizeof(struct libusb_device_descriptor));
    return err < 0 ? err : LIBUSB_SUCCESS;
}

int libusb_get_string_descriptor_ascii(libusb_device_handle *dev_handle,
    uint8_t desc_index, unsigned char *data, int length)
{
    unsigned char buf[255];
    uint_fast16_t langid;
    int err;

    err = libusb_get_string_descriptor(dev_handle, 0, 0, buf, sizeof(buf));
    if (err < 0) { return err; }
    else if (err < 4) { return LIBUSB_ERROR_IO; }

    langid = get_unaligned_le16(&buf[2]);
    err = libusb_get_string_descriptor(dev_handle, desc_index, langid, buf, sizeof(buf));
    if (err < 0) { return err; }
    else if ((err != buf[0]) || (buf[1] != LIBUSB_DT_STRING)) { return LIBUSB_ERROR_IO; }

    int pos = 0;
    for (int i = 2; (i < buf[0]) && (pos < length - 1); i += 2) {
        if ((buf[i] & 0x80) || buf[i + 1]) {
            data[pos++] = '?';
        } else {
            data[pos++] = buf[i];
        }
    }
    data[pos] = '\0';
    return pos;
}

int libusb_set_configuration(libusb_device_handle *dev_handle, int configuration)
{
    struct libusb_config_descriptor *config;
    int err = libusb_control_transfer(dev_handle, USB_DIR_OUT,
            USB_REQ_SET_CONFIGURATION, configuration, 0, NULL, 0, 1000);
    if (err < 0) { return err; }

    err = libusb_get_active_config_descriptor((libusb_device *)dev_handle, &config);
    if (err < 0) { return err; }
    libusb_free_config_descriptor(config);
    return 0;
}

int libusb_get_configuration(libusb_device_handle *dev_handle, int *config)
{
    *config = 0;
    int err = libusb_control_transfer(dev_handle, USB_DIR_IN,
            USB_REQ_GET_CONFIGURATION, 0, 0, (unsigned char *)config, 1, 1000);
    return err < 0 ? err : LIBUSB_SUCCESS;
}

int libusb_set_interface_alt_setting(libusb_device_handle *dev_handle,
        int interface_number, int alternate_setting)
{
    return libusb_control_transfer(dev_handle,
            LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_STANDARD | LIBUSB_RECIPIENT_INTERFACE,
            LIBUSB_REQUEST_SET_INTERFACE, alternate_setting, interface_number, NULL, 0, 0);
}

int libusb_release_interface(libusb_device_handle *dev_handle, int interface_number)
{
    return LIBUSB_SUCCESS;
}

int libusb_claim_interface(libusb_device_handle *dev_handle, int interface_number)
{
    return LIBUSB_SUCCESS;
}

void libusb_free_config_descriptor(struct libusb_config_descriptor *config)
{
    if (config != NULL) {
        for (uint_fast8_t i = 0; i < config->bNumInterfaces; i++) {
            free((void *)config->interface[i].altsetting);
        }
        if (config->desc != NULL) {
            free((void *)config->desc);
        }
        free(config);
    }
}

int libusb_get_descriptor(libusb_device_handle *dev_handle,
    uint8_t desc_type, uint8_t desc_index, unsigned char *data, int length)
{
    return libusb_control_transfer(dev_handle, LIBUSB_ENDPOINT_IN,
        LIBUSB_REQUEST_GET_DESCRIPTOR, (uint16_t) ((desc_type << 8) | desc_index),
        0, data, (uint16_t) length, 1000);
}

int libusb_get_string_descriptor(libusb_device_handle *dev_handle,
    uint8_t desc_index, uint16_t langid, unsigned char *data, int length)
{
    return libusb_control_transfer(dev_handle, LIBUSB_ENDPOINT_IN,
        LIBUSB_REQUEST_GET_DESCRIPTOR, (uint16_t)((LIBUSB_DT_STRING << 8) | desc_index),
        langid, data, (uint16_t) length, 1000);
}

static int __raw_desc_to_config(vsf_linux_libusb_dev_t *ldev, unsigned char *buf, struct libusb_config_descriptor **config)
{
    struct usb_config_desc_t *desc_config = (struct usb_config_desc_t *)buf;
    struct usb_interface_desc_t *desc_ifs;
    struct usb_descriptor_header_t *desc_header =
                (struct usb_descriptor_header_t *)desc_config, *header_tmp;
    struct libusb_config_descriptor *lconfig;

    struct libusb_interface *interface;
    struct libusb_interface_descriptor *interface_desc;
    struct libusb_endpoint_descriptor *endpoint_desc;

    uint_fast16_t size = desc_config->wTotalLength, len, tmpsize;
    uint_fast8_t ifs_no, alt_num, ep_num, reach_endpoint = false;

    enum {
        STAGE_NONE = 0,
        STAGE_ALLOC_ALT,
        STAGE_PROBE_ALT,
    } stage;

    if (desc_header->bDescriptorType != USB_DT_CONFIG) {
        return LIBUSB_ERROR_IO;
    }

    len = desc_config->bNumInterfaces * sizeof(struct libusb_interface);
    lconfig = calloc(1, len + sizeof(*lconfig));
    if (NULL == lconfig) { return LIBUSB_ERROR_NO_MEM; }
    interface = lconfig->interface = (struct libusb_interface *)&lconfig[1];
    lconfig->desc = (const unsigned char *)desc_config;
    lconfig->use_as__usb_config_desc_t = *desc_config;

    size -= desc_header->bLength;
    desc_header = (struct usb_descriptor_header_t *)((uint8_t *)desc_header + desc_header->bLength);

    interface_desc = NULL;
    endpoint_desc = NULL;
    stage = desc_header->bDescriptorType == USB_DT_INTERFACE ? STAGE_ALLOC_ALT : STAGE_NONE;
    ifs_no = 0;
    alt_num = 0;
    ep_num = 0;
    tmpsize = size;
    header_tmp = desc_header;
    while ((size > 0) && (size >= desc_header->bLength)) {
        switch (desc_header->bDescriptorType) {
        case USB_DT_INTERFACE:
            desc_ifs = (struct usb_interface_desc_t *)desc_header;
            if (!interface->altsetting) {
                if (desc_ifs->bInterfaceNumber == ifs_no) {
                    alt_num++;
                } else {
                alloc_alt:
                    interface->num_altsetting = alt_num;
                    interface->altsetting = calloc(1,
                            alt_num * sizeof(struct libusb_interface_descriptor)
                        +   ep_num * sizeof(struct libusb_endpoint_descriptor));
                    if (NULL == interface->altsetting) {
                        libusb_free_config_descriptor(lconfig);
                        return LIBUSB_ERROR_NO_MEM;
                    }

                    stage = STAGE_PROBE_ALT;
                    interface_desc = (struct libusb_interface_descriptor *)&interface->altsetting[-1];
                    endpoint_desc = (struct libusb_endpoint_descriptor *)&interface->altsetting[alt_num];
                    size = tmpsize;
                    desc_header = header_tmp;
                    alt_num = 0;
                    ep_num = 0;
                    reach_endpoint = false;
                    continue;
                }
            } else {
                if (desc_ifs->bInterfaceNumber == ifs_no) {
                    (++interface_desc)->use_as__usb_interface_desc_t = *desc_ifs;
                    if (interface_desc->bNumEndpoints > 0) {
                        interface_desc->endpoint = endpoint_desc;
                    }
                } else {
                probe_alt:
                    interface_desc = NULL;
                    endpoint_desc = NULL;
                    stage = size > 0 ? STAGE_ALLOC_ALT : STAGE_NONE;
                    ifs_no++;
                    if (ifs_no < lconfig->bNumInterfaces) {
                        interface++;
                    }
                    tmpsize = size;
                    header_tmp = desc_header;
                    continue;
                }
            }
            break;
        case USB_DT_ENDPOINT:
            reach_endpoint = true;
            if (endpoint_desc) {
                endpoint_desc->use_as__usb_endpoint_desc_t = *(struct usb_endpoint_desc_t *)desc_header;
                uint8_t epaddr = endpoint_desc->bEndpointAddress & 0x0F;
                if (epaddr != 0) {
                    if ((endpoint_desc->bEndpointAddress & USB_DIR_MASK) == USB_DIR_IN) {
                        ldev->pipe_in[epaddr].urb.pipe =
                            vk_usbh_get_pipe_from_ep_desc(ldev->libusb_dev->dev, (struct usb_endpoint_desc_t *)&endpoint_desc->use_as__usb_endpoint_desc_t);
                    } else {
                        ldev->pipe_out[epaddr].urb.pipe =
                            vk_usbh_get_pipe_from_ep_desc(ldev->libusb_dev->dev, (struct usb_endpoint_desc_t *)&endpoint_desc->use_as__usb_endpoint_desc_t);
                    }
                }
                endpoint_desc++;
            } else {
                ep_num++;
            }
            break;
        default:
            if ((interface_desc != NULL) && (interface_desc >= &interface->altsetting[0])) {
                if (!reach_endpoint) {
                    if (NULL == interface_desc->extra) {
                        interface_desc->extra = (const unsigned char *)desc_header;
                    }
                    interface_desc->extra_length += desc_header->bLength;
                } else if (endpoint_desc != NULL) {
                    if (NULL == endpoint_desc[-1].extra) {
                        endpoint_desc[-1].extra = (const unsigned char *)desc_header;
                    }
                    endpoint_desc[-1].extra_length += desc_header->bLength;
                }
            }
        }

        size -= desc_header->bLength;
        desc_header = (struct usb_descriptor_header_t *)((uint8_t *)desc_header + desc_header->bLength);
        if (!size && stage) {
            if (stage == STAGE_ALLOC_ALT) {
                goto alloc_alt;
            } else /*if (stage == STAGE_PROBE_ALT)*/ {
                desc_ifs = (struct usb_interface_desc_t *)desc_header;
                goto probe_alt;
            }
        }
    }
    if (config != NULL) {
        *config = lconfig;
    }
    return 0;
}

int libusb_get_active_config_descriptor(libusb_device *dev,
        struct libusb_config_descriptor **config)
{
    int err, config_val;

    err = libusb_get_configuration((libusb_device_handle *)dev, &config_val);
    if (err != LIBUSB_SUCCESS) {
        // some devices will fail if configuration is not set, so set to 0 first
        err = libusb_control_transfer((libusb_device_handle *)dev, USB_DIR_OUT,
            USB_REQ_SET_CONFIGURATION, 0, 0, NULL, 0, 1000);
        if (err != LIBUSB_SUCCESS) {
            return err;
        }
        config_val = 1;
    }
    if (0 == config_val) {
        // user want active config descriptor, but SET_CONFIGURATION is not called, use the first configuration
        err = libusb_get_config_descriptor(dev, 0, config);
        if (LIBUSB_SUCCESS == err) {
            libusb_set_configuration((libusb_device_handle *)dev, (*config)->bConfigurationValue);
        }
        return err;
    } else {
        return libusb_get_config_descriptor(dev, config_val - 1, config);
    }
}

int libusb_get_config_descriptor(libusb_device *dev, uint8_t config_index,
        struct libusb_config_descriptor **config)
{
    struct usb_config_desc_t config_desc;
    unsigned char *buf = NULL;
    int err;

    err = libusb_get_descriptor((libusb_device_handle *)dev, USB_DT_CONFIG,
            config_index, (unsigned char *)&config_desc, LIBUSB_DT_CONFIG_SIZE);
    if (err < 0) { return err; }
    if (err < LIBUSB_DT_CONFIG_SIZE) {
        return LIBUSB_ERROR_IO;
    }
    config_desc.wTotalLength = le16_to_cpu(config_desc.wTotalLength);

    buf = malloc(config_desc.wTotalLength);
    if (!buf) { return LIBUSB_ERROR_NO_MEM; }

    err = libusb_get_descriptor((libusb_device_handle *)dev, USB_DT_CONFIG,
            config_index, buf, config_desc.wTotalLength);
    if (err >= 0) {
        vsf_linux_libusb_dev_t *ldev = (vsf_linux_libusb_dev_t *)dev;
        memset(&ldev->pipe_in[1], 0, sizeof(vk_usbh_pipe_t) * (dimof(ldev->pipe_in) - 1));
        memset(&ldev->pipe_out[1], 0, sizeof(vk_usbh_pipe_t) * (dimof(ldev->pipe_out) - 1));

        err = __raw_desc_to_config(ldev, buf, config);
        if (err < 0) {
            free(buf);
        }
    }

    return err;
}

int libusb_get_config_descriptor_by_value(libusb_device *dev, uint8_t value,
        struct libusb_config_descriptor **config)
{
    // TODO: add implementation
    return LIBUSB_ERROR_IO;
}

void libusb_free_ss_endpoint_companion_descriptor(
    struct libusb_ss_endpoint_companion_descriptor *ep_comp)
{
}

int libusb_get_ss_endpoint_companion_descriptor(
    struct libusb_context *ctx,
    const struct libusb_endpoint_descriptor *endpoint,
    struct libusb_ss_endpoint_companion_descriptor **ep_comp)
{
    return -1;
}

struct libusb_transfer * libusb_alloc_transfer(int iso_packets)
{
    uint32_t size = sizeof(vsf_linux_libusb_transfer_t) +
            iso_packets * sizeof(struct libusb_iso_packet_descriptor);
    vsf_linux_libusb_transfer_t *ltransfer = calloc(1, size);
    if (ltransfer != NULL) {
        ltransfer->transfer.num_iso_packets = iso_packets;
        return &ltransfer->transfer;
    }
    return NULL;
}

static bool __vsf_linux_libusb_submit_transfer_next(vsf_linux_libusb_pipe_t *pipe)
{
    vsf_linux_libusb_transfer_t *ltransfer;
    vsf_protect_t orig;

try_next:
    orig = vsf_protect_sched();
    vsf_dlist_remove_head(vsf_linux_libusb_transfer_t, transnode, &pipe->translist, ltransfer);
    if (NULL == ltransfer) {
        pipe->is_submitted = false;
    }
    vsf_unprotect_sched(orig);

    if (ltransfer != NULL) {
        vk_usbh_urb_t *urb = &pipe->urb;
        struct libusb_transfer *transfer = &ltransfer->transfer;
        vsf_linux_libusb_dev_t *ldev = (vsf_linux_libusb_dev_t *)transfer->dev_handle;

        if (ldev->is_to_free) {
            goto failed;
        }
        if (!vk_usbh_urb_is_alloced(urb)) {
            if (VSF_ERR_NONE != vk_usbh_alloc_urb(ldev->libusb_dev->usbh, ldev->libusb_dev->dev, urb)) {
                VSF_LINUX_ASSERT(false);
                goto failed;
            }
        }

        urb->urb_hcd->buffer = transfer->buffer;
        urb->urb_hcd->transfer_length = transfer->length;
        urb->urb_hcd->timeout = transfer->timeout;
        switch (transfer->type) {
        case LIBUSB_TRANSFER_TYPE_CONTROL: {
                struct usb_ctrlrequest_t *request = (struct usb_ctrlrequest_t *)transfer->buffer;
                urb->urb_hcd->setup_packet = *request;
                urb->urb_hcd->buffer = (uint8_t *)urb->urb_hcd->buffer + sizeof(struct usb_ctrlrequest_t);
                urb->urb_hcd->transfer_length -= sizeof(struct usb_ctrlrequest_t);
                urb->urb_hcd->pipe.dir_in1out0 = (request->bRequestType & USB_DIR_IN) > 0;

//                vsf_trace_debug("ep0: %-5d: ", urb->urb_hcd->transfer_length);
//                vsf_trace_buffer(VSF_TRACE_DEBUG, request, 8);
            }
            break;
        case LIBUSB_TRANSFER_TYPE_ISOCHRONOUS:
#if VSF_USBH_CFG_ISO_EN == ENABLED
            urb->urb_hcd->iso_packet.number_of_packets = transfer->num_iso_packets;
            break;
#else
            goto failed;
#endif
//        default:
//            vsf_trace_debug("ep%s%d: %-5d" VSF_TRACE_CFG_LINEEND,
//                            urb->urb_hcd->pipe.dir_in1out0 ? "IN" : "OUT",
//                            urb->urb_hcd->pipe.endpoint, urb->urb_hcd->transfer_length);
        }

        if (VSF_ERR_NONE != vk_usbh_submit_urb_ex(ldev->libusb_dev->usbh, urb, 0, &ltransfer->eda)) {
        failed:
            transfer->actual_length = 0;
            transfer->status = LIBUSB_TRANSFER_ERROR;
            vk_usbh_free_urb(ldev->libusb_dev->usbh, urb);
            orig = vsf_protect_sched();
                vsf_dlist_add_to_tail(vsf_linux_libusb_transfer_t, transnode, &__vsf_libusb.translist_done, ltransfer);
            vsf_unprotect_sched(orig);
            vsf_eda_trig_set(&__vsf_libusb.trans_trig);

            vsf_eda_fini(&ltransfer->eda);

            goto try_next;
        }
        return true;
    }
    return false;
}

static void __vsf_libusb_transfer_evthandler(vsf_eda_t *eda, vsf_evt_t evt)
{
    vsf_linux_libusb_transfer_t *ltransfer = vsf_container_of(eda, vsf_linux_libusb_transfer_t, eda);
    vsf_linux_libusb_dev_t *ldev = (vsf_linux_libusb_dev_t *)ltransfer->transfer.dev_handle;
    vsf_linux_libusb_pipe_t *pipe = __vsf_libusb_get_pipe(ldev, ltransfer->transfer.endpoint);
    vk_usbh_urb_t *urb = &pipe->urb;
    vsf_protect_t orig;

    switch (evt) {
    case VSF_EVT_MESSAGE:
        ltransfer->transfer.actual_length = vk_usbh_urb_get_actual_length(urb);
        ltransfer->transfer.status = vk_usbh_urb_get_status(urb) == URB_OK ?
                LIBUSB_TRANSFER_COMPLETED : LIBUSB_TRANSFER_ERROR;

        orig = vsf_protect_sched();
            vsf_dlist_add_to_tail(vsf_linux_libusb_transfer_t, transnode, &__vsf_libusb.translist_done, ltransfer);
        vsf_unprotect_sched(orig);
        vsf_eda_trig_set(&__vsf_libusb.trans_trig);

        if (!__vsf_linux_libusb_submit_transfer_next(pipe)) {
            vk_usbh_free_urb(ldev->libusb_dev->usbh, urb);
        }
        break;
    }
}

int libusb_submit_transfer(struct libusb_transfer *transfer)
{
    vsf_linux_libusb_transfer_t *ltransfer = vsf_container_of(transfer, vsf_linux_libusb_transfer_t, transfer);
    vsf_linux_libusb_dev_t *ldev = (vsf_linux_libusb_dev_t *)transfer->dev_handle;
    vsf_linux_libusb_pipe_t *pipe = __vsf_libusb_get_pipe(ldev, ltransfer->transfer.endpoint);

    ltransfer->eda.fn.evthandler = __vsf_libusb_transfer_evthandler;
    vsf_eda_init(&ltransfer->eda);

    vsf_protect_t orig = vsf_protect_sched();
    bool is_submitted = pipe->is_submitted;
    pipe->is_submitted = true;
    vsf_dlist_add_to_tail(vsf_linux_libusb_transfer_t, transnode, &pipe->translist, ltransfer);
    ldev->refcnt++;
    vsf_unprotect_sched(orig);

    if (!is_submitted) {
        __vsf_linux_libusb_submit_transfer_next(pipe);
    }
    return LIBUSB_SUCCESS;
}

int libusb_cancel_transfer(struct libusb_transfer *transfer)
{
    vsf_linux_libusb_transfer_t *ltransfer = vsf_container_of(transfer, vsf_linux_libusb_transfer_t, transfer);
    vsf_linux_libusb_dev_t *ldev = (vsf_linux_libusb_dev_t *)transfer->dev_handle;
    vsf_linux_libusb_pipe_t *pipe = __vsf_libusb_get_pipe(ldev, ltransfer->transfer.endpoint);
    vk_usbh_urb_t *urb = &pipe->urb;
    bool is_in_queue;

    vk_usbh_free_urb(ldev->libusb_dev->usbh, urb);
    transfer->actual_length = 0;
    transfer->status = LIBUSB_TRANSFER_CANCELLED;

    vsf_protect_t orig = vsf_protect_sched();
        is_in_queue = vsf_dlist_is_in(vsf_linux_libusb_transfer_t, transnode, &pipe->translist, ltransfer);
        if (is_in_queue) {
            vsf_dlist_remove(vsf_linux_libusb_transfer_t, transnode, &pipe->translist, ltransfer);
        }
        vsf_dlist_add_to_tail(vsf_linux_libusb_transfer_t, transnode, &__vsf_libusb.translist_done, ltransfer);
    vsf_unprotect_sched(orig);
    vsf_eda_trig_set(&__vsf_libusb.trans_trig);

    if (!is_in_queue) {
        __vsf_linux_libusb_submit_transfer_next(pipe);
    }
    return LIBUSB_SUCCESS;
}

void libusb_free_transfer(struct libusb_transfer *transfer)
{
    if (transfer != NULL) {
        vsf_linux_libusb_transfer_t *ltransfer = vsf_container_of(transfer, vsf_linux_libusb_transfer_t, transfer);
        vsf_linux_libusb_dev_t *ldev = (vsf_linux_libusb_dev_t *)transfer->dev_handle;

        if (ltransfer->transfer.flags & LIBUSB_TRANSFER_FREE_BUFFER) {
            free(ltransfer->transfer.buffer);
        }
        free(ltransfer);

        if (ldev->is_to_free) {
            if (!ldev->refcnt) {
                vk_usbh_free_urb(ldev->libusb_dev->usbh, &ldev->libusb_dev->urb);
                vk_usbh_libusb_close(ldev->libusb_dev);
                vsf_heap_free(ldev);
            }
        }
    }
}

int libusb_handle_events_timeout_completed(libusb_context *ctx,
    struct timeval *tv, int *completed)
{
    int timeout_ms = tv->tv_sec * 1000 + tv->tv_usec / 1000;
    poll((struct pollfd *)__vsf_libusb.pollfd, dimof(__vsf_libusb.pollfd), timeout_ms);
    return LIBUSB_SUCCESS;
}

int libusb_handle_events_completed(libusb_context *ctx, int *completed)
{
    struct timeval tv;
    tv.tv_sec = 60;
    tv.tv_usec = 0;
    return libusb_handle_events_timeout_completed(ctx, &tv, completed);
}

int libusb_handle_events_timeout(libusb_context *ctx, struct timeval *tv)
{
    return libusb_handle_events_timeout_completed(ctx, tv, NULL);
}

int libusb_handle_events(libusb_context *ctx)
{
    struct timeval tv;
    tv.tv_sec = 60;
    tv.tv_usec = 0;
    return libusb_handle_events_timeout_completed(ctx, &tv, NULL);
}

const struct libusb_pollfd** libusb_get_pollfds(libusb_context *ctx)
{
    struct libusb_pollfd **pollfd = (struct libusb_pollfd **)
        calloc(dimof(__vsf_libusb.pollfd) + 1, sizeof(struct libusb_pollfd *));

    if (pollfd != NULL) {
        int i;
        for (i = 0; i < dimof(__vsf_libusb.pollfd); i++) {
            pollfd[i] = &__vsf_libusb.pollfd[i];
        }
        pollfd[i] = NULL;
    }
    return (const struct libusb_pollfd **)pollfd;
}

void libusb_free_pollfds(const struct libusb_pollfd **pollfds)
{
    if (pollfds != NULL) {
        free(pollfds);
    }
}

int __libusb_set_option_va(libusb_context *ctx, enum libusb_option option, va_list ap)
{
    return 0;
}

int libusb_set_option(libusb_context *ctx, enum libusb_option option, ...)
{
    int result;
    va_list ap;
    va_start(ap, option);
        result = __libusb_set_option_va(ctx, option, ap);
    va_end(ap);
    return result;
}

#if VSF_LINUX_APPLET_USE_LIBUSB == ENABLED && !defined(__VSF_APPLET__)
__VSF_VPLT_DECORATOR__ vsf_linux_libusb_vplt_t vsf_linux_libusb_vplt = {
    VSF_APPLET_VPLT_INFO(vsf_linux_libusb_vplt_t, 0, 0, true),

    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_strerror),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_get_version),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_init),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_exit),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_set_debug),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_error_name),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_get_device_list),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_free_device_list),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_get_device_descriptor),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_open),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_get_device),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_open_device_with_vid_pid),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_close),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_get_device_address),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_get_bus_number),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_get_port_number),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_get_parent),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_reset_device),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_clear_halt),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_release_interface),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_claim_interface),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_alloc_transfer),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_free_transfer),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_submit_transfer),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_cancel_transfer),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_control_transfer),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_bulk_transfer),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_interrupt_transfer),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_hotplug_register_callback),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_hotplug_deregister_callback),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_get_string_descriptor_ascii),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_get_config_descriptor),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_get_config_descriptor_by_value),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_get_active_config_descriptor),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_free_config_descriptor),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_get_descriptor),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_get_string_descriptor),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_set_interface_alt_setting),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_get_ss_endpoint_companion_descriptor),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_free_ss_endpoint_companion_descriptor),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_get_next_timeout),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_has_capability),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_attach_kernel_driver),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_detach_kernel_driver),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_kernel_driver_active),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_ref_device),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_unref_device),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_get_max_packet_size),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_get_device_speed),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_set_configuration),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_get_configuration),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_handle_events_timeout_completed),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_handle_events_completed),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_handle_events_timeout),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_handle_events),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_get_pollfds),
    VSF_APPLET_VPLT_ENTRY_FUNC(libusb_free_pollfds),
};
#endif

// libusb 0.1 compatibility
#if VSF_LINUX_LIBUSB_CFG_01_COMPATIBLE == ENABLED
int usb_get_driver_np(usb_dev_handle *dev, int interface, char *name, unsigned int namelen)
{
    return 0;
}

int usb_detach_kernel_driver_np(usb_dev_handle *dev, int interface)
{
    return 0;
}

int usb_find_devices(void)
{
    return __vsf_libusb.devnum;
}

struct usb_bus *usb_get_busses(void)
{
    static struct usb_bus __bus = { 0 };
    if (__bus.devices != NULL) {
        free(__bus.devices);
        __bus.devices = NULL;
    }

    int devnum = __vsf_libusb.devnum;
    if (devnum > 0) {
        __bus.devices = malloc(sizeof(struct usb_device) * devnum);
        memset(__bus.devices, 0, sizeof(struct usb_device) * devnum);

        struct usb_device *dev = __bus.devices;
        vsf_linux_libusb_dev_t *ldev;
        vsf_dlist_peek_head(vsf_linux_libusb_dev_t, devnode, &__vsf_libusb.devlist, ldev);
        for (int i = 0; i < devnum; i++) {
            if (i < (devnum - 1)) {
                dev[i].next = &dev[i + 1];
            }
            if (i > 0) {
                dev[i].prev = &dev[i - 1];
            }

            VSF_LINUX_ASSERT(ldev != NULL);
            dev[i].dev = ldev;
            libusb_get_device_descriptor((libusb_device *)ldev, &dev[i].descriptor);

            // TODO: support multi-config device
            struct libusb_config_descriptor *config;
            if (!libusb_get_active_config_descriptor((libusb_device *)ldev, &config)) {
                memcpy(&dev[i].__config, config, sizeof(*config));
                dev[i].config = &dev[i].__config;
                libusb_free_config_descriptor(config);
            }

            vsf_dlist_peek_next(vsf_linux_libusb_dev_t, devnode, ldev, ldev);
        }
    }
    return &__bus;
}

usb_dev_handle *usb_open(struct usb_device *dev)
{
    usb_dev_handle *handle = NULL;
    libusb_open((libusb_device *)dev->dev, &handle);
    return handle;
}

int usb_close(usb_dev_handle *dev)
{
    libusb_close((libusb_device_handle *)dev);
    return 0;
}

int usb_bulk_write(usb_dev_handle *dev, int ep, char *bytes, int size, int timeout)
{
    int actual_length = 0;
    VSF_LINUX_ASSERT(!(ep & 0x80));
    libusb_bulk_transfer((libusb_device_handle *)dev, ep, (unsigned char *)bytes, size, &actual_length, timeout);
    return actual_length;
}

int usb_bulk_read(usb_dev_handle *dev, int ep, char *bytes, int size, int timeout)
{
    int actual_length = 0;
    VSF_LINUX_ASSERT(ep & 0x80);
    libusb_bulk_transfer((libusb_device_handle *)dev, ep, (unsigned char *)bytes, size, &actual_length, timeout);
    return actual_length;
}

int usb_interrupt_write(usb_dev_handle *dev, int ep, char *bytes, int size, int timeout)
{
    int actual_length = 0;
    VSF_LINUX_ASSERT(!(ep & 0x80));
    libusb_interrupt_transfer((libusb_device_handle *)dev, ep, (unsigned char *)bytes, size, &actual_length, timeout);
    return actual_length;
}

int usb_interrupt_read(usb_dev_handle *dev, int ep, char *bytes, int size, int timeout)
{
    int actual_length = 0;
    VSF_LINUX_ASSERT(ep & 0x80);
    libusb_interrupt_transfer((libusb_device_handle *)dev, ep, (unsigned char *)bytes, size, &actual_length, timeout);
    return actual_length;
}
#endif      // VSF_LINUX_LIBUSB_CFG_01_COMPATIBLE

#endif      // VSF_USE_LINUX
