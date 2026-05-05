cmake_minimum_required(VERSION 4.2)
project(gamatet3d)

set(CMAKE_CXX_STANDARD 14)

# Find OpenGL (GL + GLU)
find_package(OpenGL REQUIRED)

# Find GLUT
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
        stb_image.h
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
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_SOURCE_DIR}/texture
        $<TARGET_FILE_DIR:gamatet3d>/texture
)
