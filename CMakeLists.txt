cmake_minimum_required(VERSION 3.0.0)
project(MyJson VERSION 0.1.0)

include(CheckCXXCompilerFlag)

check_cxx_compiler_flag("-std=c++2b" COMPILER_SUPPORTS_CXX23)
check_cxx_compiler_flag("-std=c++20" COMPILER_SUPPORTS_CXX20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++2b")
message("23 support " ${COMPILER_SUPPORTS_CXX23})

if(COMPILER_SUPPORTS_CXX23)
  message("set std 2b")
elseif(COMPILER_SUPPORTS_CXX20) #
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
  message("set std 20")
else()
  message(
    STATUS
      "The compiler ${CMAKE_CXX_COMPILER} has no C++20 support. Please use a different C++ compiler."
  )
endif()

include(CTest)
enable_testing()

add_subdirectory(LibMyJson)
add_subdirectory(tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
