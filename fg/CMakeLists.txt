cmake_minimum_required(VERSION 3.12.2 FATAL_ERROR)

project(fggv VERSION 0.0.0 LANGUAGES CXX)
add_executable(${PROJECT_NAME} "main.cpp" "fg.hpp" "fg.cpp" "graphviz_wrapper.hpp" "mainwindow.cpp" "mainwindow.hpp" "mainwindow.ui")

target_compile_features( ${PROJECT_NAME} PRIVATE cxx_std_20)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF AUTOMOC ON AUTOUIC ON)

find_package(Qt5 COMPONENTS Gui Core Widgets REQUIRED)


find_package(PkgConfig REQUIRED)

pkg_check_modules( graphviz IMPORTED_TARGET REQUIRED libgvc libcgraph libcdt)


target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core Qt5::Widgets Qt5::Gui PkgConfig::graphviz)
