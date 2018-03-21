#ifndef FALLBACK_NODE_WITH_MEMORY_H
#define FALLBACK_NODE_WITH_MEMORY_H


#include "control_node.h"

namespace BT {

class FallbackNodeWithMemory : public ControlNode {
public:
    explicit FallbackNodeWithMemory(std::string name);
    explicit FallbackNodeWithMemory(std::string name, int reset_policy);
    ~FallbackNodeWithMemory() override = default;

    int DrawType() override;

    // The method that is going to be executed by the thread
    ReturnStatus Tick() override;

    void Halt() override;

private:
    unsigned int current_child_idx_;
    unsigned int reset_policy_;
};

}


#endif // FALLBACK_NODE_WITH_MEMORY_H
