#ifndef QT_VISUALIZER__PROTO_UTILS_H_
#define QT_VISUALIZER__PROTO_UTILS_H_

#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "proto/scan_response.pb.h"

namespace qt_visualizer {

// Gets text from the given file
absl::StatusOr<std::string> GetTextFromFile(absl::string_view file_path);

// Parses proto::ScanResponse from the given proto text string
absl::StatusOr<slam_dunk::proto::ScanResponse> ParseProtoFromText(
    absl::string_view text_proto);

}  // namespace qt_visualizer

#endif  // QT_VISUALIZER__PROTO_UTILS_H_
