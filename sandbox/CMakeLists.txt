project(sandbox)

set(ENABLE_SANDBOX  OFF CACHE BOOL "[0 | OFF - 1 | ON]: build sandbox?")
if(NOT ENABLE_SANDBOX)
    return()
endif()

add_executable(
    ${PROJECT_NAME}
    main.cpp
)

add_executable(
    ${PROJECT_NAME}_str_name
    str_name.cpp
)

add_executable(
    ${PROJECT_NAME}_str_name_bench
    str_name_bench.cpp
)

add_executable(
    ${PROJECT_NAME}_str_enum_bench
    str_enum_bench.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    xalt
)

target_link_libraries(
    ${PROJECT_NAME}_str_name
    PRIVATE
    xalt
)

target_link_libraries(
    ${PROJECT_NAME}_str_name_bench
    PRIVATE
    xalt
)

target_link_libraries(
    ${PROJECT_NAME}_str_enum_bench
    PRIVATE
    xalt
)