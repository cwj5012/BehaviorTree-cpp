#ifndef BEHAVIORTREECORE_TREENODE_H
#define BEHAVIORTREECORE_TREENODE_H


#include <iostream>

#include <string>

#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

#include "tick_engine.h"
#include "exceptions.h"

#include <glog/logging.h>

#define DEBUG

#ifdef DEBUG
#define DEBUG_STDOUT(str) do { LOG(INFO) << str; } while( false )
#else
#define DEBUG_STDOUT(str)
#endif

namespace BT {

/**
 * @brief Enumerates the possible types of a node, for drawing
 *        we have do discriminate whoich control node it is
 */
enum NodeType {
    ACTION_NODE,
    CONDITION_NODE,
    CONTROL_NODE
};

enum DrawNodeType {
    PARALLEL,
    SELECTOR,
    SEQUENCE,
    SEQUENCESTAR,
    SELECTORSTAR,
    ACTION,
    CONDITION,
    DECORATOR
};

/**
 * @brief Enumerates the states every node can be in after execution
 *        during a particular time step:
 *
 * Success indicates that the node has completed running during this time step;
 * Failure indicates that the node has determined it will not be able to complete its task;
 * Running indicates that the node has successfully moved forward during this  time step,
 *         but the task is not yet complete;
 * Idle    indicates that the node hasn't run yet.s
 * Halted  indicates that the node has been halted by its father.
 */
enum ReturnStatus {
    RUNNING,
    SUCCESS,
    FAILURE,
    IDLE,
    HALTED,
    EXIT
};

/**
 * @brief Enumerates the options for when a parallel node is considered to have failed
 *
 * FAIL_ON_ONE indicates that the node will return failure as soon as one of its children fails;
 * FAIL_ON_ALL indicates that all of the node's children must fail before it returns failure.
 */
enum FailurePolicy {
    FAIL_ON_ONE,
    FAIL_ON_ALL
};

enum ResetPolity {
    ON_SUCCESS_OR_FAILURE,
    ON_SUCCESS,
    ON_FAILURE
};

/**
 * @brief Enumerates the options for when a parallel node is considered to have succeeded
 *
 * SUCCEED_ON_ONE
 * indicates that the node will return success as soon as one of its children succeeds;
 *
 * SUCCEED_ON_ALL
 * indicates that all of the node's children must succeed before it returns success.
 *
 * If "BT::FAIL_ON_ONE" and "BT::SUCCEED_ON_ONE" are both active and are both trigerred in the
 * same time step, failure will take precedence.
 */
enum SuccessPolicy {
    SUCCEED_ON_ONE,
    SUCCEED_ON_ALL
};

// Abstract base class for Behavior Tree Nodes
class TreeNode {
public:
    explicit TreeNode(std::string name);

    virtual ~TreeNode() = default;

    /**
     * @brief The thread that will execute the node
     */
    std::thread node_thread;

    /**
     * @brief Node semaphore to simulate the tick (and to synchronize fathers and children)
     */
    TickEngine tick_engine;

    /**
     * @brief The method that is going to be executed when the node receive a tick
     * @return
     */
    virtual BT::ReturnStatus Tick() = 0;

    /**
     * @brief The method used to interrupt the execution of the node
     */
    virtual void Halt() = 0;

    /**
     * @brief Geit Drawtype
     */
    virtual int DrawType() = 0;

    virtual void ResetColorState() = 0;

    virtual int Depth() = 0;

    // The method that retrive the state of the node
    // (conditional waiting and mutual access)
    ReturnStatus GetNodeState();

    void SetNodeState(ReturnStatus new_state);

    // Methods used to access the node state without the
    // conditional waiting (only mutual access)
    ReturnStatus ReadState();

    bool is_halted();

    // Getters and setters
    void set_color_status(ReturnStatus new_color_status);
    ReturnStatus get_color_status();

    void set_status(ReturnStatus new_status);
    ReturnStatus get_status();

    void set_x_pose(float x_pose);
    float get_x_pose() const;

    void set_x_shift(float x_shift);
    float get_x_shift() const;

    std::string get_name() const;
    void set_name(std::string new_name);

    NodeType get_type() const;

protected:
    /**
     * @brief The node state that must be treated in a thread-safe way
     */
    bool is_state_updated_;
    ReturnStatus status_;
    ReturnStatus color_status_;

    std::mutex state_mutex_;
    std::mutex color_state_mutex_;
    std::condition_variable state_condition_variable_;

    /**
     * @brief Node type
     */
    NodeType type_;

    /**
     * @brief Position and offset for horizontal positioning when drawing
     */
    float x_shift_;
    float x_pos_;

private:
    /**
     * @brief Node name
     */
    std::string name_ = "";
};

}

#endif
