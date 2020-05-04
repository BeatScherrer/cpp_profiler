cmake_minimum_required(VERSION 3.1)

project(cpp_profiling VERSION 0.0.1 LANGUAGES CXX)

# ------------------------------------------------------------------------------
# Dependencies
# ------------------------------------------------------------------------------

# GLog
find_package(glog 0.4.0 REQUIRED)

# ------------------------------------------------------------------------------
# Build
# ------------------------------------------------------------------------------

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Profiling library
add_library(${PROJECT_NAME}
  src/Instrumentor.cpp
  src/InstrumentationTimer.cpp
)

# c++17
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_EXTENSIONS OFF # disable compiler extensions to avoid issues with c++17
  CMAKE_POSITION_INDEPENDENT_CODE T # -fPIC
)

# Include directories
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# namespaced alias
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# link
target_link_libraries(${PROJECT_NAME}
  glog::glog
)

# ------------------------------------------------------------------------------
# Installation
# ------------------------------------------------------------------------------
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# Install files
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)
install(DIRECTORY include/ DESTINATION include)

# ...Config-version.cmake
write_basic_package_version_file(
  ${PROJECT_NAME}ConfigVersion.cmake
  VERSION ${PACKAGE_VERSION}
  COMPATIBILITY AnyNewerVersion
)

# export the targets directly in the ...Config.cmake file if not building for catkin
# export targets separatly and include in ...Config.cmake if dependencies need to be
# included first!
if(NOT BUILD_FOR_CATKIN)
  install(EXPORT ${PROJECT_NAME}_targets
          FILE
            ${PROJECT_NAME}Config.cmake
          NAMESPACE
            ${PROJECT_NAME}::
          DESTINATION
            ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
else()
  message(STATUS "building for catkin, not exporting targets")
endif()

# ------------------------------------------------------------------------------
# Doxygen
# ------------------------------------------------------------------------------
if(BUILD_DOC)
  message(STATUS "building documentation...")

  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    # set input and output file
    set (DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set (DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    #option ALL allows to build the docs together with the project
    add_custom_target(dox_doxygen ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM
    )
  else (DOXYGEN_FOUND)
    message ("Doxygen need to be installed to generated the doxygen documentation")
  endif ()
endif()