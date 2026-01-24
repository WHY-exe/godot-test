#include "RegisterTypes.h"
#include "log/Log.h"
#include "gd_obj/TestCharacter2D.h"
#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/godot.hpp>
#include <mutex>
#include "utils/DumpClient.h"

namespace godot {
void LoadModule(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
    static std::once_flag gOnceFlag;
    std::call_once(gOnceFlag, [] () {
        String dataPath = OS::get_singleton()->get_user_data_dir();
        dmphelper::Client::Instancitiate(
        #if defined (WIN32)
            dataPath.wide_string().get_data()
        #else
            dataPath.utf8().get_data()
        #endif
        );
    });
    ClassDB::register_class<BasicGameNode>();
    ClassDB::register_class<TestCharacter2D>();
    Log::Config logConfig{};
    logConfig.logLevel      = Log::level::debug;
    logConfig.enableConsole = true;
    logConfig.enableFile    = true;
    Log::Init(logConfig);
}

void UnloadModule(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}

}  // namespace godot

extern "C" {
GDExtensionBool GDE_EXPORT
example1_lib_init(GDExtensionInterfaceGetProcAddress p_get_proc_addr,
                  const GDExtensionClassLibraryPtr   p_lib,
                  GDExtensionInitialization*         r_initialization) {
    godot::GDExtensionBinding::InitObject ini_obj(p_get_proc_addr, p_lib, r_initialization);
    ini_obj.register_initializer(godot::LoadModule);
    ini_obj.register_terminator(godot::UnloadModule);
    ini_obj.set_minimum_library_initialization_level(godot::MODULE_INITIALIZATION_LEVEL_SCENE);
    return ini_obj.init();
}
}