
file(STRINGS ${CMAKE_SOURCE_DIR}/version VERSION_CONTENTS)
list(GET VERSION_CONTENTS 0 MEDIAX_MAJOR_VERSION)
list(GET VERSION_CONTENTS 1 MEDIAX_MINOR_VERSION)
list(GET VERSION_CONTENTS 2 MEDIAX_PATCH_VERSION)
set(MEDIAX_VERSION ${MEDIAX_MAJOR_VERSION}.${MEDIAX_MINOR_VERSION}.${MEDIAX_PATCH_VERSION})
execute_process(COMMAND git rev-parse --short HEAD OUTPUT_VARIABLE GIT_HASH OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "Version: ${MEDIAX_VERSION}")

set(VERSION_FILE ${CMAKE_BINARY_DIR}/include/version.h)
file(WRITE ${VERSION_FILE} "// This file it automatically generated, no not edit manually\n")
file(APPEND ${VERSION_FILE} "#include <string>\n")
file(APPEND ${VERSION_FILE} "#include <cstdint>\n")
file(APPEND ${VERSION_FILE} "const uint32_t kMajor = ${MEDIAX_MAJOR_VERSION};\n")
file(APPEND ${VERSION_FILE} "const uint32_t kMinor = ${MEDIAX_MINOR_VERSION};\n")
file(APPEND ${VERSION_FILE} "const uint32_t kPatch = ${MEDIAX_PATCH_VERSION};\n")
file(APPEND ${VERSION_FILE} "const std::string kVersion = \"${MEDIAX_VERSION}\";\n")
file(APPEND ${VERSION_FILE} "const std::string kGitHash = \"${GIT_HASH}\";\n")

