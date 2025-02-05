#include "proto_utils.h"
#include <fstream>
#include <google/protobuf/text_format.h>

namespace qt_visualizer {

absl::StatusOr<std::string> GetTextFromFile(absl::string_view file_path) {
  std::ifstream file(file_path.data());
  std::string data((std::istreambuf_iterator<char>(file)),
                   std::istreambuf_iterator<char>());

  if (data.empty()) {
    return absl::InternalError("Failed to open data file " +
                               std::string(file_path));
  }
  return data;
}

absl::StatusOr<slam_dunk::proto::ScanResponse> ParseProtoFromText(
    absl::string_view text_proto) {
  slam_dunk::proto::ScanResponse message;
  if (!google::protobuf::TextFormat::ParseFromString(text_proto.data(), &message)) {
    return absl::InternalError("Failed to parse proto message");
  }
  return message;
}

} // namespace qt_visualizer