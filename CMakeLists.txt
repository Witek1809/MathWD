cmake_minimum_required(VERSION 3.0)
project(MathWD)

set(CMAKE_BUILD_TYPE "Release")

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

## Ustawienia kompilatora ##########################################################################################
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # -O3 -o -Wl,-whole-archive
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
####################################################################################################################

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wl,-whole-archive ")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -O3 -Wl,-whole-archive")

## Sprawdzenie wsparcia dla OpenMP ###############################
find_package(OpenMP)
if(OPENMP_FOUND)
    message(STATUS "OpenMP found")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
else()
    message(FATAL_ERROR "Could not find GMP OpenMP")
endif()
##################################################################

# Sprawdzenie dostępności biblioteki CppuTest ##################################################################################
set(CPPUTEST_FOUND FALSE)
set(CPPUTEST_PREFIX "" CACHE PATH "path ")
find_path(CPPUTEST_INCLUDE_DIR TestHarness.h PATHS ${CPPUTEST_PREFIX}/include /usr/include/CppUTest /usr/local/include/CppUTest)
find_library(CPPUTEST_LIBRARY NAMES CppUTest libCppUTest PATHS ${CPPUTEST_PREFIX}/lib /usr/lib /usr/local/lib)
if(CPPUTEST_INCLUDE_DIR AND CPPUTEST_LIBRARY)
    get_filename_component(CPPUTEST_LIBRARY_DIR ${CPPUTEST_LIBRARY} PATH)
    set(CPPUTEST_FOUND TRUE)
endif()
if(CPPUTEST_FOUND)
   if(NOT CPPUTEST_FIND_QUIETLY)
      message(STATUS "CPPUTEST found: ${CPPUTEST_LIBRARY}")
   endif()
elseif(CPPUTEST_FOUND)
   if(CPPUTEST_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find CPPUTEST")
   endif()
endif()

set(CPPUTESTEXT_FOUND FALSE)
set(CPPUTESTEXT_PREFIX "" CACHE PATH "path ")
find_path(CPPUTESTEXT_INCLUDE_DIR GTest.h PATHS ${CPPUTESTEXT_PREFIX}/include /usr/include/CppUTestExt /usr/local/include/CppUTestExt)
find_library(CPPUTESTEXT_LIBRARY NAMES CppUTestExt libCppUTestExt PATHS ${CPPUTESTEXT_PREFIX}/lib /usr/lib /usr/local/lib)
if(CPPUTESTEXT_INCLUDE_DIR AND CPPUTESTEXT_LIBRARY)
    get_filename_component(CPPUTESTEXT_LIBRARY_DIR ${CPPUTESTEXT_LIBRARY} PATH)
    set(CPPUTESTEXT_FOUND TRUE)
endif()
if(CPPUTESTEXT_FOUND)
   if(NOT CPPUTESTEXT_FIND_QUIETLY)
      message(STATUS "CPPUTESTEXT found: ${CPPUTESTEXT_LIBRARY}")
   endif()
elseif(CPPUTESTEXT_FOUND)
   if(CPPUTESTEXT_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find CPPUTESTEXT")
   endif()
endif()
################################################################################################################################

## Sprawdzenie dostępności biblioteki GMP #############################################################
set(GMP_FOUND FALSE)
set(GMP_PREFIX "" CACHE PATH "path ")
find_path(GMP_INCLUDE_DIR gmp.h gmpxx.h PATHS ${GMP_PREFIX}/include /usr/include /usr/local/include)
find_library(GMP_LIBRARY NAMES gmp libgmp PATHS ${GMP_PREFIX}/lib /usr/lib /usr/local/lib)
if(GMP_INCLUDE_DIR AND GMP_LIBRARY)
    get_filename_component(GMP_LIBRARY_DIR ${GMP_LIBRARY} PATH)
    set(GMP_FOUND TRUE)
endif()
if(GMP_FOUND)
   if(NOT GMP_FIND_QUIETLY)
      message(STATUS "GMP found: ${GMP_LIBRARY}")
   endif()
elseif(GMP_FOUND)
   if(GMP_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find GMP")
   endif()
endif()
#######################################################################################################

## Sprawdzenie dostępności biblioteki NTL ####
find_library(NTL_LIB ntl)
if(NTL_LIB)
    message(STATUS "NTL found: ${NTL_LIB}")
else()
    message(FATAL_ERROR "Could not find NTL")
endif()
################################################

## Sprawdzenie dostępności biblioteki ECM ####
find_library(ECM_LIB ecm)
if(ECM_LIB)
    message(STATUS "ECM found: ${ECM_LIB}")
else()
    message(FATAL_ERROR "Could not find ECM")
endif()
##############################################



## Ustawienie plików nagłówkowych ############
set(HEADERS_FILE
    tools/print.h
    tools/sparse_vector_gf2.h
    tools/sparse_matrix_gf2.h
    ifp/canonical_distribution.h
    ifp/primes/select_primes.h
    ifp/ecm/ecm.h
    ifp/ecm/elliptic_curve.h
    ifp/ecm/elliptic_curve_arthmetic.h
    )
## Ustawienie plików źródłowych ##############
set(SOURCES_FILE
    tools/print.cc
    tools/sparse_vector_gf2.cc
    tools/sparse_matrix_gf2.cc
    ifp/canonical_distribution.cc
    ifp/primes/select_primes.cc
    ifp/ecm/ecm.cc
    ifp/ecm/elliptic_curve.cc
    ifp/ecm/elliptic_curve_arthmetic.cc
    )
##############################################

## Zdefiniowanie przestrzeni include ###################################
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${GMP_INCLUDE_DIR}
    ${CPPUTEST_INCLUDE_DIR}
    ${CPPUTESTEXT_INCLUDE_DIR}
    )
########################################################################

## Zbudowanie biblioteki ###############################################
add_library(${PROJECT_NAME} ${HEADERS_FILE} ${SOURCES_FILE})
target_link_libraries(${PROJECT_NAME} ${NTL_LIB} ${ECM_LIB} ${GMP_LIBRARY} pthread)
########################################################################

enable_testing()

## Zbudowanie programów testowych ############################################################################################
add_executable(MainProgram main.cc)
target_link_libraries(MainProgram
                      ${PROJECT_NAME} ${CPPUTESTEXT_LIBRARY} ${CPPUTEST_LIBRARY} ${NTL_LIB} ${ECM_LIB} ${GMP_LIBRARY} pthread)

add_executable(Algebra tests/algebra.cc)
target_link_libraries(Algebra
                      ${PROJECT_NAME} ${CPPUTESTEXT_LIBRARY} ${CPPUTEST_LIBRARY} ${NTL_LIB} ${ECM_LIB} ${GMP_LIBRARY} pthread)

add_executable(Factorial tests/factorial.cc)
target_link_libraries(Factorial
                      ${PROJECT_NAME} ${CPPUTESTEXT_LIBRARY} ${CPPUTEST_LIBRARY} ${NTL_LIB} ${ECM_LIB} ${GMP_LIBRARY} pthread)
###############################################################################################################################
