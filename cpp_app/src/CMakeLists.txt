cmake_minimum_required(VERSION 3.10)

project(ledprivateapp)

add_executable(ledprivateapp main.cpp)
install(TARGETS ledprivateapp DESTINATION bin)
target_link_libraries(ledprivateapp PUBLIC
    gpiod
)