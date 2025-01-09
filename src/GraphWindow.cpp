#include <QPainter>
#include "GraphWindow.h"


GraphWindow::GraphWindow(QWidget *parent) : QWidget(parent) {
  resize(820, 820);
  graph = new Graph();

}

void GraphWindow::paintEvent(QPaintEvent *event) {                                                                      //Метод отрисовки графа
  QPainter painter(this);
  graph->generateVertexList();
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setBrush(Qt::blue);
  painter.drawEllipse(0, 0, 800, 800);
  drawVertex(&painter);
  drawEdge(&painter, Qt::black);
}

void GraphWindow::drawVertex(QPainter *painter) const {                                                                 //Метод отрисовки узлов графа
  painter->setBrush(Qt::yellow);
  painter->setPen(QPen(Qt::black,2));

  for(const Vertex vertex : graph->getVertexList()) {
    QFont font = painter->font();
    QString vertexId = QString::number(vertex.id);  // Преобразуем id в строку
    QRect textRect = QRect(vertex.position.x(), vertex.position.y(), 20, 20);

    font.setPixelSize(10);
    painter->setFont(font);
    painter->drawEllipse(vertex.position.x(), vertex.position.y(), 20, 20);
    painter->drawText(textRect, Qt::AlignCenter, vertexId);

  }

}


void GraphWindow::drawLine(DrawLineProps props) const {                                                                 // Отрисовывает текст и узел
  QFont font = props.painter->font();                                                                                   // Разворачивает текст на вычисленный угол наклона вдоль линии
  font.setPointSize(10);
  props.painter->setFont(font);
  props.painter->setPen(QPen(props.color, 2));
  props.painter->drawLine(props.points.first, props.points.second);
  props.painter->setPen(QPen(Qt::white, 2));
  props.painter->save();
  props.painter->translate(props.textPosition);
  props.painter->rotate(angleBetweenPoints(props.points.first, props.points.second));
  props.painter->translate(-props.textPosition);
  props.painter->drawText(props.textPosition, QString::number(props.weight));
  props.painter->restore();
}

void GraphWindow::drawEdge(QPainter *painter, QColor color) const {                                                     // Отрисовывает все вершины
  std::vector<std::vector<Edge>> edgeVector = graph->getDiscoveredPathVectors();

  for (const Edge edge : graph->getEdgeSet()) {                                                                         // Отрисовывает только рёбра графа
   std::pair<QPoint, QPoint> points = graph->getAdjustedLineEndpoints(edge.verticesPair);
    const QPoint textPosition((points.first.x() + points.second.x()) / 2, (points.first.y() + points.second.y()) / 2);
    drawLine({painter, textPosition, points, Qt::black, edge.weight});
  }

  for(int i = 0; i < edgeVector.size(); i++) {                                                                          // Отрисовывает те рёбра что являются частью найденого пути
    for (const Edge edge : edgeVector[i]) {
      std::pair<QPoint, QPoint> points = graph->getAdjustedLineEndpoints(edge.verticesPair);
      const QPoint textPosition((points.first.x() + points.second.x()) / 2, (points.first.y() + points.second.y()) / 2);
      drawLine({painter, textPosition, points, Qt::red, edge.weight});
    }
  }

}

void GraphWindow::updateGraph() {                                                                                       // Обновляет граф по нажатию на кнопку
  graph->clearEdgeSet();
  graph->clearVertexList();
  graph->generateVertexList();
  graph->clearPathVector();
  graph->clearDiscoveredPathVectors();
  repaint();
}

void GraphWindow::updateEdges() {                                                                                       // Генерирует узлы и обновляет граф по нажатию на кнопку
  graph->generateEdgeSet();
  graph->clearPathVector();
  graph->clearDiscoveredPathVectors();
  repaint();
}

void GraphWindow::updatePathVector(int startVertex, int endVertex, int lowerBound, int upperBound) {                    // Генерирует путь и обновляет граф по нажатию на кнопку
  graph->setStartVertexIndex(startVertex);
  graph->setEndVertexIndex(endVertex);
  graph->setLowerBoundValue(lowerBound);
  graph->setUpperBoundValue(upperBound);
  graph->clearPathVector();
  graph->clearDiscoveredPathVectors();
  graph->buildPath();
  repaint();
}

double GraphWindow::angleBetweenPoints(const QPoint& point1, const QPoint& point2) {                                    // Вспомогательный метод для отрисовки текста под углом
  const int deltaX = point2.x() - point1.x();
  const int deltaY = point2.y() - point1.y();
  const double angle = qAtan2(deltaY, deltaX);                                                                          // Получаем угол в радианах
  const double angleDeg = qRadiansToDegrees(angle);                                                                     // Преобразуем угол в градусы
  return  angleDeg;
}
