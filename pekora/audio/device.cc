#include "device.h"

#include "absl/strings/str_format.h"
#include "propkey.h"

namespace pekora {
namespace audio {

static WAVEFORMATEX wf_2ch_441kHz_adpcm = {
    .wFormatTag = WAVE_FORMAT_IEEE_FLOAT,
    .nChannels = 2,
    .nSamplesPerSec = 44100,
    .nAvgBytesPerSec = 44100 * 32 / 8 * 2,
    .nBlockAlign = 8,
    .wBitsPerSample = 32,
    .cbSize = 0,
};

static WAVEFORMATEXTENSIBLE wfx_2ch_48kHz_float = {
    .Format = {
        .wFormatTag = WAVE_FORMAT_EXTENSIBLE,
        .nChannels = 2,
        .nSamplesPerSec = 48000,
        .nAvgBytesPerSec = 48000 * 32 / 8 * 2,
        .nBlockAlign = 8,
        .wBitsPerSample = 32,
        .cbSize = 22,
    },
    .Samples = {32},
    .dwChannelMask = 3,
    .SubFormat = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT,
};

static WAVEFORMATEXTENSIBLE wfx_2ch_441kHz_float = {
    .Format = {
        .wFormatTag = WAVE_FORMAT_EXTENSIBLE,
        .nChannels = 2,
        .nSamplesPerSec = 44100,
        .nAvgBytesPerSec = 44100 * 32 / 8 * 2,
        .nBlockAlign = 8,
        .wBitsPerSample = 32,
        .cbSize = 22,
    },
    .Samples = {32},
    .dwChannelMask = 3,
    .SubFormat = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT,
};

AudioDevice::AudioDevice(IMMDevice *device, IAudioClient3 *audio_client) {
  this->device_ = device;
  this->audio_client_ = audio_client;
}

std::string DumpWaveFormatEx(PWAVEFORMATEX pwfx) {
  std::string extended_info = {0};
  if (pwfx->wFormatTag == WAVE_FORMAT_EXTENSIBLE) {
    WAVEFORMATEXTENSIBLE *extended = reinterpret_cast<PWAVEFORMATEXTENSIBLE>(pwfx);
    GUID guid = extended->SubFormat;
    extended_info = absl::StrFormat(
        "\n"
        "  SubFormat GUID: {%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}\n"
        "  Valid bits per sample: %d, channel mask: %d",
        guid.Data1, guid.Data2, guid.Data3,
        guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
        guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7],
        extended->Samples.wValidBitsPerSample,
        extended->dwChannelMask
    );
  }

  return absl::StrFormat(
      "WAVEFORMAT %#X: %d channels, %d bits per sample, %2.1fkHz\n"
      "  %d bytes/sec, %d byte blocks%s",
      pwfx->wFormatTag, pwfx->nChannels, pwfx->wBitsPerSample, pwfx->nSamplesPerSec / 1000.0,
      pwfx->nAvgBytesPerSec, pwfx->nBlockAlign, extended_info
  );
}

void AudioDevice::PrintCapabilities() {
  IPropertyStore *pProps = NULL;
  HRESULT hr = this->device_->OpenPropertyStore(STGM_READ, &pProps);
  CHECK(!FAILED(hr)) << "Could not open audio device property store";

  PROPVARIANT prop_device_format;
  PropVariantInit(&prop_device_format);
  hr = pProps->GetValue(PKEY_AudioEngine_DeviceFormat, &prop_device_format);
  CHECK(!FAILED(hr)) << "Could not get exclusive mode format";
  PWAVEFORMATEX exclusive_mode_wfx = reinterpret_cast<PWAVEFORMATEX>(prop_device_format.blob.pBlobData);
  LOG(INFO) << "Exclusive mode " << DumpWaveFormatEx(exclusive_mode_wfx);

  WAVEFORMATEX *pwfx = NULL;
  hr = this->audio_client_->GetMixFormat(&pwfx);
  CHECK(!FAILED(hr)) << "Call to GetMixFormat failed";
  LOG(INFO) << "Shared mode " << DumpWaveFormatEx(pwfx);

  LOG(INFO) << absl::StreamFormat("0x%x",
                                  this->audio_client_->IsFormatSupported(AUDCLNT_SHAREMODE::AUDCLNT_SHAREMODE_EXCLUSIVE,
                                                                         exclusive_mode_wfx,
                                                                         NULL));

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

AudioInputDevice::AudioInputDevice(IMMDevice *device, IAudioClient3 *audio_client) :
    AudioDevice(device, audio_client) {
}

AudioOutputDevice::AudioOutputDevice(IMMDevice *device, IAudioClient3 *audio_client) :
    AudioDevice(device, audio_client) {
}

} // namespace audio
} // namespace pekora
