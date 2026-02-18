#include "maze_generator/maze_service.hpp"
#include "maze_generator/aldous_broder.hpp"
#include "maze_generator/binary_tree.hpp"
#include "maze_generator/grid.hpp"
#include "maze_generator/hunt_and_kill.hpp"
#include "maze_generator/recursive_backtracker.hpp"
#include "maze_generator/side_winder.hpp"
#include "maze_generator/wilsons.hpp"
#include <cstdint>
#include <maze_interfaces/srv/detail/generate_maze__struct.hpp>
#include <memory>
#include <rclcpp/executors.hpp>
#include <rclcpp/logging.hpp>
#include <rclcpp/utilities.hpp>
#include <vector>

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
  RCLCPP_INFO(this->get_logger(), "Using algorithm %s",
              request->algorithm.c_str());
  const auto &algorithm = request->algorithm;
  Grid grid(request->rows, request->columns);
  if (algorithm == "binary_tree") {
    BinaryTree::on(grid);
  } else if (algorithm == "side_winder") {
    SideWinder::on(grid);
  } else if (algorithm == "aldous_broder") {
    AldousBroder::on(grid);
  } else if (algorithm == "wilsons") {
    Wilsons::on(grid);
  } else if (algorithm == "hunt_and_kill") {
    HuntAndKill::on(grid);
  } else if (algorithm == "recursive_backtracker") {
    RecursiveBacktracker::on(grid);
  } else {
    RCLCPP_ERROR(this->get_logger(), "Cannot parse algorithm %s",
                 request->algorithm.c_str());
    response->success = false;
    response->error_message = "Failed to generate maze due to bad request";
    response->walls = std::vector<int32_t>();
  }

  RCLCPP_INFO_STREAM(this->get_logger(), grid.to_string());
  // Grid binary_grid(request->rows, request->columns);
  // BinaryTree::on(binary_grid);
  // RCLCPP_INFO(this->get_logger(), "Binary tree Maze generated");
  // RCLCPP_INFO_STREAM(this->get_logger(), binary_grid.to_string());
  //
  // Grid side_winder_grid(request->rows, request->columns);
  // SideWinder::on(side_winder_grid);
  // RCLCPP_INFO(this->get_logger(), "side_winder Maze generated");
  // RCLCPP_INFO_STREAM(this->get_logger(), side_winder_grid.to_string());
  //
  // Grid aldous_broder_grid(request->rows, request->columns);
  // AldousBroder::on(aldous_broder_grid);
  // RCLCPP_INFO(this->get_logger(), "Aldous Broder Maze generated");
  // RCLCPP_INFO_STREAM(this->get_logger(), aldous_broder_grid.to_string());
  //
  // Grid wilsons_grid(request->rows, request->columns);
  // Wilsons::on(wilsons_grid);
  // RCLCPP_INFO(this->get_logger(), "Wilsons Maze generated");
  // RCLCPP_INFO_STREAM(this->get_logger(), wilsons_grid.to_string());
  //
  // Grid hunt_and_kill_grid(request->rows, request->columns);
  // HuntAndKill::on(hunt_and_kill_grid);
  // RCLCPP_INFO(this->get_logger(), "Hunt and Kill Maze generated");
  // RCLCPP_INFO_STREAM(this->get_logger(), hunt_and_kill_grid.to_string());
  //
  // Grid recursive_backtracker_grid(request->rows, request->columns);
  // RecursiveBacktracker::on(recursive_backtracker_grid);
  // RCLCPP_INFO(this->get_logger(), "Recursive Backtracker Maze generated");
  // RCLCPP_INFO_STREAM(this->get_logger(),
  //                    recursive_backtracker_grid.to_string());
  response->success = true;
  response->walls = grid.to_wall_bitmask();
  response->error_message = "";
}

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MazeService>());
  rclcpp::shutdown();
  return 0;
}
