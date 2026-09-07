cmake_minimum_required(VERSION 3.21)

if (POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif ()

if (POLICY CMP0126)
    cmake_policy(SET CMP0126 NEW)
endif ()

project(crow-auth VERSION 0.0.3 LANGUAGES CXX)

enable_language(C)
enable_language(CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)

option(CROW_AUTH_ENABLE_TESTS "enable testing" ON)

CPMAddPackage(Crow
  VERSION 1.3.4
  GITHUB_REPOSITORY CrowCpp/Crow
  OPTIONS
  "CROW_BUILD_EXAMPLES Off"
  "CROW_BUILD_TOOLS Off"
  "CROW_BUILD_TESTS Off"
  "CROW_BUILD_DOCS Off"
  "CROW_ENABLE_COMPRESSION On"
  "CROW_ENABLE_SSL On"
)

# add tartan lamas expected library
CPMAddPackage(expected
  VERSION 1.3.1
  GITHUB_REPOSITORY TartanLlama/expected
  OPTIONS
  "EXPECTED_BUILD_TESTS Off"
)

CPMAddPackage(bcrypt
  GIT_TAG master
  GITHUB_REPOSITORY gittiver/libbcrypt
  OPTIONS "BUILD_SHARED_LIBS Off"
)
#message( "added ${CPM_LAST_PACKAGE_NAME}")
#message( "src=  ${bcrypt_SOURCE_DIR}")

# lib
add_library(${PROJECT_NAME}
  src/bp_user_registration.cpp
  src/bp_user_registration.hpp
  src/auth.cpp
  src/auth.hpp
  src/auth_user.hpp
  src/auth_user.cpp)
target_include_directories(
        ${PROJECT_NAME}
        PUBLIC ${CMAKE_SOURCE_DIR}/src
        PRIVATE ${bcrypt_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC tl::expected Crow::Crow bcrypt)
target_link_directories(${PROJECT_NAME} PUBLIC ${bcrypt_BINARY_DIR})

add_executable(own_authenticate "src/example/own_authenticate.cpp")
target_link_libraries(own_authenticate
        PRIVATE
        ${PROJECT_NAME}
)
Target_include_directories(own_authenticate PRIVATE ${expected_SOURCE_DIR}/include)
target_link_directories(own_authenticate PRIVATE ${bcrypt_BINARY_DIR})

add_executable(example_auth_db "src/example/example_auth_db.cpp")
target_link_libraries(example_auth_db
        PRIVATE
        ${PROJECT_NAME}
)
target_include_directories(example_auth_db PRIVATE ${expected_SOURCE_DIR}/include)

add_custom_target(resources ALL)
add_custom_command(
        TARGET resources POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
                ${CMAKE_SOURCE_DIR}/src/static
                ${CMAKE_CURRENT_BINARY_DIR}/static)

add_custom_command(
        TARGET resources POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
                ${CMAKE_SOURCE_DIR}/src/templates
                ${CMAKE_CURRENT_BINARY_DIR}/templates)


include(CTest)
if(${CROW_AUTH_ENABLE_TESTS})

  enable_testing()

  CPMAddPackage(Catch2
    VERSION 3.16.0
    GITHUB_REPOSITORY catchorg/Catch2
    OPTIONS
    "CATCH_INSTALL_DOCS Off"
    "CATCH_INSTALL_EXTRAS Off"
    "CATCH_ENABLE_REPRODUCIBLE_BUILD Off"
  )
  add_subdirectory(test)

endif()
