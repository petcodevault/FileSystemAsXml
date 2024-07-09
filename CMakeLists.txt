cmake_minimum_required(VERSION 3.14)

project(FileSystemAsXml LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(FileSystemAsXml
  main.cpp
  file.cpp
  file.h
  hashrunnable.h hashrunnable.cpp
  fileprocessor.h fileprocessor.cpp
  xmlexporter.h xmlexporter.cpp
  xmlimporter.h xmlimporter.cpp
  csvexporter.h csvexporter.cpp
  savefileutils.h savefileutils.cpp
  restorefileutils.h restorefileutils.cpp
)
target_link_libraries(FileSystemAsXml Qt${QT_VERSION_MAJOR}::Core)

include(GNUInstallDirs)
install(TARGETS FileSystemAsXml
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
