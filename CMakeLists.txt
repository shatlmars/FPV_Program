cmake_minimum_required(VERSION 3.16)

project(FPV_GR VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_QML_GENERATE_QMLLS_INI ON)
set(GST_ROOT "C:/gstreamer/1.0/msvc_x86_64")

# если CMake не видит vcpkg сам
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
elseif(EXISTS "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

find_package(Qt6 6.8 REQUIRED COMPONENTS Quick Multimedia)
find_package(SDL2 CONFIG REQUIRED)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appFPV_GR
    main.cpp
    # gamepad/gamepadmanager.cpp
)

qt_add_qml_module(appFPV_GR
    URI FPV_GR
    VERSION 1.0
    QML_FILES
    qml/main.qml
    SOURCES
        gamepad/gamepadmanager.h
        gamepad/gamepadmanager.cpp
        network/network.h
        network/network.cpp
        videoplayer/videoplayerManager.cpp
        videoplayer/videoplayerManager.h
)


target_include_directories(appFPV_GR PRIVATE
    "${GST_ROOT}/include/gstreamer-1.0"
    "${GST_ROOT}/include/glib-2.0"
    "${GST_ROOT}/lib/glib-2.0/include"
)

target_link_directories(appFPV_GR PRIVATE "${GST_ROOT}/lib")

target_link_libraries(appFPV_GR
    PRIVATE
        Qt6::Quick
        Qt6::Multimedia
        SDL2::SDL2
        gstreamer-1.0
        gstapp-1.0
        gstvideo-1.0
        gstbase-1.0
        gobject-2.0
        glib-2.0
        intl
)



# DLL рядом с exe
add_custom_command(TARGET appFPV_GR POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2::SDL2>
        $<TARGET_FILE_DIR:appFPV_GR>
)

set_target_properties(appFPV_GR PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS appFPV_GR
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)