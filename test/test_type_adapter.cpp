// Copyright 2024 OUXT Polaris. All rights reserved.
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

#include <gtest/gtest.h>

#include <functional>
#include <grid_map_type_adapter/type_adapter.hpp>

class PubNode : public rclcpp::Node
{
public:
  using AdaptedType = rclcpp::TypeAdapter<grid_map::GridMap, grid_map_msgs::msg::GridMap>;
  explicit PubNode(const rclcpp::NodeOptions & options) : Node("pub", options)
  {
    publisher_ = create_publisher<AdaptedType>("grid_map", 1);
  }
  void publish(const grid_map::GridMap & grid_map) { publisher_->publish(grid_map); }

private:
  std::shared_ptr<rclcpp::Publisher<AdaptedType>> publisher_;
};

class SubNode : public rclcpp::Node
{
public:
  using AdaptedType = rclcpp::TypeAdapter<grid_map::GridMap, grid_map_msgs::msg::GridMap>;
  explicit SubNode(
    const rclcpp::NodeOptions & options,
    const std::function<void(const grid_map::GridMap &)> function)
  : Node("sub", options)
  {
    subscriber_ = create_subscription<AdaptedType>("grid_map", 1, function);
  }

private:
  std::shared_ptr<rclcpp::Subscription<AdaptedType>> subscriber_;
};

TEST(PubSub, typeAdaptation)
{
  bool recieved = false;
  rclcpp::init(0, nullptr);
  rclcpp::NodeOptions options;
  options.use_intra_process_comms(true);
  auto pub_grid_map = grid_map::GridMap({"base"});
  pub_grid_map.setFrameId("map");
  auto sub_node =
    std::make_shared<SubNode>(options, [&](const grid_map::GridMap &) { recieved = true; });
  auto pub_node = std::make_shared<PubNode>(options);
  pub_node->publish(pub_grid_map);
  rclcpp::executors::SingleThreadedExecutor exec;
  exec.add_node(sub_node);
  exec.add_node(pub_node);
  exec.spin_some();
  rclcpp::shutdown();
  EXPECT_TRUE(recieved);
}

int main(int argc, char ** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
