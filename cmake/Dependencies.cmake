function(load_dependencies)
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
        NAME GLAD
        GITHUB_REPOSITORY Dav1dde/glad
        GIT_TAG master
    )

    # Load GTest
    CPMAddPackage(
        NAME gtest
        GITHUB_REPOSITORY google/googletest
        GIT_TAG release-1.8.1
        VERSION 1.8.1
        OPTIONS
            "INSTALL_GTEST OFF"
            "gtest_force_shared_crt ON"
    )
endfunction()
