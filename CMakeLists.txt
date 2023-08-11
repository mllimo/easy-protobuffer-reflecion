cmake_minimum_required (VERSION 3.12)

project("EasyProtobufferReflex")

if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

option(BUILD_SHARED_LIBS "Build shared libs" false)

# conan profile
set(CONAN_PROFILE CACHE STRING "default")
# conanfile.txt file
set(CONANFILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/conanfile.txt")
# conan install command
set(CONAN_INSTALL_COMMAND "conan install ${CONANFILE_PATH} -pr ${CONAN_PROFILE} --build=missing --output-folder=${CMAKE_BINARY_DIR} --build=missing")

# run conan install
if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
    message("Running conan install... Please wait")
    execute_process(
        COMMAND powershell
            ${CONAN_INSTALL_COMMAND}
        OUTPUT_VARIABLE FOO
        RESULT_VARIABLE FOOR
        ERROR_VARIABLE  FOO_ER
    )
    message("conan install finished")
    message(${FOO_ER})
endif()

include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies.cmake)
include(GenerateExportHeader)

# include your folders
#add_subdirectory("app")
add_subdirectory("ProtoReflection")
add_subdirectory("Tests")

# install
install(TARGETS
  ProtoReflection
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include
)
  