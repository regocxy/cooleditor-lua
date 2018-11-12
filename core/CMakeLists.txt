add_subdirectory(gui)
add_subdirectory(nativefd)

add_library (core core.cpp)

target_link_libraries(core PUBLIC gui)
target_link_libraries(core PUBLIC nativefd)
if(MINGW)
    target_link_libraries(core PUBLIC "-static")
endif()

target_include_directories (core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})