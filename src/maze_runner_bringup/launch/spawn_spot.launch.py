from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, SetEnvironmentVariable
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import Command, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
import os


def generate_launch_description():
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
    robot_state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[{"robot_description": Command(["xacro ", spot_urdf])}],
    )
    spawn_spot = Node(
        package="ros_gz_sim",
        executable="create",
        arguments=["-name", "spot", "-topic", "robot_description", "-z", "0.6"],
        output="screen",
    )
    return LaunchDescription(
        [ign_resource_path, gz_sim, robot_state_publisher, spawn_spot, gz_bridge]
    )
