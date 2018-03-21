#ifndef LEAFNODE_H
#define LEAFNODE_H

#include <vector>

#include "tree_node.h"

namespace BT {

class LeafNode : public TreeNode {
public:
    explicit LeafNode(std::string name);
    ~LeafNode() override = default;

    void ResetColorState() override;

    int Depth() override;
};

}

#endif
