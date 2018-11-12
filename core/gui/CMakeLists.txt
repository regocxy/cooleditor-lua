file(GLOB_RECURSE libsrc *.h *.cpp *.hpp *.c *.cc)

add_library (gui ${libsrc})

target_link_libraries(gui PUBLIC imgui)
# if(APPLE)
#     target_link_libraries(gui PUBLIC glfw)
#     target_link_libraries(gui PUBLIC "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
# endif()
