#include "MainWindow.h"



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setFixedSize(820, 1000);

    centralWidget = new QWidget(this);
    layout = new QVBoxLayout(centralWidget);
    controls = new ControlsWidget(this);
    graph = new GraphWindow(this);

    layout->addWidget(graph);
    layout->addWidget(controls);
    layout->setStretch(0, 1);
    layout->setStretch(1, 0);

    setCentralWidget(centralWidget);

    connect(controls, &ControlsWidget::onGenerateVerticesButtonClick, graph, &GraphWindow::updateGraph);
    connect(controls, &ControlsWidget::onGenerateEdgesButtonClick, graph, &GraphWindow::updateEdges);
    connect(controls, &ControlsWidget::pathParametersChanged, graph, &GraphWindow::updatePathVector);
}




