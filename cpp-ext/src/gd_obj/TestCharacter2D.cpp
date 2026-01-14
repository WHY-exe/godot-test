#include "TestCharacter2D.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>
#include "log/Log.h"

namespace godot {
TestCharacter2D::TestCharacter2D() :
		_timer(nullptr),
		_test_obj(nullptr),
		_test_node_path(),
		_player_img(nullptr),
		_speed(400.0),
		_angular_speed(Math_PI) {
	// UtilityFunctions::print("inside TestCharacter2D constructor");
}

void TestCharacter2D::Ready() {
	LOGI("initialize godot object");
	test(0.0);
	// initialze timer using memnew
	_timer = memnew(Timer());
	_timer->set_autostart(true);
	_timer->connect("timeout", Callable(this, "_on_timer_timeout"));
	add_child(_timer);
	_timer->set_owner(this);
	// initialize player image through node path that can be edit on godot editor
	if (!_test_node_path.is_empty() && _player_img == nullptr) {
		_player_img = get_node<Sprite2D>(_test_node_path);
		if (!_player_img) {
			LOGE("fail to convert node:{} to sprite2D", _test_node_path.get_concatenated_names());
		} else {
			SPDLOG_DEBUG("image node:{} converted to sprite2D", _test_node_path.get_concatenated_names());
		}
	}
}

void TestCharacter2D::ExitTree() {
	if (_timer) {
		LOGI("free godot object");
		memdelete(_timer);
		_timer = nullptr;
	}
	if (_player_img) {
		_player_img = nullptr;
	}
}

void TestCharacter2D::Process(double delta) {
	if (!_player_img) {
		return;
	}
	int direction = 0;
	Input *input_singleton = Input::get_singleton();
	if (input_singleton->is_action_pressed("ui_left")) {
		direction = -1;
	} else if (input_singleton->is_action_pressed("ui_right")) {
		direction = 1;
	}

	_player_img->rotate(static_cast<float>(direction * _angular_speed * delta));
	if (input_singleton->is_action_pressed("ui_up")) {
		const float speed_translate = static_cast<float>(_speed * delta);
		_player_img->translate(Vector2(0, -1).rotated(_player_img->get_rotation()) * speed_translate);
	}
}

void TestCharacter2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("test", "delta"), &TestCharacter2D::test);
	// binding timer callback to make callback work
	ClassDB::bind_method(D_METHOD("_on_timer_timeout"),
			&TestCharacter2D::_on_timer_timeout);
	// binding angular speed variable to engine
	ClassDB::bind_method(D_METHOD("get_angular_speed"),
			&TestCharacter2D::get_angular_speed);
	ClassDB::bind_method(D_METHOD("set_angular_speed", "speed"),
			&TestCharacter2D::set_angular_speed);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "angular_speed", PROPERTY_HINT_RANGE, "0,1000,0.001"),
			"set_angular_speed",
			"get_angular_speed");
	// binding speed variable to engine
	ClassDB::bind_method(D_METHOD("get_speed"), &TestCharacter2D::get_speed);
	ClassDB::bind_method(D_METHOD("set_speed", "speed"),
			&TestCharacter2D::set_speed);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed", PROPERTY_HINT_RANGE, "0,1000,0.1"),
			"set_speed",
			"get_speed");
	// binding godot object to editor
	ClassDB::bind_method(D_METHOD("get_test_obj"), &TestCharacter2D::get_test_obj);
	ClassDB::bind_method(D_METHOD("set_test_obj", "obj_in"),
			&TestCharacter2D::set_test_obj);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "test_obj", PROPERTY_HINT_OBJECT_ID),
			"set_test_obj",
			"get_test_obj");
	// binding godot nodes to editor
	ClassDB::bind_method(D_METHOD("get_test_node"), &TestCharacter2D::get_test_node);
	ClassDB::bind_method(D_METHOD("set_test_node", "node_in"),
			&TestCharacter2D::set_test_node);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "test_node"),
			"set_test_node",
			"get_test_node");
}

void TestCharacter2D::_on_timer_timeout() {
	if (_player_img) {
		_player_img->set_visible(!_player_img->is_visible());
	}
}

double TestCharacter2D::get_speed() const {
	return _speed;
}

void TestCharacter2D::set_speed(double speed) {
	_speed = speed;
}

double TestCharacter2D::get_angular_speed() const {
	return _angular_speed;
}

void TestCharacter2D::set_angular_speed(double speed) {
	_angular_speed = speed;
}

Object *TestCharacter2D::get_test_obj() {
	return _test_obj;
}

void TestCharacter2D::set_test_obj(Object *obj_in) {
	_test_obj = obj_in;
}

NodePath TestCharacter2D::get_test_node() {
	return _test_node_path;
}
void TestCharacter2D::set_test_node(const NodePath &node_in) {
	_test_node_path = node_in;
}

void TestCharacter2D::test(double delta) {
	LOGD("this is a debug message {}", delta);
	LOGI("this is the test function: formatted string {}", delta);
	LOGW("this is a warning");
	LOGE("this is an error");
}
}  // namespace godot