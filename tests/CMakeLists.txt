# One executable per tested component; each returns 0 when all its checks pass.
set(TESTS test_bisection test_newton test_fixed_point test_config)

foreach(name ${TESTS})
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE rootfinding)
    add_test(NAME ${name} COMMAND ${name})
endforeach()

# End-to-end tests: run the solver on every example input file.
file(GLOB EXAMPLE_FILES ${PROJECT_SOURCE_DIR}/examples/*.cfg)
foreach(cfg ${EXAMPLE_FILES})
    get_filename_component(cfgName ${cfg} NAME_WE)
    add_test(NAME example_${cfgName} COMMAND solver ${cfg})
endforeach()
