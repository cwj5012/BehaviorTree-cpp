#include "tree_node.h"

#include <string>

namespace BT {

TreeNode::TreeNode(std::string name)
        : tick_engine(0),
          is_state_updated_(false),
          name_(std::move(name)) {
    set_status(IDLE);
}

void TreeNode::set_status(ReturnStatus new_status) {
    if (new_status != IDLE) {
        set_color_status(new_status);
    }
    std::unique_lock<std::mutex> UniqueLock(state_mutex_);
    status_ = new_status;
}

ReturnStatus TreeNode::get_status() {
    std::lock_guard<std::mutex> LockGuard(state_mutex_);
    return status_;
}

ReturnStatus TreeNode::get_color_status() {
    std::lock_guard<std::mutex> LockGuard(color_state_mutex_);
    return color_status_;
}

void TreeNode::set_color_status(ReturnStatus new_color_status) {
    std::lock_guard<std::mutex> LockGuard(color_state_mutex_);
    color_status_ = new_color_status;
}

float TreeNode::get_x_pose() const {
    return x_pos_;
}

void TreeNode::set_x_pose(float x_pose) {
    x_pos_ = x_pose;
}

float TreeNode::get_x_shift() const {
    return x_shift_;
}

void TreeNode::set_x_shift(float x_shift) {
    x_shift_ = x_shift;
}

void TreeNode::set_name(std::string new_name) {
    name_ = std::move(new_name);
}

std::string TreeNode::get_name() const {
    return name_;
}

NodeType TreeNode::get_type() const {
    return type_;
}

bool TreeNode::is_halted() {
    return get_status() == HALTED;
}

}