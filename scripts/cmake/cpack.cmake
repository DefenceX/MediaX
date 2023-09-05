# Debian cpack 
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Ross Newman")
set(CPACK_PACKAGE_VERSION_MAJOR ${MEDIAX_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${MEDIAX_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${MEDIAX_PATCH_VERSION})
set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS ON)
set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS_POLICY ">=")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/scripts/debian/postinst;${CMAKE_CURRENT_SOURCE_DIR}/scripts/debian/prerm")
set(CPACK_DEBIAN_PACKAGE_CONTROL_STRICT_PERMISSION TRUE)
set(CPACK_DEBIAN_PACKAGE_SECTION "libs")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://defencex.ai")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "MediaX library for RTP and SAP")
set(CPACK_DEBIAN_PACKAGE_NAME "mediax")

include(CPack)
