#include "plot_data_model.h"

namespace qt_visualizer {

void PlotDataModel::SetPoints(const QVariantList& points) {
  points_ = points;
  emit PointsChanged();
}

} // namespace qt_visualizer