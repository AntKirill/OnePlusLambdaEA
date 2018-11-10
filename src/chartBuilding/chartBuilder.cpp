#include "chartBuilder.h"
#include "ui_mainwindow.h"
#include <algorithm>
#include <memory>

using uint = uint32_t;

const std::vector<QColor> ChartBuilder::colors = {
    QColor(255, 165, 0), QColor(21, 171, 0), QColor(255, 0, 255), Qt::blue, Qt::green, Qt::red};

ChartBuilder::ChartBuilder(QWidget *parent) : QMainWindow(parent), ui(new Ui::ChartBuilder)
{
    srand(QDateTime::currentDateTime().toTime_t());
    ui->setupUi(this);

    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->customPlot->plotLayout()->insertRow(0);
    title = new QCPTextElement(ui->customPlot, "Testing oneMax function",
                               QFont("sans", 17, QFont::Bold));
    ui->customPlot->plotLayout()->addElement(0, 0, title);
    ui->customPlot->xAxis->setLabel("n");
    ui->customPlot->yAxis->setLabel("number of fintess function evaluations");
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft | Qt::AlignTop);
    ui->customPlot->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->customPlot->legend->setFont(legendFont);
    ui->customPlot->legend->setSelectedFont(legendFont);
}

void ChartBuilder::set_x_name(std::string x_name)
{
    ui->customPlot->xAxis->setLabel(x_name.c_str());
}

void ChartBuilder::set_y_name(std::string y_name)
{
    ui->customPlot->yAxis->setLabel(y_name.c_str());
}

ChartBuilder::~ChartBuilder()
{
    delete ui;
    delete title;
}

QColor ChartBuilder::generate_color()
{
    QColor color;
    if (cntColor >= colors.size())
        color = QColor(rand() % 245 + 10, rand() % 245 + 10, rand() % 245 + 10);
    else
        color = colors[cntColor++];
    return color;
}

void ChartBuilder::addNewXYGraph(const std::vector<double> &x, const std::vector<double> &y, const QString &name)
{
    auto qx = QVector<double>::fromStdVector(x);
    auto qy = QVector<double>::fromStdVector(y);
    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName(name);
    ui->customPlot->graph()->setData(qx, qy);
    ui->customPlot->rescaleAxes();

    QPen graphPen;
    QColor color = QColor(21, 171, 0);
    graphPen.setColor(color);
    graphPen.setWidthF(1.8);
    ui->customPlot->graph()->setPen(graphPen);
}

void ChartBuilder::addNewGraphWithDeviation(const std::vector<double> &x,
                                            const std::vector<double> &y,
                                            const std::vector<double> &deviation,
                                            const QString &name)
{
    // add <x, y>
    auto qx = QVector<double>::fromStdVector(x);
    auto qy = QVector<double>::fromStdVector(y);
    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName(name);
    ui->customPlot->graph()->setData(qx, qy);
    ui->customPlot->rescaleAxes();

    QPen graphPen;
    QColor color = generate_color();
    graphPen.setColor(color);
    graphPen.setWidthF(1.8);
    ui->customPlot->graph()->setPen(graphPen);

    // add deviation for every y
    auto qd = QVector<double>::fromStdVector(deviation);
    QCPErrorBars *errorBars = new QCPErrorBars(ui->customPlot->xAxis, ui->customPlot->yAxis);

    errorBars->removeFromLegend();
    errorBars->setAntialiased(false);
    errorBars->setDataPlottable(ui->customPlot->graph());
    errorBars->setPen(QPen(color));
    errorBars->setData(qd);

    ui->customPlot->replot();
}

void ChartBuilder::saveGraphPng(QString name)
{
    if (name.at(name.size() - 4) != '.') {
        name += ".png";
    }
    ui->customPlot->rescaleAxes();
    auto xRange = ui->customPlot->xAxis->range();
    xRange.upper *= 1.05;
    xRange.lower /= 1.05;
    auto yRange = ui->customPlot->yAxis->range();
    yRange.upper *= 1.05;
    yRange.lower /= 1.05;
    ui->customPlot->xAxis->setRange(xRange);
    ui->customPlot->yAxis->setRange(yRange);
    ui->customPlot->replot();
    ui->customPlot->savePng(name, 1920, 1080);
}

void ChartBuilder::saveGraphPng(uint32_t lambda)
{
    QString name = QString("lambda=") + QString::number(lambda) + QString(".png");
    saveGraphPng(name);
}

void ChartBuilder::removeAllGraphs()
{
    ui->customPlot->clearGraphs();
    ui->customPlot->replot();
}

void ChartBuilder::resetColors() { cntColor = 0; }
