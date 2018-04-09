#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void addNewGraph(const std::vector<double> &x, const std::vector<double> &y,
                     const std::vector<double> &deviation, const QString &name);
    void saveGraphPng(uint32_t);
    void removeAllGraphs();
    void resetColors();
    ~MainWindow();

private:
    static const std::vector<QColor> colors;
    uint32_t cntColor = 0;
    Ui::MainWindow *ui;
    QCPTextElement *title;
};

#endif // MAINWINDOW_H
