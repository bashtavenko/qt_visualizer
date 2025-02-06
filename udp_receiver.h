#ifndef QT_CMAKE_UDP_RECEIVER_H_
#define QT_CMAKE_UDP_RECEIVER_H_

#include <QObject>
#include <QUdpSocket>
#include <memory>
#include "absl/status/statusor.h"
#include "plot_data_model.h"

namespace qt_visualizer {

class UdpReceiver : public QObject {
  Q_OBJECT

 public:
  static std::unique_ptr<UdpReceiver> Create(
      PlotDataModel* model, quint16 port = 9000);
  absl::Status InitSocket(quint16 port = 9000);
 public slots:
  void HandleData();
  quint16 Port() const {return port_;}
 private:
  explicit UdpReceiver(PlotDataModel* model, quint16 port);
  std::unique_ptr<QUdpSocket> socket_;
  PlotDataModel* model_;  // owned by a caller
  const quint16 port_;
};

}  // namespace qt_visualizer

#endif  // QT_CMAKE_UDP_RECEIVER_H_
