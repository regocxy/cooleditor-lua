file(GLOB_RECURSE libsrc *.h *.cpp *.hpp *.c *.cc)

add_executable (editor ${libsrc})
include_directories(${GLFW3_INCLUDE_DIRS})
target_link_libraries(editor PUBLIC lua)
target_link_libraries(editor PUBLIC cocos2d)
target_link_libraries(editor PUBLIC core)