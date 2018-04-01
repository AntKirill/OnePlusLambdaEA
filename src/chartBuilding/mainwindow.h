#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include "qcustomplot.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(std::pair<uint, uint> xRange, std::pair<uint, uint> yRange, QWidget *parent = nullptr);
    void addNewGraph(const std::vector<std::pair<uint32_t, uint32_t>> &gr, QColor color, QString name);
    void saveGraphPng(uint32_t lambda);
    void removeAllGraphs();
    ~MainWindow();

private slots:
    void titleDoubleClick(QMouseEvent *event);
    void axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part);
    void legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item);
    void selectionChanged();
    void mousePress();
    void mouseWheel();
    void addRandomGraph();
    void removeSelectedGraph();
    void contextMenuRequest(QPoint pos);
    void moveLegend();
    void graphClicked(QCPAbstractPlottable *plottable, int dataIndex);

private:
    Ui::MainWindow *ui;
    QCPTextElement *title;
    QCPLayoutElement *graphName;
};

#endif // MAINWINDOW_H
