project(Test)
include(FetchContent)
# 将Googletest下载到本地的build当中，作为本地项目的一部分。所有googletest的目标都将可见。
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.11.0)

FetchContent_MakeAvailable(googletest)

# 将下载下来的googletest编译成库 使用IMPORTED关键字
add_library(GTest::GTest INTERFACE IMPORTED)
# 链接gtest
target_link_libraries(GTest::GTest INTERFACE gtest_main)

# 将我们写的tests编译成可执行文件
add_executable(${PROJECT_NAME} test.cpp)
add_dependencies(${PROJECT_NAME} LibMyJson)



target_include_directories(${PROJECT_NAME} PUBLIC "../LibMyJson")
# 把GTest和被测函数都链接到 可执行文件 上
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::GTest LibMyJson)

# 增加测试
add_test(multiply_gtests ${PROJECT_NAME})
