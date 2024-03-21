#include "Logger.h"
#include "MMDevice.h"

class CMMCore;

typedef std::function<void(MM::Device*)> DeleteDeviceFunction;

class DeviceInstance {
 protected:
  MM::Device* pImpl_;

 private:
  CMMCore* core_;  // Weak reference
  std::shared_ptr<LoadedDeviceAdapter> adapter_;

 public:
  DeviceInstance(const DeviceInstance&) = delete;
  DeviceInstance& operator=(const DeviceInstance&) = delete;
  std::shared_ptr<LoadedDeviceAdapter> GetAdapterModule() const /* final */ { return adapter_; }
  std::vector<std::string> GetPropertyNames() const;
  std::string GetProperty(const std::string& name) const;
  void SetProperty(const std::string& name, const std::string& value) const;
  bool HasProperty(const std::string& name) const;

 protected:
  // The DeviceInstance object owns the raw device pointer (pDevice) as soon
  // as the constructor is called, even if the constructor throws.
  DeviceInstance(CMMCore* core, std::shared_ptr<LoadedDeviceAdapter> adapter,
                 const std::string& name, MM::Device* pDevice, DeleteDeviceFunction deleteFunction,
                 const std::string& label, mm::logging::Logger deviceLogger,
                 mm::logging::Logger coreLogger);
  virtual ~DeviceInstance();
};

class LoadedDeviceAdapter /* final */ : public std::enable_shared_from_this<LoadedDeviceAdapter> {
 public:
  LoadedDeviceAdapter(const LoadedDeviceAdapter&) = delete;
  LoadedDeviceAdapter& operator=(const LoadedDeviceAdapter&) = delete;

  LoadedDeviceAdapter(const std::string& name, const std::string& filename);

  std::string GetDeviceDescription(const std::string& deviceName) const;
  MM::DeviceType GetAdvertisedDeviceType(const std::string& deviceName) const;

  std::shared_ptr<DeviceInstance> LoadDevice(CMMCore* core, const std::string& name,
                                             const std::string& label,
                                             mm::logging::Logger deviceLogger,
                                             mm::logging::Logger coreLogger);
};

// Common member function implementations for concrete DeviceInstance
// subclasses.
template <typename TMMDevice>
class DeviceInstanceBase : public DeviceInstance {
 public:
  typedef TMMDevice RawDeviceClass;

 protected:
  DeviceInstanceBase(CMMCore* core, std::shared_ptr<LoadedDeviceAdapter> adapter,
                     const std::string& name, MM::Device* pDevice,
                     DeleteDeviceFunction deleteFunction, const std::string& label,
                     mm::logging::Logger deviceLogger, mm::logging::Logger coreLogger)
      : DeviceInstance(core, adapter, name, pDevice, deleteFunction, label, deviceLogger,
                       coreLogger) {}

 protected:
  RawDeviceClass* GetImpl() const /* final */ { return static_cast<RawDeviceClass*>(pImpl_); }
};

// StateInstance shown below is just one of many DeviceInstanceBase subclasses
// other subclasses include:
//   CameraDevice,
//   ShutterDevice,
//   StateDevice,
//   StageDevice,
//   XYStageDevice,
//   SerialDevice,
//   GenericDevice,
//   AutoFocusDevice,
//   CoreDevice,
//   ImageProcessorDevice,
//   SignalIODevice,
//   MagnifierDevice,
//   SLMDevice,
//   HubDevice,
//   GalvoDevice

class StateInstance : public DeviceInstanceBase<MM::State> {
 public:
  StateInstance(CMMCore* core, std::shared_ptr<LoadedDeviceAdapter> adapter,
                const std::string& name, MM::Device* pDevice, DeleteDeviceFunction deleteFunction,
                const std::string& label, mm::logging::Logger deviceLogger,
                mm::logging::Logger coreLogger)
      : DeviceInstanceBase<MM::State>(core, adapter, name, pDevice, deleteFunction, label,
                                      deviceLogger, coreLogger) {}

  int GetPosition(long& pos) const;
  int SetPosition(long pos);
  unsigned long GetNumberOfPositions() const;
  int SetGateOpen(bool open = true);
  int GetGateOpen(bool& open);
};

class CPluginManager /* final */
{
 public:
  CPluginManager();
  ~CPluginManager();
  /**
   * Return a device adapter module, loading it if necessary
   */
  std::shared_ptr<LoadedDeviceAdapter> GetDeviceAdapter(const std::string& moduleName);
  std::shared_ptr<LoadedDeviceAdapter> GetDeviceAdapter(const char* moduleName);
};
