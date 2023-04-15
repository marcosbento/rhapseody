# 
# This file is part of the raphsEOdy project.
#
# Copyright (c) 2023 Marcos Bento (mb at adlibx dot io)
#
# Distributed under the Boost Software License, Version 1.0.
# (See file LICENSE.bsl.txt or find a copy at http://www.boost.org/LICENSE_1_0.txt)
#
##

option(CMAKE_EXPORT_COMPILE_COMMANDS
    "Enable export of the compile command database" ON)
option(USE_OPENMP
    "Use OpenMP" ON)
option(ENSURE_NO_WARNINGS
    "Make compilation warnings fail build" ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# See https://gitlab.kitware.com/cmake/community/-/wikis/doc/cmake/RPATH-handling for more details
set(CMAKE_SKIP_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
if(APPLE)
  set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
elseif(UNIX)
  set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
  set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
endif()
if (DEPS_BASE)
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH};${DEPS_BASE}/lib")
endif()
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


if(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU" OR ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")

  set(GLOBAL_COMPILE_OPTIONS 
    -Wall
    -Wextra
    -Wpedantic
    -pedantic-errors
    -Wempty-body
    -Wparentheses
    -Wunknown-pragmas
    -Wunreachable-code
    -Wold-style-cast
    # -Wundef -- Disabled! It's too noisy!
    -Wredundant-decls
    -Wwrite-strings
    -Wpointer-arith
    -Wcast-qual
    -Wmissing-include-dirs
    -Wcast-align
    -Wnon-virtual-dtor
    -Wctor-dtor-privacy
    -Wdisabled-optimization
    -Winvalid-pch
    -Woverloaded-virtual
    -Wconversion
    -Wfloat-conversion
    -Wsign-conversion
    -Wswitch-enum
    -Wdouble-promotion
    -Wzero-as-null-pointer-constant
    -Wnull-dereference
    -Wdeprecated
    -Wno-gnu-zero-variadic-macro-arguments
    -fPIC
    -fno-common
    -fvisibility=default
    $<$<CONFIG:RELEASE>:-funroll-loops>
    $<$<CONFIG:RELWITHDEBINFO>:-funroll-loops>
  )

  if(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    set(GLOBAL_COMPILE_OPTIONS ${GLOBAL_COMPILE_OPTIONS}
      # Specific GCC options...
      -Wnoexcept
      -Wtrampolines
      -Wuseless-cast
      -Wvector-operation-performance
      -Wsized-deallocation
      -Wduplicated-cond
    )

    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
      MESSAGE(STATUS "Using an old GCC (4.8 or less) implies using the pre-C++11 ABI")
      set(GLOBAL_DEFINITIONS ${GLOBAL_DEFINITIONS}
        -D_GLIBCXX_USE_CXX11_ABI=0
      )
    endif()
  endif()

  if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    set(GLOBAL_COMPILE_OPTIONS ${GLOBAL_COMPILE_OPTIONS}
      # Specific Clang options...
      -Wweak-vtables
      -Wenum-conversion
      -Wint-conversion
    )

  endif()

  if(ENSURE_NO_WARNINGS)
    message(STATUS "All Warnings will be considered ***ERROR***!")
    set(GLOBAL_COMPILE_OPTIONS ${GLOBAL_COMPILE_OPTIONS}
      -Werror
    )
  endif()

  option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)
  if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
      add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
      add_compile_options (-fcolor-diagnostics)
    endif ()
  endif ()

endif()

if(APPLE)
  set(MACOSX_MINIMUM_VERSION 10.14)
  set(GLOBAL_COMPILE_OPTIONS
    ${GLOBAL_COMPILE_OPTIONS}
    -mmacosx-version-min=${MACOSX_MINIMUM_VERSION}
  )
endif()

if(MSVC)

  set(GLOBAL_DEFINITIONS ${GLOBAL_DEFINITIONS}
    /D_CRT_SECURE_NO_WARNINGS
  )

  if(ENSURE_NO_WARNINGS)
    message(STATUS "All Warnings will be considered ***ERROR***!")
    set(GLOBAL_COMPILE_OPTIONS ${GLOBAL_COMPILE_OPTIONS}
      -WX
    )
  endif()
 

endif()

option(ENSURE_COLOURED_OUTPUT "Make sure that GCC/Clang produce coloured output" TRUE)
if("${CMAKE_GENERATOR}" STREQUAL "Ninja" AND ENSURE_COLORED_OUTPUT)
  message(STATUS "Ninja generator detected! Ensuring GNU/Clang produce coloured output...")
  if(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    set(GLOBAL_COMPILE_OPTIONS ${GLOBAL_COMPILE_OPTIONS} -fdiagnostics-color=always)
  elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    set(GLOBAL_COMPILE_OPTIONS ${GLOBAL_COMPILE_OPTIONS} -fdiagnostics-color)
  endif()
endif()

add_compile_options(${GLOBAL_COMPILE_OPTIONS})
add_definitions(${GLOBAL_DEFINITIONS})

if(USE_OPENMP)
  find_package(OpenMP)
  if(OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
  else()
    message(STATUS "'OpenMP' not found. Skipping setup...")
    set(USE_OPENMP OFF)
  endif()
else()
  message(STATUS "'OpenMP' explicitly disabled")
endif()
