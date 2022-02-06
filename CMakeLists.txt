cmake_minimum_required(VERSION 3.1)

project(lib_template VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

option(${PROJECT_NAME}_BUILD_TESTS "Build the unit tests when BUILD_TESTING is enabled." ON)
option(ASAN_ENABLED "Build this target with AddressSanitizer" OFF)

##
## CONFIGURATION
##
set(${PROJECT_NAME}_TARGET_NAME               ${PROJECT_NAME})
set(${PROJECT_NAME}_INCLUDE_BUILD_DIR "${PROJECT_SOURCE_DIR}/include")

include(GNUInstallDirs)

##
## TARGET
##
message("Target name: ${${PROJECT_NAME}_TARGET_NAME}")
add_library(${${PROJECT_NAME}_TARGET_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${${PROJECT_NAME}_TARGET_NAME} ALIAS ${${PROJECT_NAME}_TARGET_NAME})

target_include_directories(
    ${${PROJECT_NAME}_TARGET_NAME}
    INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

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

