cmake_minimum_required(VERSION 3.22)
project(aquarium-rock VERSION 1.0
        LANGUAGES C
        DESCRIPTION "Data structures in C")

set(CMAKE_C_STANDARD 11)
# Dependencies
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

# Sources
set(EXPORTED_HEADER_FILES
        include/rock/error.h
        include/rock/red_black_tree.h
        include/rock/tree_map.h
        include/rock/tree_set.h
        include/rock.h)
set(SOURCES
        ${EXPORTED_HEADER_FILES}
        src/private/red_black_tree.h
        src/error.c
        src/red_black_tree.c
        src/rock.c
        src/tree_map.c
        src/tree_set.c)

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
                ${CMOCKA_SHARED_LIBRARY})
    target_include_directories(${PROJECT_NAME}
            PUBLIC
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")
    set(LINK_FLAGS "-Wl,--wrap=pthread_rwlock_init,--wrap=pthread_rwlock_destroy,--wrap=pthread_rwlock_rdlock,--wrap=pthread_rwlock_wrlock,--wrap=pthread_rwlock_unlock,--wrap=pthread_mutex_init,--wrap=pthread_mutex_destroy,--wrap=pthread_mutex_lock,--wrap=pthread_mutex_unlock,--wrap=pthread_cond_init,--wrap=pthread_cond_destroy,--wrap=pthread_cond_wait,--wrap=pthread_cond_broadcast,--wrap=pthread_cond_signal,--wrap=nanosleep")
    # Unit Tests
    enable_testing()
    # rock-unit-test
    add_executable(rock-unit-test test/test_rock.c)
    target_include_directories(rock-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(rock-unit-test
            PRIVATE
                ${PROJECT_NAME})
    set_target_properties(rock-unit-test
            PROPERTIES
                LINK_FLAGS ${LINK_FLAGS})
    add_test(rock-unit-test rock-unit-test)
    # red-black-tree-unit-test
    add_executable(red-black-tree-unit-test test/test_red_black_tree.c)
    target_include_directories(red-black-tree-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(red-black-tree-unit-test
            PRIVATE
                ${PROJECT_NAME})
    set_target_properties(red-black-tree-unit-test
            PROPERTIES
                LINK_FLAGS ${LINK_FLAGS})
    add_test(red-black-tree-unit-test red-black-tree-unit-test)
    # tree-set-unit-test
    add_executable(tree-set-unit-test test/test_tree_set.c)
    target_include_directories(tree-set-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(tree-set-unit-test
            PRIVATE
                ${PROJECT_NAME})
    set_target_properties(tree-set-unit-test
            PROPERTIES
                LINK_FLAGS ${LINK_FLAGS})
    add_test(tree-set-unit-test tree-set-unit-test)
    # tree-map-unit-test
    add_executable(tree-map-unit-test test/test_tree_map.c)
    target_include_directories(tree-map-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(tree-map-unit-test
            PRIVATE
                ${PROJECT_NAME})
    set_target_properties(tree-map-unit-test
            PROPERTIES
                LINK_FLAGS ${LINK_FLAGS})
    add_test(tree-map-unit-test tree-map-unit-test)
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
                ${CMAKE_THREAD_LIBS_INIT})
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
