cmake_minimum_required(VERSION 3.5)

project(wmm VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "/home/xcarol/Qt/6.7.0/gcc_64/")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

find_package(Qt6 REQUIRED COMPONENTS Sql)

set(TS_FILES lang/wmm_ca_ES.ts)

set(PROJECT_SOURCES
        main/main.cpp
        views/MainWindow.cpp
        views/MainWindow.h
        views/MainWindow.ui
        ${TS_FILES}
)

qt_add_executable(wmm
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    lib/CsvFile.h
    lib/CsvFile.cpp
    views/ImportFileView.h
    views/ImportFileView.cpp
    views/ImportFileView.ui
    dialogs/DatabaseSettings.h
    dialogs/DatabaseSettings.cpp
    dialogs/DatabaseSettings.ui
    lib/Database.h
    lib/Database.cpp
    views/CategorizeView.h
    views/CategorizeView.cpp
    views/CategorizeView.ui
    views/BrowseDataView.h
    views/BrowseDataView.cpp
    views/BrowseDataView.ui
    views/SqlCommandView.h
    views/SqlCommandView.cpp
    views/SqlCommandView.ui
)

qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

target_link_libraries(wmm PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(wmm PRIVATE Qt${QT_VERSION_MAJOR}::Sql)

include(GNUInstallDirs)
install(TARGETS wmm
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(wmm)
