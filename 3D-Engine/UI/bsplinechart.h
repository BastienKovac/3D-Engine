#ifndef BSPLINECHART_H
#define BSPLINECHART_H

#include <QChartView>
#include <QLineSeries>
#include <QScatterSeries>
#include <QGestureEvent>

#include <optional>
#include <memory>
#include <algorithm>

#include <Spline/bspline.h>

class BSplineChartView : public QtCharts::QChartView
{

    Q_OBJECT

public:
    BSplineChartView(QWidget *parent = nullptr);
    virtual ~BSplineChartView(){};

    void refreshBSpline();

    BSpline*spline() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:

    std::unique_ptr<BSpline> _spline;

    QtCharts::QChart * _chart;

    QtCharts::QScatterSeries * _controlPointsSeries;
    QtCharts::QLineSeries * _splineSeries;
    QtCharts::QLineSeries * _controlPolygonSeries;

    void displayControlPoints();

    // Handling of point drag
    bool _drag = false;
    QPointF _draggedPoint;
    std::optional<QPointF> getStoredPointFromPosition(QMouseEvent *event, float tolerance=0.1);

    QPointF getEventValue(QMouseEvent *event);
};

#endif // BSPLINECHART_H
