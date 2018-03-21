#ifndef CONTROLNODE_H
#define CONTROLNODE_H


#include <vector>

#include "tree_node.h"

namespace BT {

class ControlNode : public TreeNode {
public:
    explicit ControlNode(std::string name);
    ~ControlNode() override = default;

    // The method used to fill the child vector
    void AddChild(TreeNode *child);

    // The method used to know the number of children
    unsigned int GetChildrenNumber();
    std::vector<TreeNode *> GetChildren() const;

    // The method used to interrupt the execution of the node
    void Halt() override;

    void ResetColorState() override;

    int Depth() override;

    void HaltChildren(int i);

    // Methods used to access the node state without the
    // conditional waiting (only mutual access)
    bool WriteState(ReturnStatus new_state);

protected:
    std::vector<TreeNode *> children_nodes_;
    std::vector<ReturnStatus> children_states_;

    unsigned int children_count_;

    //child i status. Used to rout the ticks
    ReturnStatus child_status_;

};

}

#endif
