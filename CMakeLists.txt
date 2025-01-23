cmake_minimum_required(VERSION 3.16)

project(TegeViewer VERSION 1.00 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6Multimedia)
find_package(Qt6 REQUIRED COMPONENTS MultimediaWidgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TegeViewer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        myimageview.h myimageview.cpp
        myimagescene.h myimagescene.cpp
        define.h

        graphdialog.h graphdialog.cpp graphdialog.ui
        minmaxdialog.h minmaxdialog.cpp minmaxdialog.ui
        infodock.h infodock.cpp infodock.ui
        uvccamera.h uvccamera.cpp
        utility.h utility.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET TegeViewer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(TegeViewer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(TegeViewer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(TegeViewer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(TegeViewer PRIVATE Qt6::Core)
target_link_libraries(TegeViewer PRIVATE Qt6::Multimedia)
target_link_libraries(TegeViewer PRIVATE Qt6::MultimediaWidgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TegeViewer)
endif()
set_target_properties(TegeViewer PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS TegeViewer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TegeViewer)
endif()
