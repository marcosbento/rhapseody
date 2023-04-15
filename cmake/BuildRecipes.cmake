# 
# This file is part of the raphsEOdy project.
#
# Copyright (c) 2023 Marcos Bento (mb at adlibx dot io)
#
# Distributed under the Boost Software License, Version 1.0.
# (See file LICENSE.bsl.txt or find a copy at http://www.boost.org/LICENSE_1_0.txt)
#
##

function(add_rhapseody_library)

  set(options ALL)
  set(oneValueArgs TARGET)
  set(multiValueArgs SOURCES HEADERS INCLUDE_DIRS LIBRARIES DEFINITIONS)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  add_library(${ARGS_TARGET} SHARED)

  target_sources(${ARGS_TARGET}
    PUBLIC
      ${ARGS_HEADERS}
    PRIVATE
      ${ARGS_SOURCES})

  target_compile_definitions(${ARGS_TARGET}
    PRIVATE
    ${ARGS_DEFINITIONS})

  target_include_directories(${ARGS_TARGET}
    PUBLIC
      ${ARGS_INCLUDE_DIRS})

  target_link_libraries(${ARGS_TARGET}
    PUBLIC
      ${ARGS_LIBRARIES})

  set_property(
    TARGET ${ARGS_TARGET}
    PROPERTY POSITION_INDEPENDENT_CODE ON)

  install(
    TARGETS ${ARGS_TARGET}
    DESTINATION lib)

endfunction()


function(add_rhapseody_executable)

  set(options ALL)
  set(oneValueArgs TARGET)
  set(multiValueArgs SOURCES HEADERS INCLUDE_DIRS LIBRARIES LIBRARY_DIRS DEFINITIONS)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  add_executable(${ARGS_TARGET})

  target_sources(${ARGS_TARGET}
    PRIVATE
    ${ARGS_HEADERS}
    ${ARGS_SOURCES})

  target_include_directories(${ARGS_TARGET}
    PRIVATE
    ${ARGS_INCLUDE_DIRS})

  target_compile_definitions(${ARGS_TARGET}
    PRIVATE
    ${ARGS_DEFINITIONS})

  target_link_libraries(${ARGS_TARGET}
    PRIVATE
    ${ARGS_LIBRARIES})

  install(
    TARGETS ${NAME}
    DESTINATION bin)

endfunction()

function(add_rhapseody_utest)

  set(options ALL)
  set(oneValueArgs TARGET)
  set(multiValueArgs SOURCES HEADERS INCLUDE_DIRS LIBRARIES LIBRARY_DIRS DEFINITIONS)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  add_executable(${ARGS_TARGET})

  target_sources(${ARGS_TARGET}
    PRIVATE
    ${ARGS_HEADERS}
    ${ARGS_SOURCES})

  target_include_directories(${ARGS_TARGET}
    PRIVATE
    ${ARGS_INCLUDE_DIRS})

  target_compile_definitions(${ARGS_TARGET}
    PRIVATE
    ${ARGS_DEFINITIONS})

  target_link_libraries(${ARGS_TARGET}
    PRIVATE
    ${ARGS_LIBRARIES})

  add_test(
    NAME ${ARGS_TARGET}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ${ARGS_TARGET})
  set_tests_properties(${ARGS_TARGET} PROPERTIES
    ENVIRONMENT "LD_LIBRARY_PATH=${DEPS_BASE}/lib")

endfunction()

function(add_rhapseody_docs)

  install(
    FILES
      "${CMAKE_SOURCE_DIR}/README.md"
      "${CMAKE_SOURCE_DIR}/LICENSE.bsl.txt"
    DESTINATION
      "${CMAKE_INSTALL_PREFIX}")

endfunction()
