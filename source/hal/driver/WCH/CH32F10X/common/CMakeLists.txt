# CMakeLists head

vsf_add_sources(
    __common.c
)

add_subdirectory_existed(io)
add_subdirectory_existed(pm)
add_subdirectory_existed(usart)
add_subdirectory_existed(usb)
add_subdirectory_existed(vendor)
add_subdirectory_existed(spi)
add_subdirectory_existed(iic)
