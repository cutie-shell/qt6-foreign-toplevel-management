cmake_minimum_required(VERSION 3.16)
project(qt6-foreign-toplevel-management VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Core WaylandClient Quick)
qt_standard_project_setup()

include_directories(src)

set(lib_SOURCES
    src/foreign-toplevel-management.cpp
    src/foreign-toplevel-management_p.h
    src/qt6-foreign-toplevel-management.h)

qt6_add_library(qt6-foreign-toplevel-management
    SHARED ${lib_SOURCES}
)

set_target_properties(qt6-foreign-toplevel-management PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(qt6-foreign-toplevel-management PROPERTIES SOVERSION 0)
set_target_properties(qt6-foreign-toplevel-management PROPERTIES PUBLIC_HEADER src/qt6-foreign-toplevel-management.h)

qt6_generate_wayland_protocol_client_sources(
    qt6-foreign-toplevel-management
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/protocols/wlr-foreign-toplevel-management-unstable-v1.xml
)

target_link_libraries(qt6-foreign-toplevel-management PUBLIC
    Qt::Core
    Qt::WaylandClient
    Qt::WaylandClientPrivate
    Qt::Quick
)

configure_file(src/qt6-foreign-toplevel-management.pc.in qt6-foreign-toplevel-management.pc @ONLY)

install(TARGETS qt6-foreign-toplevel-management
    RUNTIME DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    BUNDLE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(FILES ${CMAKE_BINARY_DIR}/qt6-foreign-toplevel-management.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
