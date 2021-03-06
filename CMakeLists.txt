cmake_minimum_required(VERSION 3.5)
project(easycc)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Set targets
set(PROJECT_DEV_LIB easyccdev)
set(PROJECT_PRO_LIB easyccpro)

# Configure directory of output file
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Copy resources
set(TEST_RES_PATH "resources/test")
file(COPY "${PROJECT_SOURCE_DIR}/${TEST_RES_PATH}/grammar/" DESTINATION "${PROJECT_BINARY_DIR}/bin/${TEST_RES_PATH}/grammar/")
file(COPY "${PROJECT_SOURCE_DIR}/${TEST_RES_PATH}/graph/" DESTINATION "${PROJECT_BINARY_DIR}/bin/${TEST_RES_PATH}/graph/")
file(COPY "${PROJECT_SOURCE_DIR}/${TEST_RES_PATH}/firstfollow/" DESTINATION "${PROJECT_BINARY_DIR}/bin/${TEST_RES_PATH}/firstfollow/")

# Store src cpp files in a variable
file(GLOB_RECURSE PROJECT_SOURCE_FILES "src/*.cpp")
list(REMOVE_ITEM PROJECT_SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/src/easycc/EasyCCDev.cpp"
        "${PROJECT_SOURCE_DIR}/src/easycc/EasyCCPro.cpp")

# Include easycc header files (e.g. same as -I flag)
include_directories(include)
include_directories(thirdparty/rapidjson/include)
include_directories(tools/include)

# Check if generate files exists, if so use them
if(EXISTS "${PROJECT_SOURCE_DIR}/bin/include/easycc/tools/StaticResources.h")
    include_directories(bin/include)
else()
    include_directories(tools/convert/include)
endif()

# Add boost
add_definitions(-DBOOST_LOG_DYN_LINK)
find_package(Boost 1.63.0 COMPONENTS log log_setup thread date_time filesystem system REQUIRED)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found!")
endif()

# Link libraries
link_libraries(${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})

# Compile with -std=c++11 flag
add_compile_options(-std=c++11)

# Add development library target
add_library(${PROJECT_DEV_LIB} ${PROJECT_SOURCE_FILES} src/easycc/EasyCCDev.cpp)

# Add production library target
add_library(${PROJECT_PRO_LIB} ${PROJECT_SOURCE_FILES} src/easycc/EasyCCPro.cpp)

# Add subdirectory CMakeLists.txt
add_subdirectory(tools/convert)
add_subdirectory(thirdparty/googletest)
add_subdirectory(test)

# Explicitely generate header files
add_custom_target(
        generate_files
        COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/bin/include/easycc/tools/
        COMMAND ${TOOL_CONVERT_RESOURCES}
        -t ${PROJECT_SOURCE_DIR}/tools/convert/include/easycc/tools/StaticResources.h
        -o ${PROJECT_SOURCE_DIR}/bin/include/easycc/tools/StaticResources.h
        /*SYNTAX_ERRORS*/=${SYNTAX_ERRORS}
        /*SYNTAX_GRAMMAR*/=${SYNTAX_GRAMMAR}
        /*LEXICAL_ERRORS*/=${LEXICAL_ERRORS}
        /*LEXICAL_CONFIG*/=${LEXICAL_CONFIG}
        /*LEXICAL_STATE_MACHINE*/=${LEXICAL_STATE_MACHINE}
        DEPENDS ${TOOL_CONVERT_RESOURCES}
)
