#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

namespace Ui
{
    class ChartBuilder;
}

class ChartBuilder : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChartBuilder(QWidget *parent = nullptr);
    void set_x_name(std::string x_name);
    void set_y_name(std::string y_name);
    void addNewGraphWithDeviation(const std::vector<double> &x, const std::vector<double> &y,
                     const std::vector<double> &deviation, const QString &name);
    void saveGraphPng(uint32_t);
    void removeAllGraphs();
    void resetColors();
    ~ChartBuilder();

    void addNewXYGraph(const std::vector<double> &x, const std::vector<double> &y, const QString &name);
    void saveGraphPng(QString name);

private:
    QColor generate_color();
    static const std::vector<QColor> colors;
    uint32_t cntColor = 0;
    Ui::ChartBuilder *ui;
    QCPTextElement *title;
};

#endif // MAINWINDOW_H
