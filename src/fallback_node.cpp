#include "fallback_node.h"
#include <string>

namespace BT {

FallbackNode::FallbackNode(std::string name)
        : ControlNode::ControlNode(std::move(name)) {}

ReturnStatus FallbackNode::Tick() {
    children_count_ = (unsigned int) children_nodes_.size();

    for (unsigned int i = 0; i < children_count_; i++) {
        if (children_nodes_[i]->get_type() == ACTION_NODE) {
            // Child is action node, try do it
            child_status_ = children_nodes_[i]->get_status();
            if (child_status_ == IDLE || child_status_ == HALTED) {
                DEBUG_STDOUT(get_name() << "need to tick "
                                        << children_nodes_[i]->get_name());
                children_nodes_[i]->tick_engine.Tick();
                do {
                    child_status_ = children_nodes_[i]->get_status();
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                } while (child_status_ != RUNNING &&
                         child_status_ != SUCCESS &&
                         child_status_ != FAILURE);
            }
        } else {
            // Child is condition node or control node
            child_status_ = children_nodes_[i]->Tick();
            children_nodes_[i]->set_status(child_status_);
        }

        if (child_status_ != FAILURE) {
            if (child_status_ == SUCCESS) {
                children_nodes_[i]->set_status(IDLE);
            }
            // Halt next children
            DEBUG_STDOUT(get_name() << " is HALTING children from " << (i + 1));
            HaltChildren(i + 1);
            // set status and color status
            set_status(child_status_);
            return child_status_;
        } else {
            children_nodes_[i]->set_status(IDLE);
            if (i == children_count_ - 1) {
                // If the last children failure, then return
                set_status(FAILURE);
                return FAILURE;
            }
        }
    }

    return EXIT;
}

int FallbackNode::DrawType() {
    return SELECTOR;
}

}