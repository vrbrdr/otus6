include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(COMMON_SRCS
    ../vector2.hpp 
    ../list2.hpp
)

add_executable(containers_test 
  generic_tests.cpp 
  vector2_tests.cpp
  list2_tests.cpp
  cint.hpp 
  ${COMMON_SRCS})
target_link_libraries(containers_test GTest::gtest_main gmock)

include(GoogleTest)
gtest_discover_tests(containers_test)


