#include "sequence_node_with_memory.h"
#include <string>

namespace BT {

SequenceNodeWithMemory::SequenceNodeWithMemory(std::string name)
        : ControlNode::ControlNode(
        name) {
    reset_policy_ = ON_SUCCESS_OR_FAILURE;
    current_child_idx_ = 0;  // initialize the current running child
}


SequenceNodeWithMemory::SequenceNodeWithMemory(std::string name,
                                                   int reset_policy)
        : ControlNode::ControlNode(name) {
    reset_policy_ = reset_policy;
    current_child_idx_ = 0;  // initialize the current running child
}


ReturnStatus SequenceNodeWithMemory::Tick() {
    DLOG(INFO) << get_name() << " ticked, memory counter: " << current_child_idx_;

    // Vector size initialization. children_count_ could change at runtime if you edit the tree
    children_count_ = children_nodes_.size();

    // Routing the ticks according to the sequence node's (with memory) logic:
    while (current_child_idx_ < children_count_) {
        /*      Ticking an action is different from ticking a condition. An action executed some portion of code in another thread.
                We want this thread detached so we can cancel its execution (when the action no longer receive ticks).
                Hence we cannot just call the method Tick() from the action as doing so will block the execution of the tree.
                For this reason if a child of this node is an action, then we send the tick using the tick engine. Otherwise we call the method Tick() and wait for the response.
        */

        if (children_nodes_[current_child_idx_]->get_type() ==
            ACTION_NODE) {
            // 1) If the child i is an action, read its state.
            // Action nodes runs in another thread, hence you cannot retrieve the status just by executing it.

            child_status_ = children_nodes_[current_child_idx_]->get_status();
            DEBUG_STDOUT(get_name() << " It is an action "
                                    << children_nodes_[current_child_idx_]->get_name()
                                    << " with status: " << child_status_);

            if (child_status_ == IDLE || child_status_ == HALTED) {
                // 1.1) If the action status is not running, the sequence node sends a tick to it.
                DEBUG_STDOUT(get_name() << "NEEDS TO TICK "
                                        << children_nodes_[current_child_idx_]->get_name());
                children_nodes_[current_child_idx_]->tick_engine.Tick();

                // waits for the tick to arrive to the child
                do {
                    child_status_ = children_nodes_[current_child_idx_]->get_status();
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                } while (child_status_ != RUNNING &&
                         child_status_ != SUCCESS
                         && child_status_ != FAILURE);
            }
        } else {
            // 2) if it's not an action:
            // Send the tick and wait for the response;
            child_status_ = children_nodes_[current_child_idx_]->Tick();
            children_nodes_[current_child_idx_]->set_status(child_status_);

        }

        if (child_status_ == SUCCESS || child_status_ == FAILURE) {
            // the child goes in idle if it has returned success or failure.
            children_nodes_[current_child_idx_]->set_status(IDLE);
        }

        if (child_status_ != SUCCESS) {
            // If the  child status is not success, return the status
            DEBUG_STDOUT("the status of: " << get_name() << " becomes "
                                           << child_status_);
            if (child_status_ == FAILURE && (reset_policy_ == ON_FAILURE
                                                 || reset_policy_ ==
                                                    ON_SUCCESS_OR_FAILURE)) {
                current_child_idx_ = 0;
            }
            set_status(child_status_);
            return child_status_;
        } else if (current_child_idx_ != children_count_ - 1) {
            // If the  child status is success, continue to the next child
            // (if any, hence if(current_child_ != children_count_ - 1) ) in the for loop (if any).
            current_child_idx_++;
        } else {
            // if it the last child.
            if (child_status_ == SUCCESS) {
                // if it the last child and it has returned SUCCESS, reset the memory
                current_child_idx_ = 0;
            }
            set_status(child_status_);
            return child_status_;
        }
    }
    return EXIT;
}


int SequenceNodeWithMemory::DrawType() {
    return SEQUENCESTAR;
}


void SequenceNodeWithMemory::Halt() {
    current_child_idx_ = 0;
    ControlNode::Halt();
}

}