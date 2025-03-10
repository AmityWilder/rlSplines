cmake_minimum_required(VERSION 3.11)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(rlsplines C)

# Config options
option(BUILD_RLSPLINES_EXAMPLES "Build the examples." OFF)

# Force building examples if building in the root as standalone.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(BUILD_RLSPLINES_EXAMPLES TRUE)
endif()

# Directory Variables
set(RLSPLINES_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(RLSPLINES_SRC ${RLSPLINES_ROOT}/src)
set(RLSPLINES_EXAMPLES ${RLSPLINES_ROOT}/examples)

# rlsplines
add_library(rlsplines INTERFACE)
file(GLOB sources ${RLSPLINES_SRC}/*.h)
set(RLSPLINES_HEADERS ${sources})
install(FILES
    ${RLSPLINES_HEADERS} DESTINATION include
)
target_include_directories(rlsplines INTERFACE ${RLSPLINES_SRC})

# Examples
if(${BUILD_RLSPLINES_EXAMPLES})
    find_package(Raylib)

    # Get the sources together
    set(spline_2d
        spline_3d
        gradient
    )

    set(example_sources)
    set(example_resources)

    foreach(example_dir ${example_dirs})
        # Get the .c files
        file(GLOB sources ${RLSPLINES_EXAMPLES}/${example_dir}/*.c)
        list(APPEND example_sources ${sources})

        # Any any resources
        file(GLOB resources ${RLSPLINES_EXAMPLES}/${example_dir}/resources/*)
        list(APPEND example_resources ${resources})
    endforeach()

    # Do each example
    foreach(example_source ${example_sources})
        # Create the basename for the example
        get_filename_component(example_name ${example_source} NAME)
        string(REPLACE ".c" "${OUTPUT_EXT}" example_name ${example_name})

        # Setup the example
        add_executable(${example_name} ${example_source})

        target_link_libraries(${example_name} PUBLIC raylib rlsplines)

        string(REGEX MATCH ".*/.*/" resources_dir ${example_source})
        string(APPEND resources_dir "resources")
    endforeach()

    # Copy all of the resource files to the destination
    file(COPY ${example_resources} DESTINATION "resources/")
endif()
