cmake_minimum_required(VERSION 3.14)

# Sockets for windows should be linked
add_subdirectory(Catch2)
set_target_properties(Catch2 PROPERTIES COMPILE_FLAGS ${DEPENDANCES_WARNING_LEVEL})

if(WITH_INJA)
    add_subdirectory("inja")
    set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/inja" APPEND PROPERTY VARIABLE BUILD_TESTING OFF)
    set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/inja" APPEND PROPERTY VARIABLE BUILD_BENCHMARK OFF)
    set_target_properties(inja PROPERTIES COMPILE_FLAGS ${DEPENDANCES_WARNING_LEVEL})
    target_link_libraries(WebFrame_Library PRIVATE inja)
    target_compile_definitions(WebFrame_Library PRIVATE -DWITH_INJA)
endif()

add_subdirectory(url-matcher)
set_target_properties(regexmatcher_core PROPERTIES COMPILE_FLAGS ${DEPENDANCES_WARNING_LEVEL})
target_link_libraries(WebFrame_Library PUBLIC RegexMatcher::core)
