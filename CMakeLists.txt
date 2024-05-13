cmake_minimum_required(VERSION 3.5)

project(wmm VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "/home/xcarol/Qt/6.7.0/gcc_64/")

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql LinguistTools)

set(TS_FILES
    lang/wmm_ca_ES.ts
    lang/wmm_es_ES.ts
    lang/wmm_en.ts
)

set(PROJECT_SOURCES
    main/main.cpp
    views/MainWindow.cpp
    views/MainWindow.h
    views/MainWindow.ui
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
    views/DuplicatesView.h
    views/DuplicatesView.cpp
    views/DuplicatesView.ui
    views/CategoriesView.h
    views/CategoriesView.cpp
    views/CategoriesView.ui
    widgets/TransactionsTable.h
    widgets/TransactionsTable.cpp
    ${TS_FILES}
)

qt_add_executable(wmm
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

qt6_add_lupdate(
    SOURCES ${PROJECT_SOURCES}
    TS_FILES ${TS_FILES}
)

qt6_add_lrelease(
    TS_FILES ${TS_FILES}
    QM_FILES_OUTPUT_VARIABLE qm_files
)

target_link_libraries(wmm PRIVATE Qt6::Widgets)
target_link_libraries(wmm PRIVATE Qt6::Sql)

include(GNUInstallDirs)
install(TARGETS wmm
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES ${qm_files} DESTINATION .)

qt_finalize_executable(wmm)
