cmake_minimum_required(VERSION 3.1)

project(profiling VERSION 0.0.1 LANGUAGES CXX)

# ------------------------------------------------------------------------------
# Build
# ------------------------------------------------------------------------------

# Profiling library
add_library(${PROJECT_NAME}
  src/Instrumentor.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# ------------------------------------------------------------------------------
# Installation
# ------------------------------------------------------------------------------

include(CMakePackageConfigHelpers)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

write_basic_package_version_file(
  ${PROJECT_NAME}ConfigVersion.cmake
  VERSION ${PACKAGE_VERSION}
  COMPATIBILITY AnyNewerVersion
)

# export the targets directly in the ...Config.cmake file
install(EXPORT ${PROJECT_NAME}_targets
        FILE ${PROJECT_NAME}Config.cmake
        NAMESPACE ${PROJECT_NAME}::${PROJECT_NAME}
        DESTINATION lib/cmake/${PROJECT_NAME}
)
