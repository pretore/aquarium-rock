cmake_minimum_required(VERSION 3.22)
project(aquarium-rock VERSION 2.0.0
        LANGUAGES C
        DESCRIPTION "Data structures in C")

set(CMAKE_C_STANDARD 11)
# Dependencies
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
find_package(Doxygen OPTIONAL_COMPONENTS dot mscgen dia)
include(cmake/FetchAquariumSeagrass.cmake)

# Sources
set(EXPORTED_HEADER_FILES
        include/rock/array.h
        include/rock/error.h
        include/rock/hash_table.h
        include/rock/red_black_tree.h
        include/rock/tree_map.h
        include/rock/tree_set.h
        include/rock.h)
set(SOURCES
        ${EXPORTED_HEADER_FILES}
        src/private/array.h
        src/private/hash_table.h
        src/private/red_black_tree.h
        src/array.c
        src/error.c
        src/hash_table.c
        src/red_black_tree.c
        src/rock.c
        src/tree_map.c
        src/tree_set.c)

if(DOXYGEN_FOUND)
    set(DOXYGEN_EXTRACT_ALL YES)
    doxygen_add_docs(${PROJECT_NAME}-documentation
            ${EXPORTED_HEADER_FILES})
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    include(cmake/FetchCMocka.cmake)
    add_library(${PROJECT_NAME} STATIC "")
    add_compile_definitions(TEST)
    target_sources(${PROJECT_NAME}
            PRIVATE
                ${SOURCES}
                src/test/cmocka.h
                src/test/wrap.c
                src/test/wrap.h)
    target_link_libraries(${PROJECT_NAME}
            PUBLIC
                ${CMAKE_THREAD_LIBS_INIT}
                ${CMOCKA_SHARED_LIBRARY}
                aquarium-seagrass)
    target_include_directories(${PROJECT_NAME}
            PUBLIC
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")
    # Unit Tests
    enable_testing()
    # aquarium-rock-unit-test
    add_executable(${PROJECT_NAME}-unit-test test/test_rock.c)
    target_include_directories(${PROJECT_NAME}-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(${PROJECT_NAME}-unit-test
            PRIVATE
                ${PROJECT_NAME})
    add_test(${PROJECT_NAME}-unit-test ${PROJECT_NAME}-unit-test)
    # aquarium-rock-red-black-tree-unit-test
    add_executable(${PROJECT_NAME}-red-black-tree-unit-test
            test/test_red_black_tree.c)
    target_include_directories(${PROJECT_NAME}-red-black-tree-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(${PROJECT_NAME}-red-black-tree-unit-test
            PRIVATE
                ${PROJECT_NAME})
    add_test(${PROJECT_NAME}-red-black-tree-unit-test
            ${PROJECT_NAME}-red-black-tree-unit-test)
    # aquarium-rock-tree-set-unit-test
    add_executable(${PROJECT_NAME}-tree-set-unit-test test/test_tree_set.c)
    target_include_directories(${PROJECT_NAME}-tree-set-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(${PROJECT_NAME}-tree-set-unit-test
            PRIVATE
                ${PROJECT_NAME})
    add_test(${PROJECT_NAME}-tree-set-unit-test
            ${PROJECT_NAME}-tree-set-unit-test)
    # aquarium-rock-tree-map-unit-test
    add_executable(${PROJECT_NAME}-tree-map-unit-test test/test_tree_map.c)
    target_include_directories(${PROJECT_NAME}-tree-map-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(${PROJECT_NAME}-tree-map-unit-test
            PRIVATE
                ${PROJECT_NAME})
    add_test(${PROJECT_NAME}-tree-map-unit-test
            ${PROJECT_NAME}-tree-map-unit-test)
    # aquarium-rock-array-unit-test
    add_executable(${PROJECT_NAME}-array-unit-test test/test_array.c)
    target_include_directories(${PROJECT_NAME}-array-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(${PROJECT_NAME}-array-unit-test
            PRIVATE
                ${PROJECT_NAME})
    add_test(${PROJECT_NAME}-array-unit-test ${PROJECT_NAME}-array-unit-test)
    # aquarium-rock-hash-table-unit-test
    add_executable(${PROJECT_NAME}-hash-table-unit-test test/test_hash_table.c)
    target_include_directories(${PROJECT_NAME}-hash-table-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(${PROJECT_NAME}-hash-table-unit-test
            PRIVATE
                ${PROJECT_NAME})
    add_test(${PROJECT_NAME}-hash-table-unit-test
            ${PROJECT_NAME}-hash-table-unit-test)
else()
    add_library(${PROJECT_NAME} "")
    target_sources(${PROJECT_NAME}
            PRIVATE
                ${SOURCES})
    target_include_directories(${PROJECT_NAME}
            PUBLIC
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")
    target_link_libraries(${PROJECT_NAME}
            PUBLIC
                ${CMAKE_THREAD_LIBS_INIT}
                aquarium-seagrass)
    set_target_properties(${PROJECT_NAME}
            PROPERTIES
                VERSION ${PROJECT_VERSION}
                SOVERSION ${PROJECT_VERSION_MAJOR})
    include(GNUInstallDirs)
    install(DIRECTORY include/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install(TARGETS ${PROJECT_NAME}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
    configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)
    install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()
