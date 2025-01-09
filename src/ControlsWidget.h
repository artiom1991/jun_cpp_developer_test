#pragma once
#include <QtWidgets>


class ControlsWidget final : public QWidget {
  Q_OBJECT

public:
  explicit ControlsWidget(QWidget *parent = Q_NULLPTR);

protected:
  QHBoxLayout *generateInputGroupLayout(QSpinBox *&spinBox, const QString &labelText, int range, int value) const;

signals:
  void onGenerateVerticesButtonClick();
  void onGenerateEdgesButtonClick();
  void pathParametersChanged(int startVertex, int endVertex, int lowerBound, int upperBound);

public slots:
  void onFindPathButtonClick();

public:
  QPushButton *generateVerticesButton;
  QPushButton *generateEdgesButton;
  QPushButton *findPathButton;

  QSpinBox *startVertexSpinBox;
  QSpinBox *endVertexSpinBox;

  QSpinBox *lowerBoundSpinBox;
  QSpinBox *upperBoundSpinBox;

};


