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

#include <grid_map_ros/GridMapMsgHelpers.hpp>
#include <grid_map_type_adapter/type_adapter.hpp>

namespace grid_map_type_adapter
{
void toMessage(const grid_map::GridMap & gridMap, grid_map_msgs::msg::GridMap & message)
{
  message.header.stamp = rclcpp::Time(gridMap.getTimestamp());
  message.header.frame_id = gridMap.getFrameId();
  message.info.resolution = gridMap.getResolution();
  message.info.length_x = gridMap.getLength().x();
  message.info.length_y = gridMap.getLength().y();
  message.info.pose.position.x = gridMap.getPosition().x();
  message.info.pose.position.y = gridMap.getPosition().y();
  message.info.pose.position.z = 0.0;
  message.info.pose.orientation.x = 0.0;
  message.info.pose.orientation.y = 0.0;
  message.info.pose.orientation.z = 0.0;
  message.info.pose.orientation.w = 1.0;

  message.layers = gridMap.getLayers();
  message.basic_layers = gridMap.getBasicLayers();

  message.data.clear();
  for (const auto & layer : message.layers) {
    std_msgs::msg::Float32MultiArray dataArray;
    grid_map::matrixEigenCopyToMultiArrayMessage(gridMap.get(layer), dataArray);
    message.data.push_back(dataArray);
  }

  message.outer_start_index = gridMap.getStartIndex()(0);
  message.inner_start_index = gridMap.getStartIndex()(1);
}
}  // namespace grid_map_type_adapter
