#pragma once
#include <QWidget>
#include "Graph.h"


struct DrawLineProps {
  QPainter *painter;
  QPoint textPosition;
  std::pair<QPoint, QPoint> points;
  QColor color;
  int weight;
};


class GraphWindow final : public QWidget {
  Q_OBJECT

public:
  explicit GraphWindow(QWidget *parent = nullptr);

protected:
  void paintEvent(QPaintEvent *event) override;
  void drawVertex(QPainter *painter) const;
  void drawEdge(QPainter *painter, QColor color) const;
  void drawLine(DrawLineProps props) const;
  static double angleBetweenPoints(const QPoint& point1, const QPoint& point2);

private:
  Graph *graph;

public slots:
  void updateGraph();
  void updateEdges();
  void updatePathVector(int startVertex, int endVertex, int lowerBound, int upperBound);

};


