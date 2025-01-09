#include "ControlsWidget.h"


QHBoxLayout *ControlsWidget::generateInputGroupLayout(QSpinBox *&spinBox, const QString &labelText, int range, int value) const {
    QHBoxLayout *layout = new QHBoxLayout();
    QLabel *label = new QLabel(labelText);

    spinBox = new QSpinBox();
    spinBox->setRange(0, range);
    spinBox->setValue(value);

    layout->addWidget(label);
    layout->addWidget(spinBox);

    return layout;
}

ControlsWidget::ControlsWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QGroupBox *groupBox = new QGroupBox("Search Path Settings:");
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QHBoxLayout *groupBoxLayout = new QHBoxLayout(groupBox);

    QVBoxLayout *groupBoxFirstColumbLayout = new QVBoxLayout();
    QVBoxLayout *groupBoxSecondColumbLayout = new QVBoxLayout();
    QVBoxLayout *groupBoxThirdColumbLayout = new QVBoxLayout();

    generateVerticesButton = new QPushButton("Generate Vertices");
    generateEdgesButton = new QPushButton("Generate Edges");
    findPathButton = new QPushButton("Find Path");
    findPathButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    groupBoxLayout->addLayout(groupBoxFirstColumbLayout);
    groupBoxLayout->addLayout(groupBoxSecondColumbLayout);
    groupBoxLayout->addLayout(groupBoxThirdColumbLayout);

    groupBoxFirstColumbLayout->addLayout(generateInputGroupLayout(lowerBoundSpinBox, QString("Lower Bound:"), 1000, 150));
    groupBoxFirstColumbLayout->addLayout(generateInputGroupLayout(upperBoundSpinBox, QString("Upper Bound:"), 1000, 350));
    groupBoxSecondColumbLayout->addLayout(generateInputGroupLayout(startVertexSpinBox, QString("Start Vertes:"), 99, 15));
    groupBoxSecondColumbLayout->addLayout(generateInputGroupLayout(endVertexSpinBox, QString("End Vertex:"), 99, 25));

    groupBoxThirdColumbLayout->addWidget(findPathButton);

    buttonLayout->addWidget(generateVerticesButton);
    buttonLayout->addWidget(generateEdgesButton);

    mainLayout->addWidget(groupBox);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    connect(generateVerticesButton, &QPushButton::clicked, this, &ControlsWidget::onGenerateVerticesButtonClick);
    connect(generateEdgesButton, &QPushButton::clicked, this, &ControlsWidget::onGenerateEdgesButtonClick);
    connect(findPathButton, &QPushButton::clicked, this, &ControlsWidget::onFindPathButtonClick);

}

void ControlsWidget::onFindPathButtonClick() {
    const int startVertex = startVertexSpinBox->value();
    const int endVertex = endVertexSpinBox->value();
    const int lowerBound = lowerBoundSpinBox->value();
    const int upperBound = upperBoundSpinBox->value();

    emit pathParametersChanged(startVertex, endVertex, lowerBound, upperBound);                                         // Отправляем сигнал с этими значениями

}