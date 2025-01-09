#include <QtWidgets>
#include <random>
#include <set>
#include <queue>
#include "Graph.h"



std::vector<std::vector<Edge>> Graph::convertPathToEdge(std::vector<Path> pathVector) {                                 // Преобразует std::vector<Path> в std::vector<std::vector<Edge>>
  std::vector<std::vector<Edge>> edgeVector(pathVector.size());  // Инициализируем вектор подмассива

  for (int i = 0; i < pathVector.size(); i++) {
    if (pathVector.at(i).edgeVector.size() < 2) {
      continue;
    }

    for (int j = 1; j < pathVector.at(i).edgeVector.size(); j++) {
      std::pair<int, int> edgePair = {pathVector.at(i).edgeVector.at(j - 1), pathVector.at(i).edgeVector.at(j)};
      int distance = calcDistance(edgePair.first, edgePair.second);
      edgeVector.at(i).push_back(Edge(edgePair, distance));
    }
  }

  return edgeVector;

}


std::vector<Path> Graph::findPath() {                                                                                   // Модифицированный алгоритм поиска в ширину
  std::queue<Path> vertexQueue;
  std::vector<Path> pathList;
  vertexQueue.push(Path({startVertex}, 0));

  while (vertexQueue.size() > 0) {
    Path path = vertexQueue.front();
    Vertex currentVertex = getVertex(path.edgeVector.back());
    vertexQueue.pop();

    for (const int neighborId : currentVertex.neighborIdList) {
      int distance = path.weight + calcDistance(path.edgeVector.back(), neighborId);

      if (distance > upperBound) {
        continue;
      }

      Path neighborPath = path.getExtendedCopy(neighborId, distance);

      if (neighborId == endVertex and distance > lowerBound) {
        pathList.push_back(neighborPath);
        continue;
      }

      vertexQueue.push(neighborPath);

    }
  }

  return pathList;

}


double Graph::calcDistance(int first, int second) const {                                                               // Метод расчитывает дистанцию между двумя точками
  Vertex v1 = getVertex(first);
  Vertex v2 = getVertex(second);

  double deltaX = v2.position.x() - v1.position.x();
  double deltaY = v2.position.y() - v1.position.y();

  return std::sqrt(std::pow(deltaX, 2) + std::pow(deltaY, 2));
}

std::pair<QPoint, QPoint> Graph::getAdjustedLineEndpoints(std::pair<int, int> vertexPair) const {                       // Метод который вычисляет начало и конец координат для ребра от края круга
  Vertex first = getVertex(vertexPair.first);
  Vertex second = getVertex(vertexPair.second);
  double deltaX = second.position.x() - first.position.x();                                                             // Вычисляем вектор между двумя вершинами
  double deltaY = second.position.y() - first.position.y();
  double length = std::sqrt(deltaX * deltaX + deltaY * deltaY);                                                         // Находим длину этого вектора
  double shrinkFactor = 11.0 / length;                                                                                  // Коэффициент, на который нужно уменьшить длину линии, чтобы она не выходила за границу окружности
  int startX = first.position.x() + 11 + deltaX * shrinkFactor;                                                         // Рассчитываем координаты начала и конца линии, с учетом урезания
  int startY = first.position.y() + 11 + deltaY * shrinkFactor;
  int endX = second.position.x() + 11 - deltaX * shrinkFactor;
  int endY = second.position.y() + 11 - deltaY * shrinkFactor;

  QPoint start(startX, startY);
  QPoint end(endX, endY);
  return std::make_pair(start, end);
}

void Graph::generateEdgeSet() {                                                                                         // Генерирует множество связей между узлами
  for (int i = 0, counter = 0; i < getVertexListSize() ; counter = 0, i++) {
    std::set<std::pair<double, int>> tmpEdgeSet;
    Vertex first = getVertex(i);
    int value = 6 - first.neighborIdList.size();

    if(first.neighborIdList.size() > 5) { continue; }

    for (int j = 0; j < getVertexListSize(); j++) {
      if (i == j) { continue;}
      double distance = calcDistance(i, j);
      Vertex second = getVertex(j);

      if (second.neighborIdList.size() > 5 ) { continue; }

      tmpEdgeSet.insert({distance, j});
    }

    for (const auto& edge : tmpEdgeSet) {
      if (counter == value) { break; }
      Edge newEdge = Edge({i, edge.second}, edge.first);
      addEdge(newEdge);
      linkVertexNeighbors(i, edge.second);
      counter++;
    }
  }
}

