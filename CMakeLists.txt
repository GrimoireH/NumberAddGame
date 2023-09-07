project(NumAddGame)
add_executable(NumAddGame NumberAddGame.cpp)
target_include_directories(NumAddGame PUBLIC /usr/include/SFML)
target_link_libraries(NumAddGame sfml-system sfml-window sfml-graphics)