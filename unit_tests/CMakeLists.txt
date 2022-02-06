
include(FetchContent)


FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG 216713a4066b79d9803d374f261ccb30c0fb451f
)

FetchContent_MakeAvailable(Catch2)

add_executable(${PROJECT_NAME}_validity
 "main.cpp")

target_link_libraries(${PROJECT_NAME}_validity acl::acl Catch2::Catch2)
add_test(${PROJECT_NAME}_validity ${PROJECT_NAME}_validity)
target_compile_features(${PROJECT_NAME}_validity PRIVATE cxx_std_20)

if(ASAN_ENABLED)
  if(MSVC)
    message("-- Address santizer is active for acl unit_tests")
    target_compile_options(${PROJECT_NAME}_validity PUBLIC /fsanitize=address)
  else()
    target_compile_options(${PROJECT_NAME}_validity PUBLIC -fsanitize=address)
    target_link_options(${PROJECT_NAME}_validity PUBLIC -fsanitize=address)
  endif()
endif()
