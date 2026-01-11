#ifndef GDEXAMPLE1_REGISTER_TYPES_H_
#define GDEXAMPLE1_REGISTER_TYPES_H_
#include <godot_cpp/godot.hpp>

namespace godot {
void LoadModule(ModuleInitializationLevel p_level);
void UnloadModule(ModuleInitializationLevel p_level);

} // namespace godot

#endif