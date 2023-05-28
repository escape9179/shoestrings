cmake_minimum_required(VERSION 3.15)
project(CPPTest)

set(CMAKE_CXX_STANDARD 20)

add_executable(CPPTest main.cpp Entity.cpp Entity.h Color.cpp Color.h EntityType.h Enemy.cpp Enemy.h Bullet.cpp Bullet.h Player.cpp Player.h)