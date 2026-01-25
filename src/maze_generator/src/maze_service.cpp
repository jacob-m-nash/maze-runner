#include "maze_generator/maze_service.hpp"
#include "maze_generator/binary_tree.hpp"
#include "maze_generator/grid.hpp"
#include <maze_interfaces/srv/detail/generate_maze__struct.hpp>
#include <memory>
#include <rclcpp/executors.hpp>
#include <rclcpp/logging.hpp>
#include <rclcpp/utilities.hpp>

MazeService::MazeService() : Node("maze_service") {
  service_ = this->create_service<maze_interfaces::srv::GenerateMaze>(
      "generate_maze", std::bind(&MazeService::generate_maze, this,
                                 std::placeholders::_1, std::placeholders::_2));
  RCLCPP_INFO(this->get_logger(), "maze_service ready");
}

void MazeService::generate_maze(
    const std::shared_ptr<maze_interfaces::srv::GenerateMaze::Request> request,
    std::shared_ptr<maze_interfaces::srv::GenerateMaze::Response> response) {
  RCLCPP_INFO(this->get_logger(), "Generating %dx%d maze", request->rows,
              request->columns);
  Grid grid(request->rows, request->columns);
  BinaryTree::on(grid);
  RCLCPP_INFO(this->get_logger(), "Maze generated");
  RCLCPP_INFO_STREAM(this->get_logger(), grid.to_string());
  response->success = true;
}

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MazeService>());
  rclcpp::shutdown();
  return 0;
}
