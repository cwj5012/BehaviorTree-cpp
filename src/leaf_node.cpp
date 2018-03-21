#include <string>
#include "leaf_node.h"

namespace BT {

LeafNode::LeafNode(std::string name)
        : TreeNode(std::move(name)) {}

void LeafNode::ResetColorState() {
    color_status_ = IDLE;
}

int LeafNode::Depth() {
    return 0;
}

}