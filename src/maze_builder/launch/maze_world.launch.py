from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.events import IncludeLaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    algorithm_arg = DeclareLaunchArgument(
        "algorithm", default_value="recursive_backtracker"
    )
    rows_arg = DeclareLaunchArgument("rows", default_value="10")
    cols_arg = DeclareLaunchArgument("cols", default_value="10")
    wall_length_arg = DeclareLaunchArgument("wall_length", default_value="1")
    wall_height_arg = DeclareLaunchArgument("wall_height", default_value="1")
    wall_thickness_arg = DeclareLaunchArgument("wall_thickness", default_value="0.1")

    maze_generator = Node(
        package="maze_generator",
        executable="maze_service",
        name="maze_generator",
        output="screen",
    )
    maze_builder = Node(
        package="maze_builder",
        executable="maze_builder",
        name="maze_builder",
        output="screen",
    )

    gz_sim = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution(
                [FindPackageShare("ros_gz_sim"), "launch", "gz_sim.launch.py"]
            )
        ),
        launch_arguments={"gz_args": "-r empty.sdf"}.items(),
    )
    gz_bridge = Node(
        package="ros_gz_bridge",
        executable="parameter_bridge",
        arguments=["/world/empty/create@ros_gz_interfaces/srv/SpawnEntity"],
        output="screen",
    )
    build_maze_client = Node(
        package="maze_builder",
        executable="build_maze_client",
        name="build_maze_client",
        output="screen",
        parameters=[
            {
                "rows": LaunchConfiguration("rows"),
                "columns": LaunchConfiguration("cols"),
                "wall_length": LaunchConfiguration("wall_length"),
                "wall_height": LaunchConfiguration("wall_height"),
                "wall_thickness": LaunchConfiguration("wall_thickness"),
                "algorithm": LaunchConfiguration("algorithm"),
            }
        ],
    )

    return LaunchDescription(
        [
            algorithm_arg,
            rows_arg,
            cols_arg,
            wall_length_arg,
            wall_height_arg,
            wall_thickness_arg,
            gz_sim,
            gz_bridge,
            maze_generator,
            maze_builder,
            build_maze_client,
        ]
    )
