#ifndef CONDITIONNODE_H
#define CONDITIONNODE_H

#include "leaf_node.h"

namespace BT {

class ConditionNode : public LeafNode {
public:
    // Constructor
    explicit ConditionNode(std::string name);
    ~ConditionNode() override = default;

    // The method that is going to be executed by the thread
    BT::ReturnStatus Tick() override = 0;

    // The method used to interrupt the execution of the node
    void Halt() override;

    // Methods used to access the node state without the
    // conditional waiting (only mutual access)
    bool WriteState(ReturnStatus new_state);

    int DrawType() override;
};

}

#endif
