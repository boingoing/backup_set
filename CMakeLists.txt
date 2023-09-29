cmake_minimum_required (VERSION 3.8 FATAL_ERROR)

find_program (CLANG_TIDY_EXE NAMES "clang-tidy")
if (CLANG_TIDY_EXE)
  set (CMAKE_CXX_CLANG_TIDY clang-tidy)
endif ()

set (CMAKE_CXX_STANDARD 17)

project (backup_set)

include_directories (${PROJECT_SOURCE_DIR}/src)

set (BACKUP_SET_LIB_SOURCES
  ${PROJECT_SOURCE_DIR}/src/BackupSet.cc
  ${PROJECT_SOURCE_DIR}/src/BackupSetReader.cc
  ${PROJECT_SOURCE_DIR}/src/BackupSetWriter.cc)
add_library (backup_set_lib STATIC ${BACKUP_SET_LIB_SOURCES})

set (BACKUP_SET_COMPARE_SOURCES
  ${PROJECT_SOURCE_DIR}/src/BackupSetCompare.cc)
add_executable (backup_set_compare ${BACKUP_SET_COMPARE_SOURCES})
target_link_libraries (backup_set_compare backup_set_lib)

set (TESTRUNNER_SOURCES
  ${PROJECT_SOURCE_DIR}/src/test/Constants.cc
  ${PROJECT_SOURCE_DIR}/src/test/TestCaseContainer.cc
  ${PROJECT_SOURCE_DIR}/src/test/BackupSetTests.cc
  ${PROJECT_SOURCE_DIR}/src/test/TestRunner.cc)
add_executable (test_runner ${TESTRUNNER_SOURCES})
target_link_libraries (test_runner backup_set_lib)

if (MSVC)
  # disable some benign warnings on MSVC
  add_compile_options ("/Wall;/wd4514;/wd4625;/wd4626;/wd5026;/wd5027;/wd5045;/wd4710;/wd4820;")
else ()
  add_compile_options ("-Wall;")
endif ()
