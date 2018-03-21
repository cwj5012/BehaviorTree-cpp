#ifndef SEQUENCENODE_H
#define SEQUENCENODE_H

#include "control_node.h"

namespace BT {

/**
 * Sequence node base algorithm
 *
 * for i from 1 to n do
 *     childstatus ← Tick(child(i))
 *     if childstatus = running
 *        return running
 *     else if childstatus = failure
 *        return failure
 * end
 * return success
 */
class SequenceNode : public ControlNode {
public:
    explicit SequenceNode(std::string name);
    ~SequenceNode() override = default;

    int DrawType() override;

    ReturnStatus Tick() override;

private:
    // Deal node in the Tick() function
    void DealNodeType(unsigned int index);
    void WaitToTick(unsigned int index);
};

}

#endif
