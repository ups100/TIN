MESSAGE( "Building Utilities libraries." )

#directories with headers for this target
INCLUDE_DIRECTORIES( 	${QT_INCLUDES}
						${Boost_INCLUDE_DIRS}
						${TIN_SOURCE_DIR}/Utilities/include 
)

#if some class inherits QObject (has Q_OBJECT macro) add it's header file here
SET( UTILITIES_SRC
		${TIN_SOURCE_DIR}/Utilities/src/AliasFileList.cpp
		${TIN_SOURCE_DIR}/Utilities/src/FileLocation.cpp
		${TIN_SOURCE_DIR}/Utilities/src/Message.cpp
		${TIN_SOURCE_DIR}/Utilities/src/Password.cpp
		${TIN_SOURCE_DIR}/Utilities/src/InterprocessName.cpp
		${TIN_SOURCE_DIR}/Utilities/src/CommunicationProtocol.cpp
		${TIN_SOURCE_DIR}/Utilities/src/FileTree.cpp
		${TIN_SOURCE_DIR}/Utilities/src/Identify.cpp
		${TIN_SOURCE_DIR}/Utilities/src/AliasTree.cpp
		${TIN_SOURCE_DIR}/Utilities/src/Identifier.cpp
)

add_library( utilities STATIC
			 ${UTILITIES_SRC} 
)

#here add some additional qt modules
TARGET_LINK_LIBRARIES(	utilities
						${QT_LIBRARIES}
						${Boost_LIBRARIES}
)