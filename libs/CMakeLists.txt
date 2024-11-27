cmake_minimum_required(VERSION 3.14)

# Sockets for windows should be linked
add_subdirectory(Catch2)
set_target_properties(Catch2::Catch2 PROPERTIES COMPILE_FLAGS ${DEPENDANCES_WARNING_LEVEL})

if(USE_INJA)
    set(INJA_BUILD_TESTS OFF CACHE INTERNAL "disables the inja tests" FORCE)
    add_subdirectory("inja")
    set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/inja" APPEND PROPERTY VARIABLE BUILD_TESTING OFF)
    set_target_properties(inja PROPERTIES COMPILE_FLAGS ${DEPENDANCES_WARNING_LEVEL})
    add_definitions(-DUSE_INJA)
    list(APPEND EXTRA_LIBS pantor::inja)
endif()

add_subdirectory(url-matcher)
set_target_properties(url-matcher PROPERTIES COMPILE_FLAGS ${DEPENDANCES_WARNING_LEVEL})
