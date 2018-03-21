#include "action_node.h"
#include <string>

#pragma clang diagnostic ignored "-Wmissing-noreturn"

namespace BT {

ActionNode::ActionNode(std::string name)
        : LeafNode(std::move(name)) {
    type_ = ACTION_NODE;
    node_thread = std::thread(&ActionNode::WaitForTick, this);
}

void ActionNode::WaitForTick() {
    while (true) {
        DLOG(INFO) << get_name() << " wait for tick";
        tick_engine.Wait();
        DLOG(INFO) << get_name() << " tick received";

        set_status(RUNNING);
        ReturnStatus status = Tick();
        set_status(status);
    }
}

int ActionNode::DrawType() {
    return ACTION;
}

}