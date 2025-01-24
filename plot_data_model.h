#ifndef QT_CMAKE_PLOT_DATA_MODEL_H_
#define QT_CMAKE_PLOT_DATA_MODEL_H_

#include <QtQml/QQmlContext>

namespace qt_visualizer {

class PlotDataModel : public QObject {
  Q_OBJECT
  Q_PROPERTY(
      QVariantList points READ Points WRITE SetPoints NOTIFY PointsChanged)

 public:
  explicit PlotDataModel(QObject* parent = nullptr) : QObject(parent) {}

  QVariantList Points() const { return points_; }

 public slots:
  void SetPoints(const QVariantList& point);

 signals:
  void PointsChanged();

 private:
  QVariantList points_;
};

} // namespace qt_visualizer
#endif  // QT_CMAKE_PLOT_DATA_MODEL_H_
