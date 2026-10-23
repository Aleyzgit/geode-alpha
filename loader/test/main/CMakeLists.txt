cmake_minimum_required(VERSION 3.21)

set(PROJECT_NAME TestMod)

project(${PROJECT_NAME} VERSION 1.0.0)

add_library(${PROJECT_NAME} SHARED main.cpp)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)

set(GEODE_LINK_SOURCE ON)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
target_link_libraries(TestMod TestDependency)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mod.json.in ${CMAKE_CURRENT_SOURCE_DIR}/mod.json)
setup_geode_mod(${PROJECT_NAME} DONT_INSTALL)

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # Apple Clang crashes while loading the universal SDK PCH for this JSON
    # translation unit. Compile the dependency from its own complete headers.
    set_source_files_properties("${CPM_PACKAGE_json_SOURCE_DIR}/src/value.cpp"
        PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
endif()
