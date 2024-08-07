# use fake x86/linux with __CPU_EMSCRIPTEN__ defined
set(VSF_HAL_CHIP_VENDOR     generic)
set(VSF_ARCH_SERIES         generic)

set(VSF_HOST_SYSTEM         linux)
set(VSF_HOST_SYSTEM32       1)
set(VSF_HOST_HAS_LIBUSB     0)
set(VSF_HOST_HAS_LIBBSD     0)
set(VSF_HOST_HAS_WEBUSB     1)

set(VSF_TARGET_DEFINITIONS
    "__LINUX__"
    "__CPU_WEBASSEMBLY__"
    "VSF_ARCH_ENTRY_NO_PENDING"
    "VSF_KERNEL_THREAD_USE_HOST=ENABLED"
)

target_link_options(${CMAKE_PROJECT_NAME} PUBLIC
    -sLLD_REPORT_UNDEFINED
    -sTOTAL_MEMORY=536870912
    -sPTHREAD_POOL_SIZE_STRICT=0
# do not add PROXY_TO_PTHREAD, or emscripten_set_main_loop will not be valid
#    -sPROXY_TO_PTHREAD
    -sFORCE_FILESYSTEM=1
    -sUSE_SDL=2
    -sASYNCIFY
    --bind
)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
