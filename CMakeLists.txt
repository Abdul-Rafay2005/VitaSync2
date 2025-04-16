cmake_minimum_required(VERSION 3.16)

project(VitaSync VERSION 0.1 LANGUAGES CXX)

# Set Qt automation features (note: removed duplicate AUTOMOC)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

# Source files
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    loginwindow.h
    loginwindow.cpp
    chatwindow.cpp
    chatwindow.h
)

# Create executable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(VitaSync
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(VitaSync SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(VitaSync
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Link Qt libraries
target_link_libraries(VitaSync PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
)

# macOS specific settings
if(APPLE)
    if(${QT_VERSION} VERSION_LESS 6.1.0)
        set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.VitaSync)
    endif()

    set_target_properties(VitaSync PROPERTIES
        ${BUNDLE_ID_OPTION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
    )
endif()

# Windows specific settings
if(WIN32)
    set_target_properties(VitaSync PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# Installation
include(GNUInstallDirs)
install(TARGETS VitaSync
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize for Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(VitaSync)
endif()
