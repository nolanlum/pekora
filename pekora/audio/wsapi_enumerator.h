#ifndef PEKORA_AUDIO_INPUT_H
#define PEKORA_AUDIO_INPUT_H

#include <string>
#include <vector>

#include "glog/logging.h"
#include "audioclient.h"
#include "mmdeviceapi.h"
#include "Winerror.h"

#include "device.h"

namespace pekora {
namespace audio {

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient3 = __uuidof(IAudioClient3);

class DeviceDescription {
 public:
  void QueryDeviceInfo();

  IMMDevice *device = NULL;
  std::string endpoint_id;
  std::string friendly_name;
  bool supports_raw_mode;
};

class WsapiDeviceEnumerator {
 public:
  WsapiDeviceEnumerator();
  ~WsapiDeviceEnumerator();

  HRESULT EnumerateInput();
  HRESULT EnumerateOutput();
  HRESULT Cleanup();

  AudioInputDevice OpenInputDevice(int);
  AudioOutputDevice OpenOutputDevice(int);

  const std::vector<DeviceDescription> GetDeviceList() const;

 private:
  HRESULT Enumerate(EDataFlow);
  IAudioClient3* OpenDevice(IMMDevice*, bool);

  bool has_data = false;

  IMMDeviceEnumerator *device_enumerator_ = NULL;
  IMMDeviceCollection *device_collection_ = NULL;

  std::vector<DeviceDescription> device_list;
};

} // namespace audio
} // namespace pekora

#endif //PEKORA_AUDIO_INPUT_H
