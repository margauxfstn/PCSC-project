add_executable(test_bisection test_bisection.cpp)
target_link_libraries(test_bisection PRIVATE rootfinding)
add_test(NAME bisection COMMAND test_bisection)