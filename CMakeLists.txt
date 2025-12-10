cmake_minimum_required(VERSION 3.16)

project(yerkontrol VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. DUZELTME: Network modülünü burada çağırdık (Qt6 bulamazsa Qt5 dener)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(yerkontrol
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(yerkontrol SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(yerkontrol
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# 2. DUZELTME: Network kütüphanesini projene linkledik (Bağladık)
target_link_libraries(yerkontrol PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
)

# 3. DUZELTME: MAVLink klasörünü derleyiciye tanıttık
# (mavlink klasörünün proje dosyalarının yanında olduğunu varsayıyorum)
target_include_directories(yerkontrol PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/mavlink)

# Mac ve iOS ayarları (Standart kısım)
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.yerkontrol)
endif()
set_target_properties(yerkontrol PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS yerkontrol
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(yerkontrol)
endif()
