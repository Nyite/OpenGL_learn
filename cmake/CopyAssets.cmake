function(copy_assets project_name)

set(ASSET_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(ASSET_BUILD_DIR "${CMAKE_BINARY_DIR}/assets")

add_custom_target(COPY_ASSETS
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different 
    ${ASSET_SOURCE_DIR}
    ${ASSET_BUILD_DIR}
    COMMENT "Copying assets from ${ASSET_SOURCE_DIR} to ${ASSET_BUILD_DIR}"
)
add_dependencies(${project_name} COPY_ASSETS)

endfunction()
