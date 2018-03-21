#ifndef PARALLEL_NODE_H
#define PARALLEL_NODE_H

#include "control_node.h"

namespace BT {

class ParallelNode : public ControlNode {
public:
    explicit ParallelNode(std::string name, int threshold_M);
    ~ParallelNode() override = default;

    int DrawType() override;

    // The method that is going to be executed by the thread
    ReturnStatus Tick() override;

    void Halt() override;

    unsigned int get_threshold_M();

    void set_threshold_M(unsigned int threshold_M);

private:
    unsigned int threshold_M_;
    unsigned int success_childred_num_;
    unsigned int failure_childred_num_;
};

}

#endif // PARALLEL_NODE_H
