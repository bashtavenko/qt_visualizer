#include <QThread>
#include <QtCore/QTimer>
#include <QtQuick/QQuickView>
#include <QtWidgets/QApplication>
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "plot_data_model.h"
#include "udp_receiver.h"

int main(int argc, char* argv[]) {
  using qt_visualizer::PlotDataModel;
  google::InitGoogleLogging(*argv);
  gflags::ParseCommandLineFlags(&argc, &argv, /*remove_flags=*/true);

  // In CMake glog flags work differently than in Bazel
  FLAGS_logtostderr = 1;
  FLAGS_colorlogtostderr = 1;

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
  // TODO: Thread synchronization
  // QObject: Cannot create children for a parent that is in a different thread.
  // (Parent is QUdpSocket(0x5da2d58d9840), parent's thread is
  // QThread(0x5da2d556cc70), current thread is QThread(0x7ffee007e410)
  // Something like
  // socket_->moveToThread(QThread::currentThread());
  // Happens after worker_thread.start()
  receiver->moveToThread(&worker_thread);
  QObject::connect(&worker_thread, &QThread::started, [&receiver]() {
    if (!receiver->InitSocket().ok()) {
      LOG(ERROR) << "Failed to initialize UDP socket";
    } else {
      LOG(INFO) << "UDP listener started";
    }
  });
  worker_thread.start();
  return app.exec();
}
