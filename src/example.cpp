#include <iostream>

#include <glog/logging.h>

#include "behavior_tree.h"

// my condition
class MyCondition : public BT::ConditionNode {
public:
    explicit MyCondition(std::string name);
    ~MyCondition() override = default;

    BT::ReturnStatus Tick() override;
};

MyCondition::MyCondition(std::string name)
        : BT::ConditionNode::ConditionNode(std::move(name)) {
    set_status(BT::SUCCESS);
}

BT::ReturnStatus MyCondition::Tick() {
    return BT::SUCCESS;
}

// my action
class MyAction : public BT::ActionNode {
public:
    explicit MyAction(std::string name);
    ~MyAction() override = default;

    BT::ReturnStatus Tick() override;
    void Halt() override;
};

MyAction::MyAction(std::string name)
        : ActionNode::ActionNode(std::move(name)) {}

BT::ReturnStatus MyAction::Tick() {
    LOG(INFO) << get_name() << " Tick()";

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    if (is_halted()) {
        return BT::HALTED;
    }

    LOG(INFO) << get_name() << " has succeeded";
    return BT::SUCCESS;
}

void MyAction::Halt() {
    LOG(INFO) << get_name() << " Halt()";
}

// Init glog
void GlogInit(char *argv[]) {
    // glog init
    google::InitGoogleLogging(argv[0]);
    google::SetLogDestination(google::GLOG_INFO, "./glog-");
    FLAGS_alsologtostderr = true;
    FLAGS_logbuflevel = -1;
}

int main(int argc, char *argv[]) {
    GlogInit(argv);

    BT::FallbackNode *root = new BT::FallbackNode("Root");

    BT::SequenceNode *seq_1 = new BT::SequenceNode("Sequence_1");
    MyCondition *my_con_1 = new MyCondition("Condition_1");
    MyAction *my_act_1 = new MyAction("Action_1");
    MyAction *my_act_2 = new MyAction("Action_2");
    MyAction *my_act_3 = new MyAction("Action_3");

    root->AddChild(seq_1);

    seq_1->AddChild(my_con_1);
    seq_1->AddChild(my_act_1);
    seq_1->AddChild(my_act_2);
    seq_1->AddChild(my_act_3);

    int tick_ms = 1000;
    BT::Execute(root, tick_ms);

    return 0;
}
