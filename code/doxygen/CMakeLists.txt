
get_filename_component( CMAKE_DOXYGEN_DIRECTORY ${CMAKE_BINARY_DIR} PATH)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/doxygen/Doxyfile @ONLY)

add_custom_target(doc ALL
	${DOXYGEN_EXECUTABLE} 
	${CMAKE_CURRENT_BINARY_DIR}/doxygen/Doxyfile
	WORKING_DIRECTORY ${CMAKE_DOXYGEN_DIRECTORY}
	COMMENT "Generating API documentation with Doxygen" VERBATIM
)