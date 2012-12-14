cmake_minimum_required (VERSION 2.6) 

#project name
project (TIN) 

#look for boost library (required for this project)
FIND_PACKAGE( Boost REQUIRED )

#look for qt library (required for this project)
FIND_PACKAGE( Qt4 REQUIRED )

#look for doxygen to make documentation
find_package(Doxygen REQUIRED)

INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS(${QT_DEFINITIONS})

SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTXMLPATTERNS TRUE)
SET(QT_DONT_USE_QTGUI TRUE)

#turn on automoc
SET(CMAKE_AUTOMOC TRUE)

SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

#subdirectories
add_subdirectory(Utilities)
add_subdirectory(Client)
add_subdirectory(Daemon)
add_subdirectory(Server)
#add_subdirectory(doxygen)

