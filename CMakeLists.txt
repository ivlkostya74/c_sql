cmake_minimum_required(VERSION 3.20)

project(c_sqlhw)

set(PostgresSQL_ADDITIONAL_VERSION "16")

add_executable(c_sqlhw "main.cpp")

add_subdirectory("C:\\Users\\aser\\source\\repos\\libpqxx-7.9.1" pqxx-build) 

target_compile_features(c_sqlhw PRIVATE cxx_std_17) 

target_link_libraries(c_sqlhw PRIVATE pqxx) 
