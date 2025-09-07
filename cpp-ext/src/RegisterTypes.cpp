#include "RegisterTypes.h"
#include "Log.h"
#include "gdexample.h"
#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

namespace godot {
void initialize_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	ClassDB::register_class<BasicGameObj>();
	ClassDB::register_class<TestCharacter2D>();
	log::Config logConfig{};
	logConfig.logLevel      = log::level::debug;
	logConfig.enableConsole = true;
	logConfig.enableFile    = true;
	log::Init(logConfig);
}

void uninitialize_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

} // namespace godot

extern "C" {
GDExtensionBool GDE_EXPORT
example1_lib_init(GDExtensionInterfaceGetProcAddress p_get_proc_addr,
		const GDExtensionClassLibraryPtr p_lib,
		GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject ini_obj(p_get_proc_addr, p_lib, r_initialization);
	ini_obj.register_initializer(godot::initialize_module);
	ini_obj.register_terminator(godot::uninitialize_module);
	ini_obj.set_minimum_library_initialization_level(
			godot::MODULE_INITIALIZATION_LEVEL_SCENE);
	return ini_obj.init();
}
}