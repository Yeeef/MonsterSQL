cmake_minimum_required(VERSION 3.5)
project(monsterSQL)

set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_STANDARD 11)
aux_source_directory(. DIR_SRCS)


add_executable(monsterSQL ${DIR_SRCS})

