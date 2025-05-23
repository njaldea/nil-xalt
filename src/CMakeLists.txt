project(xalt)

set(
    HEADERS
        publish/nil/xalt/cast.hpp
        publish/nil/xalt/checks.hpp
        publish/nil/xalt/errors.hpp
        publish/nil/xalt/fn_make.hpp
        publish/nil/xalt/fn_sign.hpp
        publish/nil/xalt/literal.hpp
        publish/nil/xalt/noisy_type.hpp
        publish/nil/xalt/str_name.hpp
        publish/nil/xalt/str_enum.hpp
        publish/nil/xalt/tlist.hpp
        publish/nil/xalt/typed.hpp
        publish/nil/xalt/transparent_stl.hpp
)


add_library(
    ${PROJECT_NAME}
    INTERFACE
    ${HEADERS}
)

target_include_directories(
    ${PROJECT_NAME}
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/publish>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

nil_install_headers()
nil_install_targets(${PROJECT_NAME})

add_test_subdirectory()