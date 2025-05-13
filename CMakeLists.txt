cmake_minimum_required(VERSION 3.21)

if (POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif ()

if (POLICY CMP0126)
    cmake_policy(SET CMP0126 NEW)
endif ()

project(crow-auth VERSION 0.0.1 LANGUAGES CXX)

enable_language(C)
enable_language(CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)

option(CROW_AUTH_ENABLE_TESTS "enable testing" ON)

CPMAddPackage(Crow
  VERSION 1.2.1.2
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
  VERSION 1.1.0
  GITHUB_REPOSITORY TartanLlama/expected
  OPTIONS
  "EXPECTED_BUILD_TESTS Off"
)

CPMAddPackage(bcrypt
  GIT_TAG master
  GITHUB_REPOSITORY trusch/libbcrypt
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

# main executable
add_executable(${PROJECT_NAME}_main src/main.cpp)
target_link_libraries(${PROJECT_NAME}_main
        PRIVATE
        ${PROJECT_NAME}
)
target_include_directories(${PROJECT_NAME}_main PRIVATE ${expected_SOURCE_DIR}/include)
target_link_directories(${PROJECT_NAME}_main PRIVATE ${bcrypt_BINARY_DIR})

# auth db example
add_executable(example_auth_db src/example_auth_db.cpp)
target_link_libraries(example_auth_db
        PRIVATE
        ${PROJECT_NAME}
)
target_include_directories(example_auth_db PRIVATE ${expected_SOURCE_DIR}/include)
target_link_directories(example_auth_db PRIVATE ${bcrypt_BINARY_DIR})

include(CTest)
if(${CROW_AUTH_ENABLE_TESTS})

  enable_testing()

  CPMAddPackage(Catch2
    VERSION 3.8.1
    GITHUB_REPOSITORY catchorg/Catch2
    OPTIONS
    "CATCH_INSTALL_DOCS Off"
    "CATCH_INSTALL_EXTRAS Off"
    "CATCH_ENABLE_REPRODUCIBLE_BUILD Off"
  )
  add_subdirectory(test)

endif()
