#pragma once
#include <QtWidgets>
#include "GraphWindow.h"
#include "ControlsWidget.h"


class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);

private:
    QWidget *centralWidget;
    QVBoxLayout *layout;
    ControlsWidget *controls;
    GraphWindow *graph;
};

