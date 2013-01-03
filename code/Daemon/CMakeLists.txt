MESSAGE( "Building daemon target." )

#directories with headers for this target
INCLUDE_DIRECTORIES( 	${QT_INCLUDES}
						${Boost_INCLUDE_DIRS}
						${TIN_SOURCE_DIR}/Daemon/include
						${TIN_SOURCE_DIR}/Utilities/include 
)

#if some class inherits QObject (has Q_OBJECT macro) add it's header file here
SET( DAEMON_SRC
		${TIN_SOURCE_DIR}/Daemon/src/ClientCommunication.cpp
		${TIN_SOURCE_DIR}/Daemon/src/DaemonApplication.cpp
		${TIN_SOURCE_DIR}/Daemon/src/DaemonThread.cpp
		${TIN_SOURCE_DIR}/Daemon/src/File.cpp
		${TIN_SOURCE_DIR}/Daemon/src/FileReciver.cpp
		${TIN_SOURCE_DIR}/Daemon/src/FileSender.cpp
		${TIN_SOURCE_DIR}/Daemon/src/FileTransferListener.cpp
		${TIN_SOURCE_DIR}/Daemon/include/ServerConnection.h
		${TIN_SOURCE_DIR}/Daemon/src/ServerConnection.cpp
		${TIN_SOURCE_DIR}/Daemon/src/ServerConnectionListener.cpp
		${TIN_SOURCE_DIR}/Daemon/src/main.cpp
		${TIN_SOURCE_DIR}/Daemon/src/DaemonConfiguration.cpp
)

add_executable( daemon ${DAEMON_SRC} )

#here add some additional qt modules
TARGET_LINK_LIBRARIES(	daemon
						${QT_LIBRARIES}
						${QT_QTNETWORK_LIBRARIES}
						${Boost_LIBRARIES}
						${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libutilities.a
)