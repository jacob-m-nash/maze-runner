from maze_interfaces.srv import GenerateMaze, BuildMaze
from gazebo_msgs.srv import SpawnEntity
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
        self.spawn_client = self.create_client(
            SpawnEntity, "/spawn_entity", callback_group=self.callback_group
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
            await self.decode_walls(
                gen_response.walls, request.rows, request.columns, request.wall_size
            )
            response.success = True
            return response
        else:
            response.success = False
            response.error_message = (
                f"Failed to generate maze: {gen_response.error_message}"
            )
            return response

    async def decode_walls(self, walls, rows, columns, wall_size):
        NORTH, SOUTH, EAST, WEST = 1, 2, 4, 8
        wall_thikness = 0.1
        spawned_walls = set()
        for row in range(rows):
            for col in range(columns):
                mask = walls[row * columns + col]
                x = col * wall_size + wall_size / 2
                y = row * wall_size + wall_size / 2
                if mask & NORTH:
                    wall_key = ("h", row + 1, col)
                    if wall_key not in spawned_walls:
                        await self.spawn_wall(
                            x,
                            y + wall_size / 2,
                            wall_size,
                            wall_thikness,
                            f"wall_{row}_{col}_n",
                        )
                        spawned_walls.add(wall_key)

                if mask & SOUTH:
                    wall_key = ("h", row, col)
                    if wall_key not in spawned_walls:
                        await self.spawn_wall(
                            x,
                            y - wall_size / 2,
                            wall_size,
                            wall_thikness,
                            f"wall_{row}_{col}_s",
                        )
                        spawned_walls.add(wall_key)
                if mask & EAST:
                    wall_key = ("v", row, col + 1)
                    if wall_key not in spawned_walls:
                        await self.spawn_wall(
                            x + wall_size / 2,
                            y,
                            wall_thikness,
                            wall_size,
                            f"wall_{row}_{col}_e",
                        )
                        spawned_walls.add(wall_key)
                if mask & WEST:
                    wall_key = ("v", row, col)
                    if wall_key not in spawned_walls:
                        await self.spawn_wall(
                            x - wall_size / 2,
                            y,
                            wall_thikness,
                            wall_size,
                            f"wall_{row}_{col}_w",
                        )
                        spawned_walls.add(wall_key)
                # has_north_wall = bool(mask & NORTH)
                # has_south_wall = bool(mask & SOUTH)
                # has_east_wall = bool(mask & EAST)
                # has_west_wall = bool(mask & WEST)

    async def spawn_wall(self, x, y, length_x, length_y, name):
        if not self.spawn_client.wait_for_service(1.0):
            self.get_logger().error("spawn service not avalanble")
            return
        height = 0.5
        sdf = f"""<?xml version="1.0"?>
                    <sdf version="1.6">
                    <model name="{name}">
                        <static>true</static>
                        <link name="link">
                        <collision name="collision">
                            <geometry><box><size>{length_x} {length_y} {height}</size></box></geometry>
                        </collision>
                        <visual name="visual">
                            <geometry><box><size>{length_x} {length_y} {height}</size></box></geometry>
                        </visual>
                        </link>
                    </model>
                    </sdf>"""
        request = SpawnEntity.Request()
        request.name = name
        request.xml = sdf
        request.initial_pose.position.x = x
        request.initial_pose.position.y = y
        request.initial_pose.position.z = height / 2
        result = await self.spawn_client.call_async(request)
        self.get_logger().info(f"spawned {name}: {result.success}")


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
