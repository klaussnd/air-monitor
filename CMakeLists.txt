cmake_minimum_required(VERSION 3.5)
project(air-monitor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/home-control/cmake)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()
find_package(mosquitto REQUIRED)

add_subdirectory(home-control/libs/mqttbase)

set(PROJECT_SOURCES
    src/main.cpp
    src/traywidget.cpp
    src/traywidget.h
    src/mqttdatacollector.h
    src/mqttdatacollector.cpp
    src/colormap.h
    src/colormap.cpp
)

qt_add_executable(air-monitor
    ${PROJECT_SOURCES}
)
target_link_libraries(air-monitor
    PRIVATE
        mqttbase
        Qt6::Widgets
)
set_target_properties(air-monitor PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
install(TARGETS air-monitor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