void Graph::linkVertexNeighbors (int first, int second) {                                                               // Связывает для узла
  vertexList[first].neighborIdList.push_back(second);
  vertexList[second].neighborIdList.push_back(first);
}


void Graph::generateVertexList() {
  int radius = 800 / 2 - 40;                                                                                            // Уменьшенный радиус с учётом отступа
  QPoint center(10 + 800 / 2, 10 + 800 / 2);
  while (getVertexListSize() < 100) {                                                                                   // Генерация 100 узлов с случайными координатами внутри круга
    QPoint point = generatePoint();
    if (isPointInsideCircle(point, center, radius) and not isOverlappingWithExistingCircles(point)) {                   // Проверка: находятся ли координаты узла внутри круга или нет, а так же пересекается ли узел с другим узлом
      addVertex(point);
    }
  }
}


QPoint Graph::generatePoint() const {                                                                                   // Генерирует случайные координаты
  std::random_device device;
  std::mt19937 generator(device());
  std::uniform_int_distribution<int> distribution(0, 800);

  int posX = distribution(generator);
  int posY = distribution(generator);

  return QPoint(posX, posY);                                                                                            // Возвращаем объект QPoint с координатами (x, y)
}

bool Graph::isPointInsideCircle(const QPoint& point, const QPoint& center, int radius) const {                          // Проверяет находятся ли координаты внутри главного круга
  int deltaX = point.x() - center.x();
  int deltaY = point.y() - center.y();
  return (deltaX * deltaX + deltaY * deltaY) <= (radius * radius);
}

bool Graph::isOverlappingWithExistingCircles(const QPoint& newCircle) const {                                           // Проверяет есть ли пересечения координат узла с другим узлом
  int requiredDistance = 25;

  for (const auto& existingCircle : getVertexList()) {
    int deltaX = newCircle.x() - existingCircle.position.x();
    int deltaY = newCircle.y() - existingCircle.position.y();
    int distanceSquared = deltaX * deltaX + deltaY * deltaY;

    if (distanceSquared < (requiredDistance * requiredDistance)) {
      return true;
    }
  }

  return false;
}

std::vector<Vertex> Graph::getVertexList() const {                                                                      // Геттер для доступа к списку Узлов
  return vertexList;
}

std::set<Edge, EdgeComparator> Graph::getEdgeSet() const {                                                              // Геттер для списка рёбер
  return edgeSet;
}

void Graph::addVertex(QPoint position) {                                                                                // Добавляет (генерирует) узел с передаными координатами в вектор со всеми узлами
  Vertex vertex = { getVertexListSize() , position };
  vertexList.push_back(vertex);
}

void Graph::addEdge(Edge edge) {                                                                                        // Добавляет (генерирует) ребро в вектор со всеми рёбрами (новый метод с перегружеными параметрами для удобства)
  edgeSet.insert(edge);
}

int Graph::getEdgeSetSize() const {                                                                                     // Возвращает длину вектора с узлами
  return static_cast<int>(edgeSet.size());
}

int Graph::getVertexListSize() const {
  return static_cast<int>(vertexList.size());
}

void Graph::clearVertexList() {
  vertexList.clear();
}

void Graph::clearEdgeSet() {
  edgeSet.clear();
}

Vertex Graph::getVertex(int id) const{
  return vertexList.at(id);
}


void Graph::clearPathVector() {
  discoveredPaths.clear();
}

void Graph::setStartVertexIndex(int id) {
  startVertex = id;
}
void Graph::setEndVertexIndex(int id) {
  endVertex = id;
}

void Graph::setLowerBoundValue(int value) {
  lowerBound = value;
}
void Graph::setUpperBoundValue(int value) {
  upperBound = value;
}

std::vector<std::vector<Edge>> Graph::getDiscoveredPathVectors () const {
  return discoveredPathVectors;
}

void Graph::clearDiscoveredPathVectors () {
  discoveredPathVectors.clear();
}

std::vector<Path> Graph::getPathVector() const {
  return discoveredPaths;
}

void Graph::setDiscoveredPathVectors (std::vector<std::vector<Edge>> pathVectors) {
  discoveredPathVectors = pathVectors;
}

void Graph::buildPath() {
  std::vector<Path> pathVector = findPath();
  std::vector<std::vector<Edge>> pathVectors = convertPathToEdge(pathVector);
  setDiscoveredPathVectors(pathVectors);
}