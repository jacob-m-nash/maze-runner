from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.events import IncludeLaunchDescription
from launch.actions import IncludeLaunchDescription, ExecuteProcess
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    algorithum_arg = DeclareLaunchArgument(
        "algorithum", default_value="recursive_backtracker"
    )
    rows_arg = DeclareLaunchArgument("rows", default_value="10")
    cols_arg = DeclareLaunchArgument("cols", default_value="10")
    wall_size_arg = DeclareLaunchArgument("wall_size", default_value="1")

    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            [
                PathJoinSubstitution(
                    [FindPackageShare("gazebo_ros"), "launch", "gazebo.launch.py"]
                )
            ]
        ),
        launch_arguments={"world": ""}.items(),
    )

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

    build_maze_client = Node(
        package="maze_builder",
        executable="build_maze_client",
        name="build_maze_client",
        output="screen",
        parameters=[
            {
                "rows": LaunchConfiguration("rows"),
                "columns": LaunchConfiguration("cols"),
                "wall_size": LaunchConfiguration("wall_size"),
                "algorithum": LaunchConfiguration("algorithum"),
            }
        ],
    )

    return LaunchDescription(
        [
            algorithum_arg,
            rows_arg,
            cols_arg,
            wall_size_arg,
            gazebo,
            maze_generator,
            maze_builder,
            build_maze_client,
        ]
    )
