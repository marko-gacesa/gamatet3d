# Debug build
#g++ -g -O0 -Wall -Werror gamatet.cpp tet3d.cpp menu.cpp anim.cpp brick.cpp brickfield.cpp timer.cpp -o gamatet3d -lGL -lglut

# Release build
g++ -s -O3 -Wall -Werror gamatet.cpp tet3d.cpp menu.cpp anim.cpp brick.cpp brickfield.cpp timer.cpp -o gamatet3d -lGL -lglut
