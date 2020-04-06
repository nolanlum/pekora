#include "device.h"

#include "absl/strings/str_format.h"

namespace pekora {
namespace audio {

AudioDevice::AudioDevice(IAudioClient3 *audio_client) {
  this->audio_client_ = audio_client;
}
void AudioDevice::PrintCapabilities() {
  WAVEFORMATEX *pwfx = NULL;
  HRESULT hr = this->audio_client_->GetMixFormat(&pwfx);
  CHECK(!FAILED(hr)) << "Call to GetMixFormat failed";

  LOG(INFO) << absl::StreamFormat("IAudioClient3: %d channels, %2.1fkHz",
                                  pwfx->nChannels, pwfx->nSamplesPerSec / 1000.0);

  REFERENCE_TIME default_device_period, minimum_device_period;
  hr = this->audio_client_->GetDevicePeriod(&default_device_period, &minimum_device_period);
  CHECK(!FAILED(hr)) << "Call to GetDevicePeriod failed";
  LOG(INFO) << absl::StreamFormat("  Device period: %0.1fms, exclusive mode: %0.1fms",
                                  default_device_period / REFERENCE_TICKS_PER_MS,
                                  minimum_device_period / REFERENCE_TICKS_PER_MS);

  UINT32 default_period_frames, fundamental_period_frames, min_period_frames, max_period_frames;
  hr = this->audio_client_->GetSharedModeEnginePeriod(
      pwfx, &default_period_frames, &fundamental_period_frames, &min_period_frames, &max_period_frames);
  CHECK(!FAILED(hr)) << "Call to GetSharedModeEnginePeriod failed";

  LOG(INFO) << absl::StreamFormat("  Default period: %d frames (%0.1fms), fundamental period: %d (%0.1fms)",
                                  default_period_frames, default_period_frames * 1000.0 / pwfx->nSamplesPerSec,
                                  fundamental_period_frames, fundamental_period_frames * 1000.0 / pwfx->nSamplesPerSec);
  LOG(INFO) << absl::StreamFormat("  Min: %d frames (%0.1fms), Max: %d frames (%0.1fms)",
                                  min_period_frames, min_period_frames * 1000.0 / pwfx->nSamplesPerSec,
                                  max_period_frames, max_period_frames * 1000.0 / pwfx->nSamplesPerSec);
}

AudioInputDevice::AudioInputDevice(IAudioClient3 *audio_client) : AudioDevice(audio_client) {

}

AudioOutputDevice::AudioOutputDevice(IAudioClient3 *audio_client) : AudioDevice(audio_client) {

}

} // namespace audio
} // namespace pekora
