#include "udp_receiver.h"
#include <QThread>
#include "absl/status/statusor.h"
#include "glog/logging.h"
#include "plot_data_model.h"

namespace qt_visualizer {

std::unique_ptr<UdpReceiver> UdpReceiver::Create(
    qt_visualizer::PlotDataModel* model, quint16 port) {
  auto receiver = std::unique_ptr<UdpReceiver>(new UdpReceiver(model, port));
  return receiver;
}

UdpReceiver::UdpReceiver(PlotDataModel* model, quint16 port)
    : model_(model), socket_(std::make_unique<QUdpSocket>()) {}

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
  LOG(INFO) << "Received data";
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