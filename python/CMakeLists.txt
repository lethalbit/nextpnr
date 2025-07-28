add_library(nextpnr_python INTERFACE)

target_include_directories(nextpnr_python INTERFACE .)

target_sources(nextpnr_python INTERFACE
	pyarch.h
	pybindings_ng.h
)

if (BUILD_PYTHON)
	target_sources(nextpnr_python INTERFACE
		pybindings_ng.cc
	)

	target_include_directories(nextpnr_python INTERFACE
		${Python3_INCLUDE_DIRS}
	)

	target_link_libraries(nextpnr_python INTERFACE
		pybind11::headers
	)

	set(NEXTPNR_PYTHON_LIBS "${NEXTPNR_DATA_DIR}/python")

	install(DIRECTORY repl DESTINATION ${NEXTPNR_PYTHON_LIBS})
	install(DIRECTORY examples DESTINATION ${NEXTPNR_PYTHON_LIBS})
endif()
