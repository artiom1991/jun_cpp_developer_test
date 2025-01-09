#pragma once
#include <vector>
#include <set>


struct Path {
public:
  Path(const std::vector<int> edgeList, const int weight) : edgeVector(edgeList), weight(weight) {}

  Path getExtendedCopy(const int id, const int weight) const {
    Path tmp = Path(edgeVector, weight);
    tmp.weight = weight;
    tmp.edgeVector.push_back(id);
    return tmp;
  }

public:
  std::vector<int> edgeVector;
  double weight;

};

struct Vertex {
public:
    int id;
    QPoint position;
    std::vector<int> neighborIdList;
};

struct Edge {
public:
  Edge(const std::pair<int, int> verticesPair, const int weight) : verticesPair(verticesPair), weight(weight) {}

public:
  std::pair<int, int> verticesPair;
  int weight;

};

struct EdgeComparator {
  bool operator()(const Edge& edge1, const Edge& edge2) const {                                                         // Сортируем вершины так, чтобы (min, max) всегда было одинаковым
    const int v1_1 = std::min(edge1.verticesPair.first, edge1.verticesPair.second);
    const int v1_2 = std::max(edge1.verticesPair.first, edge1.verticesPair.second);
    const int v2_1 = std::min(edge2.verticesPair.first, edge2.verticesPair.second);
    const int v2_2 = std::max(edge2.verticesPair.first, edge2.verticesPair.second);
    if (v1_1 != v2_1) return v1_1 < v2_1;                                                                               // Сначала сравниваем вершины
    if (v1_2 != v2_2) return v1_2 < v2_2;
    return edge1.weight < edge2.weight;                                                                                 // Если вершины одинаковые, сравниваем по весу
  }
};

class Graph final {

public:
  void generateVertexList();
  void generateEdgeSet();

  void clearVertexList();
  void clearEdgeSet();
  void clearPathVector();
  void clearDiscoveredPathVectors ();

  std::vector<Vertex> getVertexList() const;
  std::set<Edge, EdgeComparator> getEdgeSet() const;
  std::pair<QPoint, QPoint> getAdjustedLineEndpoints(std::pair<int, int> vertexPair) const;
  int getVertexListSize() const;
  int getEdgeSetSize() const;
  Vertex getVertex(int id) const;
  Edge getEdge(int id) const;
  int getStartVertexIndex() const;
  int getEndVertexIndex() const;
  int getLowerBoundValue() const;
  int getUpperBoundValue() const;
  std::vector<std::vector<Edge>> getDiscoveredPathVectors () const;
  std::vector<Path> getPathVector() const;

  void setStartVertexIndex(int id);
  void setEndVertexIndex(int id);
  void setLowerBoundValue(int value);
  void setUpperBoundValue(int value);
  void setDiscoveredPathVectors (std::vector<std::vector<Edge>> pathVectors);

  double calcDistance(int first, int second) const;
  void buildPath();


protected:
  QPoint generatePoint() const;
  bool isPointInsideCircle(const QPoint& point, const QPoint& center, int radius) const;
  bool isOverlappingWithExistingCircles(const QPoint& newCircle) const;
  void addVertex(QPoint point);
  void addEdge(Edge edge);

  void linkVertexNeighbors  (int first, int second);

  std::vector<Path> findPath();
  std::vector<std::vector<Edge>> convertPathToEdge(std::vector<Path> pathVector);

private:
  std::vector<Vertex> vertexList;
  std::set<Edge, EdgeComparator> edgeSet;
  std::vector<Path> discoveredPaths;
  std::vector<std::vector<Edge>> discoveredPathVectors;
  int startVertex = 0;
  int endVertex = 1;
  int lowerBound = 0;
  int upperBound = 150;

};
