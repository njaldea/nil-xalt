project(xalt)

set(
    HEADERS
        publish/nil/xalt.hpp
        publish/nil/xalt/enum_name.hpp
        publish/nil/xalt/errors.hpp
        publish/nil/xalt/fsign.hpp
        publish/nil/xalt/literal.hpp
        publish/nil/xalt/make_call.hpp
        publish/nil/xalt/tlist.hpp
        publish/nil/xalt/type_id.hpp
        publish/nil/xalt/type_name.hpp
        publish/nil/xalt/value_name.hpp
        publish/nil/xalt/detail/tparam_name.hpp
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