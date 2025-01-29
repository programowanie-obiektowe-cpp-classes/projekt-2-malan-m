
cmake_minimum_required(VERSION 3.14)
project(cpp-project CXX)

# Ustaw standard C++
set(CMAKE_CXX_STANDARD 23)

# Dodaj katalog z biblioteką TinyXML2
add_subdirectory(libs/tinyxml2)

# Pliki źródłowe projektu
file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(cpp-project ${SRC_FILES})

# Ścieżki do nagłówków
target_include_directories(cpp-project PUBLIC "${CMAKE_SOURCE_DIR}/include")

# Połącz z biblioteką tinyxml2
target_link_libraries(cpp-project PRIVATE tinyxml2)


#cmake_minimum_required(VERSION 3.14)
#project(cpp-project CXX)


#file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
#add_executable(cpp-project ${SRC_FILES})
#target_include_directories(cpp-project PUBLIC "${CMAKE_SOURCE_DIR}/include")
#set_target_properties(cpp-project PROPERTIES CXX_STANDARD 23)
