#include "parallel_node.h"

#include <string>

namespace BT {

ParallelNode::ParallelNode(std::string name, int threshold_M)
        : ControlNode(std::move(name)) {
    threshold_M_ = threshold_M;
}

ReturnStatus ParallelNode::Tick() {
    success_childred_num_ = 0;
    failure_childred_num_ = 0;

    children_count_ = (unsigned int) children_nodes_.size();

    for (unsigned int i = 0; i < children_count_; i++) {
        DEBUG_STDOUT(
                get_name() << "TICKING " << children_nodes_[i]->get_name());

        if (children_nodes_[i]->get_type() == ACTION_NODE) {
            child_status_ = children_nodes_[i]->get_status();
            if (child_status_ == IDLE || child_status_ == HALTED) {
                DEBUG_STDOUT(get_name() << "NEEDS TO TICK "
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
            child_status_ = children_nodes_[i]->Tick();
        }
        switch (child_status_) {
            case SUCCESS:
                children_nodes_[i]->set_status(
                        IDLE);  // the child goes in idle if it has returned success.
                if (++success_childred_num_ == threshold_M_) {
                    success_childred_num_ = 0;
                    failure_childred_num_ = 0;
                    HaltChildren(0);
                    set_status(child_status_);
                    return child_status_;
                }
                break;
            case FAILURE:
                children_nodes_[i]->set_status(IDLE);
                if (++failure_childred_num_ > children_count_ - threshold_M_) {
                    DEBUG_STDOUT(
                            "*******PARALLEL" << get_name()
                                              << " FAILED****** failure_childred_num_:"
                                              << failure_childred_num_);
                    success_childred_num_ = 0;
                    failure_childred_num_ = 0;
                    HaltChildren(0);
                    set_status(child_status_);
                    return child_status_;
                }
                break;
            case RUNNING:
                set_status(child_status_);
                break;
            default:
                break;
        }
    }
    return RUNNING;
}

void ParallelNode::Halt() {
    success_childred_num_ = 0;
    failure_childred_num_ = 0;
    ControlNode::Halt();
}

int ParallelNode::DrawType() {
    return PARALLEL;
}

unsigned int ParallelNode::get_threshold_M() {
    return threshold_M_;
}

void ParallelNode::set_threshold_M(unsigned int threshold_M) {
    threshold_M_ = threshold_M;
}

}