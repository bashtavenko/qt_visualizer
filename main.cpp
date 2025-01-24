#include <QThread>
#include <QtCore/QTimer>
#include <QtQuick/QQuickView>
#include <QtWidgets/QApplication>
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "plot_data_model.h"
#include "udp_receiver.h"

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(*argv);
  gflags::ParseCommandLineFlags(&argc, &argv, /*remove_flags=*/false);
  gflags::SetCommandLineOption("logtostderr", "1");
  using qt_visualizer::PlotDataModel;
  QApplication app(argc, argv);
  auto viewer = std::make_unique<QQuickView>();
  PlotDataModel model;
  viewer->rootContext()->setContextProperty("scanData", &model);
  viewer->setSource(QUrl("qrc:/main.qml"));
  viewer->show();

  QThread worker_thread;
  auto receiver = qt_visualizer::UdpReceiver::Create(&model);
  if (!receiver) {
    LOG(ERROR) << "Failed to create UdpReceiver";
    return EXIT_FAILURE;
  }
  receiver->moveToThread(&worker_thread);
  QObject::connect(&worker_thread, &QThread::started,
                   []() { std::cout << "UDP listener started...\n"; });
  worker_thread.start();
  return app.exec();
}
