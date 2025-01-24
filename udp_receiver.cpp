#include "udp_receiver.h"
#include "absl/status/statusor.h"
#include "glog/logging.h"
#include "plot_data_model.h"

namespace qt_visualizer {

UdpReceiver::UdpReceiver(PlotDataModel* model, quint16 port)
    : model_(model), socket_(std::make_unique<QUdpSocket>()) {}

absl::Status UdpReceiver::InitSocket(quint16 port) {
  if (!socket_->bind(QHostAddress::LocalHost, port)) {
    return absl::InternalError("Failed to bind UDP socket to port " +
                               std::to_string(port));
  }
  QObject::connect(socket_.get(), &QUdpSocket::readyRead, this,
                   &UdpReceiver::HandleData);
  return absl::OkStatus();
}

std::unique_ptr<UdpReceiver> UdpReceiver::Create(
    qt_visualizer::PlotDataModel* model, quint16 port) {
  auto receiver = std::unique_ptr<UdpReceiver>(new UdpReceiver(model, port));
  absl::Status status = receiver->InitSocket(port);
  if (!status.ok()) {
    LOG(ERROR) << "Fail to init socket at " << port;
    return nullptr;
  }
  return receiver;
}

void UdpReceiver::HandleData() {
  while (socket_->hasPendingDatagrams()) {
    QByteArray buffer;
    buffer.resize(socket_->pendingDatagramSize());
    socket_->readDatagram(buffer.data(), buffer.size());

    // TODO: Parse data from UDP and invoke model
    // or alternatively invokeMethod
    QVariantList points;
    model_->SetPoints(points);
  }
}

}  // namespace qt_visualizer