
cmake_minimum_required(VERSION 3.25)

project(CaelestiaColours
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

# GUI application
qt_add_executable(caelestia_colours
    app/main.cpp
    app/windows/main_window.hpp
    app/windows/main_window.cpp
    app/widgets/clickable_frame.hpp
)

target_include_directories(caelestia_colours
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/app
)

target_link_libraries(caelestia_colours
    PRIVATE
        Qt6::Widgets
)

target_compile_options(caelestia_colours
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

# Backend tests
add_executable(scheme_editor_test
    tests/scheme_editor_test.cpp
)

target_include_directories(scheme_editor_test
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(scheme_editor_test
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

enable_testing()

add_test(
    NAME scheme_editor_test
    COMMAND scheme_editor_test
)