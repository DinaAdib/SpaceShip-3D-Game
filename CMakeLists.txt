# CMake entry point
cmake_minimum_required (VERSION 2.6)
project (Graphics_Lab4)

find_package(OpenGL REQUIRED)

# Compile external dependencies 
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-1c73e35")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)



include_directories(
	external/AntTweakBar-1.16/include/
	external/glfw-2.7.6/include/
	external/glm-0.9.4.0/
	external/glew-1.9.0/include/
	external/assimp-3.0.1270/include/
	external/bullet-2.81-rev2613/src/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	GLFW_276
	GLEW_190
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)


# Textures
add_executable( 1Textures
	1Textures/1Textures.cpp
	common/shader.cpp
	common/shader.hpp
	common/texture.cpp
	common/texture.hpp
	common/text2D.cpp
	common/text2D.hpp
	2Model_loading/Fireworks.cpp
	2Model_loading/Fireworks.hpp
)
target_link_libraries(1Textures
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(1Textures PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/1Textures/")
create_target_launcher(1Textures WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/1Textures/")


# Model Loading
add_executable(2Model_loading
	2Model_loading/2Model_loading.cpp
	2Model_loading/ObjectModel.cpp
	2Model_loading/ObjectModel.h
	2Model_loading/GameHeader.h
	2Model_loading/Buffers.h
	2Model_loading/Buffers.cpp
	2Model_loading/Spaceship.h
	2Model_loading/Spaceship.cpp
	2Model_loading/Fireworks.cpp
	2Model_loading/Fireworks.hpp
	common/shader.cpp
	common/shader.hpp
	common/controls.cpp
	common/controls.hpp
	common/texture.cpp
	common/texture.hpp
	common/objloader.cpp
	common/objloader.hpp
	common/text2D.cpp
	common/text2D.hpp
)
target_link_libraries(2Model_loading
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(2Model_loading PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/2Model_loading/")
create_target_launcher(2Model_loading WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/2Model_loading/")

# Lighting
add_executable(3Lighting
	3Lighting/3Lighting.cpp
	common/shader.cpp
	common/shader.hpp
	common/controls.cpp
	common/controls.hpp
	common/texture.cpp
	common/texture.hpp
	common/objloader.cpp
	common/objloader.hpp
	common/text2D.cpp
	common/text2D.hpp
)
target_link_libraries(3Lighting
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(3Lighting PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3Lighting/")
create_target_launcher(3Lighting WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/3Lighting/")




SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )


if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

add_custom_command(
   TARGET 1Textures POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/1Textures${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/1Textures/"
)

add_custom_command(
   TARGET 2Model_loading POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/2Model_loading${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/2Model_loading/"
)
add_custom_command(
   TARGET 3Lighting POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/3Lighting${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/3Lighting/"
)


elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )


