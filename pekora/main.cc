#include <iostream>

#include "glog/logging.h"
#include "absl/strings/str_format.h"

#include "audio/wsapi_enumerator.h"
#include "audio/device.h"

void printDevices() {
  pekora::audio::WsapiDeviceEnumerator ac;
  ac.EnumerateInput();
  LOG(INFO) << "Input Device Count: " << ac.GetDeviceList().size() << std::endl;
  for (int i = 0; i < ac.GetDeviceList().size(); i++) {
    LOG(INFO)
        << absl::StreamFormat("%d: %s (%s)", i, ac.GetDeviceList()[i].friendly_name, ac.GetDeviceList()[i].endpoint_id)
        << std::endl;
  }

  ac.EnumerateOutput();
  LOG(INFO) << "Output Device Count: " << ac.GetDeviceList().size() << std::endl;
  for (int i = 0; i < ac.GetDeviceList().size(); i++) {
    LOG(INFO)
        << absl::StreamFormat("%d: %s (%s)", i, ac.GetDeviceList()[i].friendly_name, ac.GetDeviceList()[i].endpoint_id)
        << std::endl;
  }
}

int main(int argc, char *argv[]) {
  FLAGS_logtostderr = 1;
  google::InitGoogleLogging(argv[0]);

  pekora::audio::WsapiDeviceEnumerator ac;

  pekora::audio::AudioInputDevice input_device = ac.OpenInputDevice(-1);
  input_device.PrintCapabilities();

  pekora::audio::AudioOutputDevice output_device = ac.OpenOutputDevice(-1);
  output_device.PrintCapabilities();

  return 0;
}
