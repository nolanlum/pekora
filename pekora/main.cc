#include <iostream>

#include "glog/logging.h"
#include "absl/strings/str_format.h"

#include "audio/wsapi_enumerator.h"
#include "audio/device.h"

int main(int argc, char *argv[]) {
  FLAGS_logtostderr = 1;
  google::InitGoogleLogging(argv[0]);

  pekora::audio::WsapiDeviceEnumerator ac;
  ac.EnumerateInput();
  LOG(INFO) << "Input Device Count: " << ac.GetDeviceList().size() << std::endl;
  for (int i = 0; i < ac.GetDeviceList().size(); i++) {
    LOG(INFO)
        << absl::StreamFormat("%d: %s (%s)", i, ac.GetDeviceList()[i].friendly_name, ac.GetDeviceList()[i].endpoint_id)
        << std::endl;
  }
  pekora::audio::AudioInputDevice input_device = ac.OpenInputDevice(3);
  input_device.PrintCapabilities();

  ac.EnumerateOutput();
  LOG(INFO) << "Output Device Count: " << ac.GetDeviceList().size() << std::endl;
  for (int i = 0; i < ac.GetDeviceList().size(); i++) {
    LOG(INFO)
        << absl::StreamFormat("%d: %s (%s)", i, ac.GetDeviceList()[i].friendly_name, ac.GetDeviceList()[i].endpoint_id)
        << std::endl;
  }
  pekora::audio::AudioOutputDevice output_device = ac.OpenOutputDevice(3);
  output_device.PrintCapabilities();

  return 0;
}