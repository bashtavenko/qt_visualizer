#include "udp_receiver.h"
#include <QThread>
#include "absl/status/statusor.h"
#include "glog/logging.h"
#include "plot_data_model.h"
#include "proto/scan_response.pb.h"
#include "proto_utils.h"

namespace qt_visualizer {

std::unique_ptr<UdpReceiver> UdpReceiver::Create(
    qt_visualizer::PlotDataModel* model, quint16 port) {
  auto receiver = std::unique_ptr<UdpReceiver>(new UdpReceiver(model, port));
  return receiver;
}

UdpReceiver::UdpReceiver(PlotDataModel* model, quint16 port)
    : model_(model), socket_(std::make_unique<QUdpSocket>()), port_(port) {}

absl::Status UdpReceiver::InitSocket(quint16 port) {
  socket_->moveToThread(QThread::currentThread());
  if (!socket_->bind(QHostAddress::LocalHost, port)) {
    return absl::InternalError("Failed to bind UDP socket to port " +
                               std::to_string(port));
  }
  QObject::connect(socket_.get(), &QUdpSocket::readyRead, this,
                   &UdpReceiver::HandleData);
  return absl::OkStatus();
}

void UdpReceiver::HandleData() {
  using slam_dunk::proto::ScanResponse;
  while (socket_->hasPendingDatagrams()) {
    QByteArray buffer;
    buffer.resize(socket_->pendingDatagramSize());
    socket_->readDatagram(buffer.data(), buffer.size());
    absl::StatusOr<ScanResponse> message =
        qt_visualizer::ParseProtoFromText(buffer.toStdString());
    if (!message.ok()) {
      LOG(ERROR) << "Failed to parse proto message";
      return;
    }
    QVariantList points;
    for (const auto& item : message->items()) {
      QVariantMap point;
      point["angle"] = item.theta() * 90 / (1 << 14);
      point["distance"] = item.distance_mm();
      points.append(point);
    }
    model_->SetPoints(points);
  }
}

}  // namespace qt_visualizer