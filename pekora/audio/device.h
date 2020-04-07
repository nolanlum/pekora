#ifndef PEKORA_PEKORA_AUDIO_DEVICE_H_
#define PEKORA_PEKORA_AUDIO_DEVICE_H_

#include "glog/logging.h"
#include "initguid.h"
#include "audioclient.h"
#include "mmdeviceapi.h"
#include "Winerror.h"

namespace pekora {
namespace audio {

const float REFERENCE_TICKS_PER_MS = 10000.0;

class AudioDevice {
 public:
  AudioDevice(IMMDevice*, IAudioClient3*);

  void PrintCapabilities();
 private:
  IMMDevice *device_;
  IAudioClient3 *audio_client_;
};

class AudioInputDevice : public AudioDevice {
 public:
  AudioInputDevice(IMMDevice*, IAudioClient3*);
};

class AudioOutputDevice : public AudioDevice {
 public:
  AudioOutputDevice(IMMDevice*, IAudioClient3*);
};

} // namespace audio
} // namespace pekora

#endif //PEKORA_PEKORA_AUDIO_DEVICE_H_
