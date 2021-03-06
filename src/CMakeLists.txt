cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE "./externals/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(CMAKE_CXX_STANDARD 17)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(libsita)

find_package(fmt CONFIG REQUIRED)
find_package(eabase CONFIG REQUIRED)
find_package(eastl CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)



function(sita_source_group src_path src_files)
	foreach(FILE ${src_files}) 
		get_filename_component(PARENT_DIR "${FILE}" PATH)
		file(RELATIVE_PATH PARENT_DIR ${src_path} ${PARENT_DIR})
		string(REPLACE "/" "\\" GROUP "${PARENT_DIR}")
		set(GROUP "${GROUP}")
		source_group("${GROUP}" FILES "${FILE}")	
	endforeach()
endfunction()

function(sita_add_library target_name src_path)
	file(GLOB_RECURSE src_files
		"${src_path}/src/*.h"
		"${src_path}/src/*.cpp"
	)	
	sita_source_group(${src_path} "${src_files}")
	
	add_library(${target_name} ${src_files})	
	target_precompile_headers(${target_name} PUBLIC src/${target_name}/${target_name}-pch.h)
	target_include_directories(${target_name} PUBLIC src)
endfunction()

function(sita_add_executable target_name src_path)
	file(GLOB_RECURSE src_files
		"${src_path}/src/*.h"
		"${src_path}/src/*.cpp"
	)	
	sita_source_group(${src_path} "${src_files}")
	
	add_executable(${target_name} ${src_files})	
	target_precompile_headers(${target_name} INTERFACE src/${target_name}/${target_name}-pch.h)
endfunction()


add_subdirectory(core)
add_subdirectory(core/test)

add_subdirectory(net/common)
add_subdirectory(net/client)
add_subdirectory(net/server/base)
add_subdirectory(net/server/controller)
add_subdirectory(net/server/gateway)
add_subdirectory(net/server/login)
add_subdirectory(net/server/zone)
add_subdirectory(net/examples/sita_net_example_001)

add_subdirectory(render)


