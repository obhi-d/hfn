cmake_minimum_required(VERSION 3.1)

project(hfn VERSION 0.0.1 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

option(${PROJECT_NAME}_BUILD_TESTS "Build the unit tests when BUILD_TESTING is enabled." ON)
option(ASAN_ENABLED "Build this target with AddressSanitizer" OFF)

##
## CONFIGURATION
##
set(${PROJECT_NAME}_TARGET_NAME               ${PROJECT_NAME})
set(${PROJECT_NAME}_INCLUDE_BUILD_DIR "${PROJECT_SOURCE_DIR}/include")

include(GNUInstallDirs)
include(FetchContent)

FetchContent_Declare(
  xxhash
  GIT_REPOSITORY https://github.com/Cyan4973/xxHash.git
  GIT_TAG 35b0373c697b5f160d3db26b1cbb45a0d5ba788c
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
)

FetchContent_Declare(
  wyhash
  GIT_REPOSITORY https://github.com/wangyi-fudan/wyhash.git
  GIT_TAG 4103e76eaf5077970b160990804c51ad2d5497df
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
)

FetchContent_MakeAvailable(xxhash wyhash)
##
## TARGET
##
message("Target name: ${${PROJECT_NAME}_TARGET_NAME} and links to ${xxhash_SOURCE_DIR}, ${wyhash_SOURCE_DIR}")
add_library(${${PROJECT_NAME}_TARGET_NAME} 
  "src/murmur3.cpp")

add_library(${PROJECT_NAME}::${${PROJECT_NAME}_TARGET_NAME} ALIAS ${${PROJECT_NAME}_TARGET_NAME})

target_include_directories(
    ${${PROJECT_NAME}_TARGET_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${wyhash_SOURCE_DIR}>
    $<BUILD_INTERFACE:${xxhash_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_sources(${${PROJECT_NAME}_TARGET_NAME} PRIVATE ${xxhash_SOURCE_DIR}/xxhash.c)

##
## TESTS
##
include(CTest) #adds option BUILD_TESTING (default ON)

if(BUILD_TESTING AND ${PROJECT_NAME}_BUILD_TESTS)
    enable_testing()
    add_subdirectory(unit_tests EXCLUDE_FROM_ALL)
endif()

##
## INSTALL
##
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion)

configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION
      cmake
)


 install(EXPORT ${PROJECT_NAME}_Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION cmake)

install(FILES 
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION cmake)

install(DIRECTORY include/${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES 
    ${wyhash_SOURCE_DIR}/wyhash.h
    ${wyhash_SOURCE_DIR}/wyhash32.h
    ${xxhash_SOURCE_DIR}/xxhash.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

