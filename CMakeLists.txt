cmake_minimum_required(VERSION 3.30)

project(NARIS VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_GENERATOR_PLATFORM x64)
set(CMAKE_CXX_FLAGS_RELEASE "-Os -s")

# Files
set(DIRECTX_SDK_PATH "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)")

file(GLOB_RECURSE SOURCES
	"src/**.cpp"

	"include/**.cpp"
)

file(GLOB_RECURSE HEADERS
	"src/**.h"
	"src/**.hpp"

	"include/**.h"
	"include/**.hpp"
)

include_directories("include")

include_directories("${DIRECTX_SDK_PATH}/Include")
link_directories("${DIRECTX_SDK_PATH}/Lib/x64")

# Stuff
add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(
	${PROJECT_NAME} PROPERTIES

	OUTPUT_NAME "naris"
	PREFIX ""
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
	WIN32_LEAN_AND_MEAN
	UNICODE=1
	VERSION="${CMAKE_PROJECT_VERSION}"
)

target_link_libraries(${PROJECT_NAME} d3d9 d3dx9 dwmapi)

# Yay
install(
	TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin
)
