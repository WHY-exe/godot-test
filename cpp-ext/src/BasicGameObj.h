#pragma once
#include <godot_cpp/classes/node.hpp>

namespace godot {
class BasicGameObj : public Node {
    GDCLASS(BasicGameObj, Node)
protected:
    static void _bind_methods(){};

protected:
    void         _notification(int64_t p_what);
    virtual void Ready();
    virtual void Process(double delta);
    virtual void ExitTree();
};
}  // namespace godot