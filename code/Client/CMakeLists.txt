MESSAGE( "Building client target." )

#directories with headers for this target
INCLUDE_DIRECTORIES( 	${QT_INCLUDES}
						${Boost_INCLUDE_DIRS}
						${TIN_SOURCE_DIR}/Client/include
						${TIN_SOURCE_DIR}/Utilities/include
						${TIN_SOURCE_DIR}/qtsingleapplication/include  
)

#if some class inherits QObject (has Q_OBJECT macro) add it's header file here
SET( CLIENT_SRC
		${TIN_SOURCE_DIR}/Client/src/AliasCommunicationListener.cpp
		${TIN_SOURCE_DIR}/Client/src/ClientApplication.cpp
		${TIN_SOURCE_DIR}/Client/src/ClientView.cpp
		${TIN_SOURCE_DIR}/Client/src/CommandParser.cpp
		${TIN_SOURCE_DIR}/Client/src/Commands.cpp
		${TIN_SOURCE_DIR}/Client/src/DaemonCommunication.cpp
		${TIN_SOURCE_DIR}/Client/include/ServerConnection.h
		${TIN_SOURCE_DIR}/Client/include/ClientApplication.h
		${TIN_SOURCE_DIR}/Client/include/ClientView.h
		${TIN_SOURCE_DIR}/Client/src/ServerConnection.cpp
		${TIN_SOURCE_DIR}/Client/src/ServerConnectionListener.cpp
		${TIN_SOURCE_DIR}/Client/src/main.cpp
		${TIN_SOURCE_DIR}/Client/src/Argument.cpp
)

add_executable( client ${CLIENT_SRC} )

#here add some additional qt modules
TARGET_LINK_LIBRARIES(	client
						${QT_LIBRARIES}
						${QT_QTNETWORK_LIBRARIES}
						${Boost_LIBRARIES}
						${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libutilities.a
						
						#linking library for QtSingleCoreApplication
						${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libbuildlib.so
)