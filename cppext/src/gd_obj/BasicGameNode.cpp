#include "BasicGameNode.h"
#include <godot_cpp/classes/engine.hpp>

namespace godot {
void BasicGameNode::_notification(int64_t p_what) {
    // to make sure that the cpp function is called only when the game is running
    if (godot::Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    switch (p_what) {
    case NOTIFICATION_READY:
        Ready();
        set_process(true);
        break;
    case NOTIFICATION_PROCESS:
        Process(get_process_delta_time());
        break;
    case NOTIFICATION_EXIT_TREE:
        set_process(false);
        ExitTree();
        break;
    }
}

void BasicGameNode::Ready() {
}

void BasicGameNode::Process(double /* delta */) {
}

void BasicGameNode::ExitTree() {
}
}  // namespace godot