add_test_executable(
    ${PROJECT_NAME}_test
    literal.cpp
    str_enum.cpp
    str_name.cpp
    tlist.cpp
)

target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}_test PRIVATE GTest::gmock)
target_link_libraries(${PROJECT_NAME}_test PRIVATE GTest::gtest)
target_link_libraries(${PROJECT_NAME}_test PRIVATE GTest::gtest_main)