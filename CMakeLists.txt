# CMAKE version of Ubuntu 16.04 is 3.5.
cmake_minimum_required(VERSION 3.5)
project(SPW)

set(SRC)
set(INCL)
set(LINKLIBS)
set(COMPILE_OPTION)

set(SRC_Qt_MOC)
set(SRC_Qt)


# For Raspberry Pi 4
if(NOT MSVC)
	EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
	message( STATUS "Architecture: ${ARCHITECTURE}" )

	if(${ARCHITECTURE} STREQUAL "aarch64")
		SET(AARCH64 TRUE)
	endif()
endif()

include(${CMAKE_CURRENT_LIST_DIR}/lib/RtAudioWrapper/RtAudio.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/lib/RtAudioWrapper/Libsamplerate.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/lib/WAV/WAV.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/lib/SSTT_wrapper/SSTT.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/lib/jsonConfig/jsonConfig.cmake)


# Sources to be compiled by MOC
list(APPEND SRC_Qt_MOC
	${CMAKE_CURRENT_LIST_DIR}/src/WSetting.h
	${CMAKE_CURRENT_LIST_DIR}/src/WSPW.h
	${CMAKE_CURRENT_LIST_DIR}/src/WSSTT.h
)

list(APPEND SRC_Qt
	${CMAKE_CURRENT_LIST_DIR}/src/WSetting.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/WSPW.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/WSSTT.cpp
	
)

macro(display_elements
	DISPLAY_MESSAGE
	LIST_TO_DISPLAY)
    message("[ ${DISPLAY_MESSAGE} ]")
    foreach(_DISPLAY_ELEMENT ${LIST_TO_DISPLAY})
		message(STATUS ${_DISPLAY_ELEMENT})
    endforeach()
endmacro()

file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/tmp)

## MOC(Meta Object Compiler) of QtFramework
#display_elements(MOC_TARGET ${SRC_Qt_MOC})
	## sources to be processed by MOC
	list(APPEND SRC_Qt_MOC
	#${PROJECT_SOURCE_DIR}/
	)	
	
	## sources without MetaObject 
	list(APPEND SRC_Qt
	#${PROJECT_SOURCE_DIR}/
		)
	
	foreach(i  ${SRC_Qt_MOC})
		string(REPLACE "/" ";" parse_i_1 ${i})
		LIST(GET parse_i_1 -1 parse_i_2)
		string(REPLACE "." ";" parse_i_3 ${parse_i_2})
		LIST(GET parse_i_3 0 fisrt_element)
		
		execute_process(
			COMMAND moc ${i} -o ${PROJECT_SOURCE_DIR}/tmp/moc_${fisrt_element}.cpp
			WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin  
			 )
		
		list(APPEND SRC_Qt
			tmp/moc_${fisrt_element}.cpp
		)
	endforeach()

list(APPEND SRC
	src/main.cpp
	
	src/AudioRoutine.h
	src/AudioRoutine.cpp
  )
list(APPEND INCL
  ${CMAKE_CURRENT_LIST_DIR}/.
  )
list(APPEND LINKLIBS
  )
if(MSVC)
  list(APPEND COMPILE_OPTION
     /D_CONFIG_JSON="../config.json"
    )
else()
  list(APPEND COMPILE_OPTION
     -D_CONFIG_JSON="../config.json"
    )
endif()

list(APPEND SRC ${SRC_Qt} ${SRC_Qt_MOC})

if(NOT AARCH64)
list(APPEND INCL
	src
	tmp
	# QtFramework
	bin/include
	bin/include/QtCore
	bin/include/QtCore5Compat
	bin/include/QtGui
	bin/include/QtWidgets
	bin/include/QtOpenGL
	bin/include/QtOpenGLWidgets
)
else()
list(APPEND INCL
	src
	tmp

	/usr/include/aarch64-linux-gnu/qt5/
	/usr/include/aarch64-linux-gnu/qt5/QtCore/
	/usr/include/aarch64-linux-gnu/qt5/QtGui/
	/usr/include/aarch64-linux-gnu/qt5/QtWidgets/
)

endif()

## QtFramework ##
if(AARCH64)
  list(APPEND LINKLIBS
		Qt5Core
		Qt5Widgets
		Qt5Gui

		Qt5XcbQpa
	)
  list(APPEND COMPILE_OPTION
      -fPIC
    )

elseif(UNIX AND NOT APPLE)
 message( FATAL_ERROR "Not implemented for generic UNIX" )
  list(APPEND LINKLIBS
	)
  list(APPEND COMPILE_OPTION
      -fPIC
    )
elseif(MSVC)
	list(APPEND LINKLIBS
		debug ${PROJECT_SOURCE_DIR}/bin/lib/Qt6Cored.lib optimized ${PROJECT_SOURCE_DIR}/bin/lib/Qt6Core.lib
		debug ${PROJECT_SOURCE_DIR}/bin/lib/Qt6Guid.lib optimized ${PROJECT_SOURCE_DIR}/bin/lib/Qt6Gui.lib
		debug ${PROJECT_SOURCE_DIR}/bin/lib/Qt6Widgetsd.lib optimized ${PROJECT_SOURCE_DIR}/bin/lib/Qt6Widgets.lib	
		debug ${PROJECT_SOURCE_DIR}/bin/lib/Qt6OpenGLWidgetsd.lib optimized ${PROJECT_SOURCE_DIR}/bin/lib/Qt6OpenGLWidgets.lib	
	)
endif()


add_executable(${PROJECT_NAME} ${SRC})
target_include_directories(${PROJECT_NAME}    PRIVATE    ${INCL}  )
target_link_libraries(${PROJECT_NAME} ${LINKLIBS})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPTION} )
set_target_properties(${PROJECT_NAME} PROPERTIES  CXX_STANDARD 17 )