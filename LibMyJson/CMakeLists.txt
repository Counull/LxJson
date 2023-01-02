cmake_minimum_required(VERSION 3.0.0)
project(LibMyJson VERSION 0.1.0)

aux_source_directory(. SRC)


add_library (LibMyJson STATIC ${SRC})