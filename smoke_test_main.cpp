// Smoke testing the server by directly calling it
// ./cmake-build-debug/smoke_test_main --data_path=testdata/lidar.txtpb
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fstream>
#include <iostream>
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/status/status.h"
#include "absl/strings/str_format.h"
#include "gflags/gflags.h"
#include "glog/logging.h"

constexpr char kMachine[] = "127.0.0.1";
constexpr int32_t kPort = 9000;

ABSL_FLAG(std::string, data_path, "testdata/lidar.txtpb",
          "Lidar data in proto text format");

absl::Status Run() {
  int32_t sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) return absl::InternalError("Failed to create socket");

  struct sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(kPort);

  int32_t result = inet_pton(AF_INET, kMachine, &server_addr.sin_addr);
  if (result <= 0) {
    close(sockfd);
    return absl::InternalError(absl::StrFormat(
        "Invalid address format %s:%i. Result: %i", kMachine, kPort, result));
  }

  std::ifstream file(absl::GetFlag(FLAGS_data_path));
  std::string data((std::istreambuf_iterator<char>(file)),
                   std::istreambuf_iterator<char>());
  if (data.empty()) {
    return absl::InternalError("Failed to open data file " +
                               absl::GetFlag(FLAGS_data_path));
  }
  ssize_t sent_bytes =
      sendto(sockfd, data.c_str(), data.size(), 0,
             (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (sent_bytes < 0) {
    close(sockfd);
    return absl::InternalError("Failed to send message.");
  }
  LOG(INFO) << "Message sent with length of bytes: " << sent_bytes;
  return absl::OkStatus();
}

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  absl::ParseCommandLine(argc, argv);
  FLAGS_alsologtostderr = 1;
  FLAGS_colorlogtostdout = 1;
  auto run_status = Run();
  if (!run_status.ok()) {
    LOG(ERROR) << run_status.message();
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}