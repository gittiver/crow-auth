cmake_minimum_required(VERSION 3.21)
project(crow_test)

set(CMAKE_POLICY_DEFAULT_CMP0077 new)
set(CMAKE_POLICY_WARNING_CMP0126 new)
  
enable_testing()

set(TEST_SRCS
        auth_user_test.cpp
  unittest.cpp
)

add_executable(unittest ${TEST_SRCS})
target_include_directories(unittest PRIVATE ../src ${bcrypt_SOURCE_DIR}/include)
target_link_libraries(unittest crow-auth bcrypt Catch2::Catch2WithMain)


add_test(NAME unittest COMMAND unittest)
