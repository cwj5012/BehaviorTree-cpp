#include "behavior_tree.h"

#include <glog/logging.h>

#pragma clang diagnostic ignored "-Wmissing-noreturn"

namespace BT {

void Execute(ControlNode *root, int tick_ms) {
    LOG(INFO) << "=== Start Execute ===";
    LOG(INFO) << "=== Tick " << tick_ms << " Ms ===";

    // Start a new thread to drawing the bt
    std::thread thr_draw(&drawTree, root);

    root->ResetColorState();

    while (true) {
        DLOG(INFO) << "=== Ticking Root Node ""===";

        root->Tick();

        if (root->get_status() != RUNNING) {
            // when the root returns a status it resets the colors of the tree
            root->ResetColorState();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(tick_ms));
    }
}

}
