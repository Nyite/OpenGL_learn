function(load_dependencies)
    set(FETCHCONTENT_BASE_DIR ${CMAKE_BINARY_DIR}/_deps)

    include(cmake/CPM.cmake)
    include(cmake/SystemLink.cmake)

    # Load fmt
    CPMAddPackage(
        NAME fmt
        GITHUB_REPOSITORY fmtlib/fmt
        GIT_TAG 6.1.2
    )

    # Load glfw
    CPMAddPackage(
        NAME glfw
        GITHUB_REPOSITORY glfw/glfw
        GIT_TAG 3.4
        OPTIONS
          "GLFW_BUILD_TESTS OFF"
          "GLFW_BUILD_EXAMPLES OFF"
          "GLFW_BUILD_DOCS OFF"
    )

    # Load GLAD
    CPMAddPackage(
        NAME glad
        GITHUB_REPOSITORY Dav1dde/glad
        GIT_TAG master
    )

    # Load stb_image
    file(DOWNLOAD
        "https://raw.githubusercontent.com/nothings/stb/master/stb_image.h"
        "${FETCHCONTENT_BASE_DIR}/stb/include/stb/stb_image.h"
    )
    file(WRITE "${FETCHCONTENT_BASE_DIR}/stb/src/stb_image.cpp"
        "#define STB_IMAGE_IMPLEMENTATION\n#include <stb/stb_image.h>\n"
    )
    add_library(stb_image STATIC
        "${FETCHCONTENT_BASE_DIR}/stb/src/stb_image.cpp"
    )
    target_include_directories(stb_image PUBLIC
        "${FETCHCONTENT_BASE_DIR}/stb/include"
    )

    # Load GTest
    CPMAddPackage(
        NAME googletest
        GITHUB_REPOSITORY google/googletest
        GIT_TAG release-1.8.1
        VERSION 1.8.1
        OPTIONS
            "INSTALL_GTEST OFF"
            "gtest_force_shared_crt ON"
    )
endfunction()
