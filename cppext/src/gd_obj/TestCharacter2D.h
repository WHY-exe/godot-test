#pragma once
#include "BasicGameNode.h"
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/timer.hpp>

namespace godot {
class TestCharacter2D : public BasicGameNode {
    GDCLASS(TestCharacter2D, BasicGameNode)
private:
    // NOTE: GDCLASS mustbe initialize with memnew
    // it seems that the raw ptr is auto freed after scene is destroyed?
    Timer*    _timer;
    Object*   _test_obj;
    NodePath  _test_node_path;
    Sprite2D* _player_img;
    double    _speed;
    double    _angular_speed;

protected:
    static void _bind_methods();
    void        _on_timer_timeout();

public:
    TestCharacter2D();
    // call the process ready/exit_tree/process .etc here
    // before script call the overrided function
    void Ready() override final;
    void Process(double delta) override final;
    void ExitTree() override final;
    // getter/setter for speed
    void   set_speed(double speed);
    double get_speed() const;
    // getter/setter for angular speed
    void   set_angular_speed(double speed);
    double get_angular_speed() const;
    // getter/setter for test obj
    godot::Object* get_test_obj();
    void           set_test_obj(godot::Object* obj_in);
    // getter/setter for node_path
    godot::NodePath get_test_node();
    void            set_test_node(const godot::NodePath& node_in);
    void            test(double delta);
};
}  // namespace godot