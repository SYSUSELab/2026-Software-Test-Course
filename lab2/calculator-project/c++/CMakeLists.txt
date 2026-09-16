cmake_minimum_required(VERSION 3.16)
project(CalculatorProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(calculator
    Calculator.h
    Calculator.cpp
    MainWindow.h
    MainWindow.cpp
    main.cpp
)

target_link_libraries(calculator PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
