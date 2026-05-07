cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0072 NEW)

project(gamatet3d)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(.)

add_executable(gamatet3d
        anim.cpp
        anim.h
        brick.cpp
        brick.h
        brickfield.cpp
        brickfield.h
        color.h
        gamatet.cpp
        menu.cpp
        menu.h
        random.h
        stb/stb_image.h
        tet3d.cpp
        tet3d.h
        timer.cpp
        timer.h)

target_link_libraries(gamatet3d
        OpenGL::GL
	OpenGL::GLU
        GLUT::GLUT
)

add_custom_command(TARGET gamatet3d POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/texture
        $<TARGET_FILE_DIR:gamatet3d>/texture
)
