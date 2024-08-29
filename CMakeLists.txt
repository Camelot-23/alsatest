cmake_minimum_required(VERSION 3.14)

project(alsatest LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(PkgConfig REQUIRED)
# 获取当前系统环境变量中的 PKG_CONFIG_PATH
if(DEFINED ENV{PKG_CONFIG_PATH})
    set(OLD_PKG_CONFIG_PATH $ENV{PKG_CONFIG_PATH})
else()
    set(OLD_PKG_CONFIG_PATH "")
endif()

# 设置新的路径，并附加到旧的路径后面
set(NEW_PKG_CONFIG_PATH "/home/elot/code/Qt/alsatest/3rd/alsa-install/lib/pkgconfig")
set(ENV{PKG_CONFIG_PATH} "${NEW_PKG_CONFIG_PATH}:${OLD_PKG_CONFIG_PATH}")

message("path== $ENV{PKG_CONFIG_PATH}")

pkg_check_modules(ALSA REQUIRED alsa)
message("ALSA INCLUDE PATH = ${ALSA_INCLUDE_DIRS}")
message("ALSA LIB PATH     = ${ALSA_LIBRARY_DIRS}")
message("ALSA LIBS         = ${ALSA_LIBRARIES}")
message("ALSA CFLAGS       = ${ALSA_CFLAGS}")
message("ALSA LDFLAGS      = ${ALSA_LDFLAGS}")
message("ALSA VERSION      = ${ALSA_VERSION}")

add_executable(alsatest
  main.cpp
)
# target_link_libraries(alsatest Qt${QT_VERSION_MAJOR}::Core)

target_include_directories(alsatest PUBLIC ${ALSA_INCLUDE_DIRS})
target_link_directories(alsatest PUBLIC ${ALSA_LIBRARY_DIRS})
target_link_libraries(alsatest PUBLIC ${ALSA_LIBRARIES})

include(GNUInstallDirs)
install(TARGETS alsatest
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
