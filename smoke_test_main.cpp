// Smoke testing the server by directly calling it
#include <arpa/inet.h>
#include <sys/socket.h>
#include "absl/status/status.h"
#include "absl/strings/str_format.h"
#include "gflags/gflags.h"
#include "glog/logging.h"

constexpr char kMachine[] = "127.0.0.1";
constexpr int32_t kPort = 9000;

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

  const char* message = "Test message";
  ssize_t sent_bytes =
      sendto(sockfd, message, strlen(message), 0,
             (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (sent_bytes < 0) {
    close(sockfd);
    return absl::InternalError("Failed to send message.");
  }

  LOG(INFO) << "Message sent";
  return absl::OkStatus();
}

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  gflags::ParseCommandLineFlags(&argc, &argv, /*remove_flags=*/true);
  FLAGS_alsologtostderr = 1;
  FLAGS_colorlogtostdout = 1;
  auto run_status = Run();
  if (!run_status.ok()) {
    LOG(ERROR) << run_status.message();
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}