#include "wsapi_enumerator.h"

#include <codecvt>
#include <iostream>
#include <locale>

#include "absl/synchronization/mutex.h"
#include "combaseapi.h"
#include "comdef.h"
#include "functiondiscoverykeys_devpkey.h"
#include "objbase.h"
#include "propkey.h"

namespace pekora {
namespace audio {

WsapiDeviceEnumerator::WsapiDeviceEnumerator() {
}

WsapiDeviceEnumerator::~WsapiDeviceEnumerator() {
  if (this->has_data) {
    HRESULT hr = this->Cleanup();
    CHECK(!FAILED(hr)) << "Cleanup during WsapiDeviceEnumerator failed";
  }
}

HRESULT WsapiDeviceEnumerator::Enumerate(EDataFlow endpoint_type) {
  if (this->has_data) {
    HRESULT cleanup_hr = this->Cleanup();
    CHECK(!FAILED(cleanup_hr)) << "Cleanup before WsapiDeviceEnumerator::Enumerate failed";
  }

  HRESULT hr = CoInitializeEx(NULL, COINIT::COINIT_MULTITHREADED);
  CHECK(!FAILED(hr)) << "Could not CoInitializeEx";

  hr = CoCreateInstance(
      CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void **) &this->device_enumerator_);
  CHECK(!FAILED(hr)) << "Could not create MMDeviceEnumerator";

  hr = this->device_enumerator_->EnumAudioEndpoints(endpoint_type, DEVICE_STATE_ACTIVE, &this->device_collection_);
  CHECK(!FAILED(hr)) << "Could not EnumAudioEndpoints";

  UINT device_count = 0;
  hr = this->device_collection_->GetCount(&device_count);
  CHECK(!FAILED(hr)) << "Could not get audio device count";

  std::wstring_convert<std::codecvt_utf8<wchar_t>> wstring_convert;
  for (UINT i = 0; i < device_count; i++) {
    device_list.push_back(DeviceDescription());
    DeviceDescription *waspi_device = &device_list[i];

    hr = this->device_collection_->Item(i, &waspi_device->device);
    CHECK(!FAILED(hr)) << "Could not get audio device";

    LPWSTR endpoint_id = NULL;
    hr = waspi_device->device->GetId(&endpoint_id);
    CHECK(!FAILED(hr)) << "Could not get audio device endpoint ID";
    waspi_device->endpoint_id = wstring_convert.to_bytes(endpoint_id);

    IPropertyStore *pProps = NULL;
    hr = waspi_device->device->OpenPropertyStore(STGM_READ, &pProps);
    CHECK(!FAILED(hr)) << "Could not open audio device property store";

    PROPVARIANT prop_name;
    PropVariantInit(&prop_name);
    hr = pProps->GetValue(PKEY_Device_FriendlyName, &prop_name);
    CHECK(!FAILED(hr)) << "Could not get audio device friendly name";
    waspi_device->friendly_name = wstring_convert.to_bytes(prop_name.pwszVal);

    PROPVARIANT prop_supports_raw;
    PropVariantInit(&prop_supports_raw);
    hr = pProps->GetValue(PKEY_Devices_AudioDevice_RawProcessingSupported, &prop_name);
    CHECK(!FAILED(hr)) << "Could not query raw processing support";
    waspi_device->supports_raw_mode = prop_supports_raw.boolVal;

    CoTaskMemFree(endpoint_id);
    endpoint_id = NULL;
    PropVariantClear(&prop_name);
    PropVariantClear(&prop_supports_raw);
    pProps->Release();
  }

  this->has_data = true;
  return 0;
}

HRESULT WsapiDeviceEnumerator::EnumerateInput() {
  return this->Enumerate(EDataFlow::eCapture);
}

HRESULT WsapiDeviceEnumerator::EnumerateOutput() {
  return this->Enumerate(EDataFlow::eRender);
}

HRESULT WsapiDeviceEnumerator::Cleanup() {
  for (int i = 0; i < this->device_list.size(); i++) {
    DeviceDescription *waspi_device = &device_list[i];
    if (waspi_device->device != NULL) {
      waspi_device->device->Release();
      waspi_device->device = NULL;
    }
  }

  this->device_list.clear();

  if (this->device_collection_ != NULL) {
    this->device_collection_->Release();
    this->device_collection_ = NULL;
  }
  if (this->device_enumerator_ != NULL) {
    this->device_enumerator_->Release();
    this->device_enumerator_ = NULL;
  }

  return 0;
}

const std::vector <DeviceDescription> WsapiDeviceEnumerator::GetDeviceList() const {
  return this->device_list;
}

AudioInputDevice WsapiDeviceEnumerator::OpenInputDevice(int device_index) {
  IAudioClient3 *audio_client = NULL;
  HRESULT hr = this->device_list[device_index].device->Activate(
      IID_IAudioClient3, CLSCTX_ALL, NULL, (void**) &audio_client);
  CHECK(!FAILED(hr)) << "Could not activate audio device";

  if (this->device_list[device_index].supports_raw_mode) {
    AudioClientProperties audio_props = {0};
    audio_props.cbSize = sizeof(AudioClientProperties);
    audio_props.eCategory = AudioCategory_Media;
    audio_props.Options |= AUDCLNT_STREAMOPTIONS_RAW;
    hr = audio_client->SetClientProperties(&audio_props);
    CHECK(!FAILED(hr)) << "Call to SetClientProperties failed: " << _com_error(hr).ErrorMessage();
  }

  this->device_list[device_index].device = NULL;
  this->Cleanup();

  return AudioInputDevice(audio_client);
}

AudioOutputDevice WsapiDeviceEnumerator::OpenOutputDevice(int device_index) {
  IAudioClient3 *audio_client = NULL;
  HRESULT hr = this->device_list[device_index].device->Activate(
      IID_IAudioClient3, CLSCTX_ALL, NULL, (void**) &audio_client);
  CHECK(!FAILED(hr)) << "Could not activate audio device";

  if (this->device_list[device_index].supports_raw_mode) {
    AudioClientProperties audio_props = {0};
    audio_props.cbSize = sizeof(AudioClientProperties);
    audio_props.eCategory = AudioCategory_Media;
    audio_props.Options |= AUDCLNT_STREAMOPTIONS_RAW;
    hr = audio_client->SetClientProperties(&audio_props);
    CHECK(!FAILED(hr)) << "Call to SetClientProperties failed: " << _com_error(hr).ErrorMessage();
  }

  this->device_list[device_index].device = NULL;
  this->Cleanup();

  return AudioOutputDevice(audio_client);
}

} // namespace audio
} // namespace pekora
