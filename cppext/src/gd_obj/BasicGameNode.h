#pragma once
#include <godot_cpp/classes/node.hpp>

namespace godot {
class BasicGameNode : public Node {
    GDCLASS(BasicGameNode, Node)
protected:
    static void _bind_methods(){};

protected:
    void         _notification(int64_t p_what);
    virtual void Ready();
    virtual void Process(double delta);
    virtual void ExitTree();
};
}  // namespace godot