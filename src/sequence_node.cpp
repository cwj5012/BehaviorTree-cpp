#include "sequence_node.h"

#include <string>

namespace BT {

SequenceNode::SequenceNode(std::string name)
        : ControlNode(std::move(name)) {}

ReturnStatus SequenceNode::Tick() {
    children_count_ = (unsigned int) children_nodes_.size();

    for (unsigned int i = 0; i < children_count_; i++) {
        DLOG(INFO) << get_name()
                   << " Get children " << children_nodes_[i]->get_name()
                   << " return status " << child_status_;

        DealNodeType(i);

        if (child_status_ != SUCCESS) {
            if (child_status_ == FAILURE) {
                children_nodes_[i]->set_status(IDLE);
            }
            DLOG(INFO) << get_name() << " is halting children index "
                       << (i + 1);
            HaltChildren(i + 1);
            set_status(child_status_);
            return child_status_;
        } else {
            // children_nodes_[i]->set_status(IDLE);
            if (i == children_count_ - 1) {
                for (unsigned int j = 0; j < children_count_; j++) {
                    if (children_nodes_[j]->get_type() == ACTION_NODE) {
                        children_nodes_[j]->set_status(IDLE);
                        DLOG(INFO) << children_nodes_[j]->get_name()
                                   << " set status " << IDLE;
                    }
                }
                set_status(SUCCESS);
                return SUCCESS;
            }
        }
    }

    return EXIT;
}

int SequenceNode::DrawType() {
    return SEQUENCE;
}

void SequenceNode::DealNodeType(unsigned int index) {
    if (children_nodes_[index]->get_type() == ACTION_NODE) {
        child_status_ = children_nodes_[index]->get_status();
        DLOG(INFO) << children_nodes_[index]->get_name()
                   << " status " << child_status_;
        if (child_status_ == IDLE || child_status_ == HALTED) {
            WaitToTick(index);
        }
    } else {
        LOG(INFO) << get_name() << " call "
                  << children_nodes_[index]->get_name() << " Tick()";
        child_status_ = children_nodes_[index]->Tick();
        LOG(INFO) << children_nodes_[index]->get_name()
                  << " status " << child_status_;
        children_nodes_[index]->set_status(child_status_);
    }
}

void SequenceNode::WaitToTick(unsigned int index) {
    DLOG(INFO) << get_name() << " need to tick "
               << children_nodes_[index]->get_name();
    children_nodes_[index]->tick_engine.Tick();
    DLOG(INFO) << get_name() << " tick "
               << children_nodes_[index]->get_name();
    do {
        child_status_ = children_nodes_[index]->get_status();
        // LOG(INFO) << children_nodes_[index]->get_name()
        //           << " waiting, status "
        //           << child_status_;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    } while (child_status_ != RUNNING
             && child_status_ != SUCCESS
             && child_status_ != FAILURE);
}

}