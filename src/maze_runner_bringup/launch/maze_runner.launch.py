import os

from launch import LaunchDescription
from launch.actions import (
    DeclareLaunchArgument,
    IncludeLaunchDescription,
    SetEnvironmentVariable,
)
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import Command, LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    algorithm_arg = DeclareLaunchArgument(
        "algorithm", default_value="recursive_backtracker"
    )
    rows_arg = DeclareLaunchArgument("rows", default_value="10")
    cols_arg = DeclareLaunchArgument("cols", default_value="10")
    wall_length_arg = DeclareLaunchArgument("wall_length", default_value="2")
    wall_height_arg = DeclareLaunchArgument("wall_height", default_value="1")
    wall_thickness_arg = DeclareLaunchArgument("wall_thickness", default_value="0.1")
    spot_spawn_x_arg = DeclareLaunchArgument("spot_spawn_x", default_value="1")
    spot_spawn_y_arg = DeclareLaunchArgument("spot_spawn_y", default_value="1")
    spot_spawn_z_arg = DeclareLaunchArgument("spot_spawn_z", default_value="0.6")

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

    spot_urdf = os.path.join(
        FindPackageShare("spot_description").find("spot_description"),
        "urdf",
        "spot.urdf.xacro",
    )
    ign_resource_path = SetEnvironmentVariable(
        name="IGN_GAZEBO_RESOURCE_PATH",
        value=os.path.join(
            FindPackageShare("spot_description").find("spot_description"),
            "..",  # share/spot_description -> share/
        ),
    )

    robot_state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[{"robot_description": Command(["xacro ", spot_urdf])}],
    )
    spawn_spot = Node(
        package="ros_gz_sim",
        executable="create",
        arguments=[
            "-name",
            "spot",
            "-topic",
            "robot_description",
            "-x",
            LaunchConfiguration("spot_spawn_x"),
            "-y",
            LaunchConfiguration("spot_spawn_y"),
            "-z",
            LaunchConfiguration("spot_spawn_z"),
        ],
        output="screen",
    )
    return LaunchDescription(
        [
            ign_resource_path,
            algorithm_arg,
            rows_arg,
            cols_arg,
            wall_length_arg,
            wall_height_arg,
            wall_thickness_arg,
            spot_spawn_x_arg,
            spot_spawn_y_arg,
            spot_spawn_z_arg,
            gz_sim,
            gz_bridge,
            maze_generator,
            maze_builder,
            build_maze_client,
            robot_state_publisher,
            spawn_spot,
        ]
    )
