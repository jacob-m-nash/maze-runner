from maze_interfaces.srv import BuildMaze
import rclpy
from rclpy.node import Node


class BuildMazeClient(Node):
    def __init__(self):
        super().__init__("BuildMazeClient")
        # Declare parameters
        self.declare_parameter("rows", 10)
        self.declare_parameter("columns", 10)
        self.declare_parameter("algorithm", "recursive_backtracker")

        self.declare_parameter("wall_length", 1)
        self.declare_parameter("wall_height", 1)
        self.declare_parameter("wall_thickness", 0.1)
        self.client = self.create_client(BuildMaze, "build_maze")

        # Wait for service, then call it
        self.get_logger().info("Waiting for build_maze service...")
        self.client.wait_for_service()
        self.send_request()

    def send_request(self):
        request = BuildMaze.Request()
        request.rows = self.get_parameter("rows").value
        request.columns = self.get_parameter("columns").value
        request.wall_length = self.get_parameter("wall_length").value
        request.wall_height = self.get_parameter("wall_height").value
        request.wall_thickness = self.get_parameter("wall_thickness").value
        request.algorithm = self.get_parameter("algorithm").value

        future = self.client.call_async(request)
        future.add_done_callback(self.response_callback)

    def response_callback(self, future):
        result = future.result()
        if result.success:
            self.get_logger().info("Maze built successfully!")
        else:
            self.get_logger().error(f"Failed: {result.error_message}")
        raise SystemExit  # Cleanly exit after job is done


def main(args=None):
    rclpy.init(args=args)
    node = BuildMazeClient()
    rclpy.spin(node)
    rclpy.shutdown()
