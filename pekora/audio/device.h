#ifndef PEKORA_PEKORA_AUDIO_DEVICE_H_
#define PEKORA_PEKORA_AUDIO_DEVICE_H_

#include "glog/logging.h"
#include "audioclient.h"
#include "mmdeviceapi.h"
#include "Winerror.h"

namespace pekora {
namespace audio {

const float REFERENCE_TICKS_PER_MS = 10000.0;

class AudioDevice {
 public:
  AudioDevice(IAudioClient3*);

  void PrintCapabilities();
 private:
  IAudioClient3 *audio_client_;
};

class AudioInputDevice : public AudioDevice {
 public:
  AudioInputDevice(IAudioClient3*);
};

class AudioOutputDevice : public AudioDevice {
 public:
  AudioOutputDevice(IAudioClient3*);
};

} // namespace audio
} // namespace pekora

#endif //PEKORA_PEKORA_AUDIO_DEVICE_H_
