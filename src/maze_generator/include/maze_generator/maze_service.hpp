#ifndef MAZE_SERVICE_HPP
#define MAZE_SERVICE_HPP
#include "maze_interfaces/srv/generate_maze.hpp"
#include <rclcpp/node.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/service.hpp>
class MazeService : public rclcpp::Node {
public:
  MazeService();

private:
  void generate_maze(
      const std::shared_ptr<maze_interfaces::srv::GenerateMaze::Request>
          request,
      std::shared_ptr<maze_interfaces::srv::GenerateMaze::Response> response);
  rclcpp::Service<maze_interfaces::srv::GenerateMaze>::SharedPtr service_;
};
#endif // !MAZE_SERVICE_HPP
