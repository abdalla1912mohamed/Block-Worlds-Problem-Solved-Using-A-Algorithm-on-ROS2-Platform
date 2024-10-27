cmake_minimum_required(VERSION 3.5)
project(task_subscriber)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

# Add executable target
add_executable(task_subscriber src/task_subscriber.cpp)
ament_target_dependencies(task_subscriber rclcpp std_msgs)

# Install executable
install(TARGETS task_subscriber
  DESTINATION lib/${PROJECT_NAME})

# Install package.xml
ament_package()

