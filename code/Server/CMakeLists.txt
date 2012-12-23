MESSAGE( "Building server target." )

#directories with headers for this target
INCLUDE_DIRECTORIES( 	${QT_INCLUDES}
						${Boost_INCLUDE_DIRS}
						${TIN_SOURCE_DIR}/Server/include
						${TIN_SOURCE_DIR}/Utilities/include
						${TIN_SOURCE_DIR}/qtsingleapplication/include 
)

#if some class inherits QObject (has Q_OBJECT macro) add it's header file here
SET( SERVER_SRC
		${TIN_SOURCE_DIR}/Server/src/Alias.cpp
		${TIN_SOURCE_DIR}/Server/src/ClientConnection.cpp
		${TIN_SOURCE_DIR}/Server/src/ClientConnectionListener.cpp
		${TIN_SOURCE_DIR}/Server/src/DaemonConnection.cpp
		${TIN_SOURCE_DIR}/Server/src/DaemonConnectionListener.cpp
		${TIN_SOURCE_DIR}/Server/src/FileTransferListener.cpp
		${TIN_SOURCE_DIR}/Server/src/FileTransferServer.cpp
		${TIN_SOURCE_DIR}/Server/include/MainServer.h
		${TIN_SOURCE_DIR}/Server/src/MainServer.cpp
		${TIN_SOURCE_DIR}/Server/include/UnknownConnection.h
		${TIN_SOURCE_DIR}/Server/src/UnknownConnection.cpp
		${TIN_SOURCE_DIR}/Server/src/UnknownConnectionListener.cpp
		${TIN_SOURCE_DIR}/Server/src/TcpServer.cpp
		${TIN_SOURCE_DIR}/Server/src/main.cpp
)

add_executable( server ${SERVER_SRC} )

#here add some additional qt modules
TARGET_LINK_LIBRARIES(	server
						${QT_LIBRARIES}
						${QT_QTNETWORK_LIBRARY}
						${Boost_LIBRARIES}
						${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libutilities.a
						
						#linking library for QtSingleCoreApplication
						${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libbuildlib.so
)