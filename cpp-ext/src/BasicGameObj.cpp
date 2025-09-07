#include "BasicGameObj.h"
#include <godot_cpp/classes/engine.hpp>

namespace godot {
void BasicGameObj::_notification(int64_t p_what) {
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

void BasicGameObj::Ready() {
}

void BasicGameObj::Process(double /* delta */) {
}

void BasicGameObj::ExitTree() {
}
}  // namespace godot