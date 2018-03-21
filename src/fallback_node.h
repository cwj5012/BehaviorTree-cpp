#ifndef FALLBACKNODE_H
#define FALLBACKNODE_H

#include "control_node.h"

namespace BT {

/**
 * Fallback node base algorithm
 *
 * for i from 1 to n do
 *     childstatus ← Tick(child(i))
 *     if childstatus = running
 *        return running
 *     else if childstatus = success
 *        return success
 * end
 * return failure
 *
 */
class FallbackNode : public ControlNode {
public:
    explicit FallbackNode(std::string name);
    ~FallbackNode() override = default;

    int DrawType() override;

    /**
     * @brief Tick the child node
     *
     * If the children is action node, try to do it until
     * the status is running or success or failure
     *
     * If the children is condition node or control node
     * call its Tick() method
     *
     * @return
     */
    ReturnStatus Tick() override ;
};

}

#endif
