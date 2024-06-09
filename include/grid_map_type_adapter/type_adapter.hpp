// Copyright (c) 2024 OUXT Polaris
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef GRID_MAP_TYPE_ADAPTER__TYPE_ADAPTER_HPP_
#define GRID_MAP_TYPE_ADAPTER__TYPE_ADAPTER_HPP_

#include <grid_map_ros/GridMapRosConverter.hpp>
#include <rclcpp/rclcpp.hpp>

namespace grid_map_type_adapter
{
void toMessage(const grid_map::GridMap & gridMap, grid_map_msgs::msg::GridMap & message);
}

template <>
struct rclcpp::TypeAdapter<grid_map::GridMap, grid_map_msgs::msg::GridMap>
{
  using is_specialized = std::true_type;
  using custom_type = grid_map::GridMap;
  using ros_message_type = grid_map_msgs::msg::GridMap;

  static void convert_to_ros_message(const custom_type & source, ros_message_type & destination)
  {
    grid_map_type_adapter::toMessage(source, destination);
  }

  static void convert_to_custom(const ros_message_type & source, custom_type & destination)
  {
    if (!grid_map::GridMapRosConverter::fromMessage(source, destination)) {
      throw std::runtime_error("Failed to construct grid map from ROS 2 message.");
    }
  }
};

#endif  // GRID_MAP_TYPE_ADAPTER__TYPE_ADAPTER_HPP_
