import sys
from maze_interfaces.srv import GenerateMaze, BuildMaze
import rclpy
from rclpy.node import Node
from rclpy.callback_groups import ReentrantCallbackGroup
from rclpy.executors import MultiThreadedExecutor


class MazeBuilder(Node):
    def __init__(self):
        super().__init__("maze_builder")
        self.callback_group = ReentrantCallbackGroup()
        self.generate_client = self.create_client(
            GenerateMaze, "generate_maze", callback_group=self.callback_group
        )
        self.service = self.create_service(
            BuildMaze,
            "build_maze",
            self.build_maze_callback,
            callback_group=self.callback_group,
        )
        self.get_logger().info("maze_builder service ready")

    async def build_maze_callback(self, request, response):
        if not self.generate_client.wait_for_service(timeout_sec=5.0):
            response.success = False
            response.error_message = "generate_maze service unavalable"
            return response

        gen_request = GenerateMaze.Request()
        gen_request.algorithum = request.algorithum
        gen_request.rows = request.rows
        gen_request.columns = request.columns

        gen_response = await self.generate_client.call_async(gen_request)
        if gen_response.success:
            self.decode_walls(gen_response.walls, request.rows, request.columns)
            response.success = True
            return response
        else:
            response.success = False
            response.error_message = (
                f"Failed to generate maze: {gen_response.error_message}"
            )
            return response

    def decode_walls(self, walls, rows, columns):
        NORTH, SOUTH, EAST, WEST = 1, 2, 4, 8
        for row in range(rows):
            for col in range(columns):
                mask = walls[row * columns + col]
                has_north_wall = bool(mask & NORTH)
                has_south_wall = bool(mask & SOUTH)
                has_east_wall = bool(mask & EAST)
                has_west_wall = bool(mask & WEST)


def main(args=None):
    rclpy.init(args=args)
    node = MazeBuilder()
    executor = MultiThreadedExecutor()
    executor.add_node(node)
    try:
        executor.spin()
    finally:
        node.destroy_node()
        rclpy.shutdown()
