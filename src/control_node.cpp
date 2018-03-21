#include "control_node.h"
#include <string>
#include <vector>
#include <glog/logging.h>

namespace BT {

ControlNode::ControlNode(std::string name) : TreeNode::TreeNode(name) {
    type_ = CONTROL_NODE;

    // TODO(...) In case it is desired to set to idle remove the ReturnStatus
    // type in order to set the member variable
    // ReturnStatus child_status_ = IDLE;  // commented out as unused
}

void ControlNode::AddChild(TreeNode *child) {
//    // Checking if the child is not already present
//    for (unsigned int i=0; i<children_nodes_.size(); i++)
//    {
//        if (children_nodes_[i] == child)
//        {
//            throw BehaviorTreeException("'" + child->get_name() + "' is already a '" + get_name() + "' child.");
//        }
//    }

    children_nodes_.push_back(child);
    children_states_.push_back(IDLE);
}

unsigned int ControlNode::GetChildrenNumber() {
    return children_nodes_.size();
}

void ControlNode::Halt() {
    DEBUG_STDOUT("HALTING: " << get_name());
    HaltChildren(0);
    set_status(HALTED);
}

std::vector<TreeNode *> ControlNode::GetChildren() const {
    return children_nodes_;
}

void ControlNode::ResetColorState() {
    set_color_status(IDLE);
    for (auto item:children_nodes_) {
        item->ResetColorState();
    }
}

void ControlNode::HaltChildren(int i) {
    auto index = (unsigned int) i;
    DLOG(INFO) << "children count " << children_nodes_.size();
    for (; index < children_nodes_.size(); index++) {
        if (children_nodes_[index]->get_type() == CONDITION_NODE) {
            DLOG(INFO) << "reset color start "
                       << children_nodes_[index]->get_name();
            children_nodes_[i]->ResetColorState();
        } else {
            if (children_nodes_[index]->get_status() == RUNNING) {
                DLOG(INFO) << "sending halt to child "
                           << children_nodes_[index]->get_name();
                children_nodes_[index]->Halt();
            } else {
                DLOG(INFO) << "no need to halt "
                           << children_nodes_[index]->get_name()
                           << " status "
                           << children_nodes_[index]->get_status();
            }
        }
    }
}

int ControlNode::Depth() {
    int depMax = 0;
    int dep = 0;
    for (auto item:children_nodes_) {
        dep = item->Depth();
        if (dep > depMax) {
            depMax = dep;
        }
    }
    return 1 + depMax;
}

}