find_package(gflags REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0 gstreamer-app-1.0 gstreamer-video-1.0)

include_directories(${CMAKE_SOURCE_DIR}/src ${CMAKE_BINARY_DIR}/include)

#transmit
add_executable(rtp-sap-transmit rtp_sap_transmit.cc)
target_link_libraries(rtp-sap-transmit mediax_static ${GFLAGS_LIBRARIES} ${GST_LIBRARIES} )
