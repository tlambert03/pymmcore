import pymmcore._pymmdevice_swig as s

P = "/Users/talley/Library/Application Support/pymmcore-plus/mm/Micro-Manager-80d5ac1"
pluginManager_ = s.CPluginManager()
pluginManager_.SetSearchPaths([P])
print(pluginManager_)
print(pluginManager_.GetSearchPaths())

# module: 's.LoadedDeviceAdapter' = pluginManager_.GetDeviceAdapter("DemoCamera")

# print(module)
# print(module.GetName())
# module.Unload()


pluginManager_.UnloadPluginLibrary("DemoCamera")

# not yet working
# dev: 's.CMMDevice' = module.LoadDevice("DemoCamera", "MyCamera")