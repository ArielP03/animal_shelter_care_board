cmake_minimum_required(VERSION 3.20)

project(animal_shelter_care_board LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(PREPEND CMAKE_PREFIX_PATH
	"/opt/homebrew/opt/qt@6"
	"/opt/homebrew"
	"/usr/local/opt/qt@6"
	"/usr/local"
)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(animal_shelter_care_board
	src/main.cpp
	src/care_board.cpp
)

target_include_directories(animal_shelter_care_board PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(animal_shelter_care_board PRIVATE Qt6::Widgets)
target_compile_features(animal_shelter_care_board PRIVATE cxx_std_20)

if (MSVC)
	target_compile_options(animal_shelter_care_board PRIVATE /W4)
else()
	target_compile_options(animal_shelter_care_board PRIVATE -Wall -Wextra -Wpedantic)
endif()
