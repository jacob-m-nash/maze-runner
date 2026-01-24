cmake_minimum_required(VERSION 3.8)
project(maze_generator)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(maze_interfaces REQUIRED)
include_directories(include)
add_library(maze_generator src/grid.cpp src/cell.cpp src/binary_tree.cpp
            src/rand_num_gen.cpp)
target_include_directories(maze_generator PUBLIC include)
ament_target_dependencies(maze_generator rclcpp maze_interfaces)
add_executable(maze_service src/maze_service.cpp)
target_link_libraries(maze_service maze_generator)
install(TARGETS maze_service DESTINATION lib/${PROJECT_NAME})
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
