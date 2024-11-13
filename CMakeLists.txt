cmake_minimum_required(VERSION 3.29)
project(telnetServer)

# Set C++ standard to 20
set(CMAKE_CXX_STANDARD 20)

# Find Boost libraries (Asio and System are necessary)
find_package(Boost 1.70 REQUIRED COMPONENTS system)

# Add the executable target
add_executable(telnetServer main.cpp)

# Link Boost.System (which contains Boost.Asio) to the executable
target_link_libraries(telnetServer Boost::system)

# If you're using pthreads on Linux, you may need to link pthread as well
target_link_libraries(telnetServer pthread)

# Copy the ascii_star_wars.txt file to the build directory
add_custom_command(
        TARGET telnetServer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/ascii_star_wars.txt
        $<TARGET_FILE_DIR:telnetServer>/ascii_star_wars.txt
        COMMENT "Copying ascii_star_wars.txt to the output directory"
)