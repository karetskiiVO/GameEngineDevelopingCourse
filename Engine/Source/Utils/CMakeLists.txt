file(GLOB PRIVATE_FILES CONFIGURE_DEPENDS
    private/*.cpp
    private/*.h
)
file(GLOB PUBLIC_FILES CONFIGURE_DEPENDS
    public/*.h
)

add_library(Utils STATIC)

target_sources(Utils
    PRIVATE
        ${PRIVATE_FILES}
        ${PLATFORM_PRIVATE_FILES}
    PUBLIC
        ${PUBLIC_FILES}
        ${PLATFORM_PUBLIC_FILES}
)

target_include_directories(Utils
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/public
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_precompile_headers(Utils
    PRIVATE
        <unordered_map>
        <vector>
        <string>
        <cassert>
        <format>
        <functional>
)