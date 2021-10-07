#include "bsplinechart.h"

#include <QVector3D>

BSplineChartView::BSplineChartView(QWidget *parent) : QChartView(parent)
{
    _chart = new QtCharts::QChart();
    _chart->legend()->hide();
    _chart->setTitle("B-Spline");

    _controlPointsSeries = new QtCharts::QScatterSeries();
    _controlPointsSeries->setMarkerShape(QtCharts::QScatterSeries::MarkerShape::MarkerShapeRectangle);
    _chart->addSeries(_controlPointsSeries);

    _splineSeries = new QtCharts::QLineSeries();
    _splineSeries->setUseOpenGL(true);
    _chart->addSeries(_splineSeries);

    _controlPolygonSeries = new QtCharts::QLineSeries();
    _controlPolygonSeries->setBrush(QBrush("blue"));
    _controlPolygonSeries->setPen(QPen(Qt::PenStyle::DashLine));
    _controlPolygonSeries->setUseOpenGL(true);
    _chart->addSeries(_controlPolygonSeries);

    setRubberBand(QChartView::RectangleRubberBand);
    setChart(_chart);

    // Init model bspline
    _spline = std::unique_ptr<BSpline>(new BSpline());
    connect(_spline.get(), SIGNAL(splineChanged()), this, SLOT(refreshBSpline()));
}

void BSplineChartView::displayControlPoints()
{
    _controlPointsSeries->clear();
    _controlPolygonSeries->clear();
    for (QVector3D control : _spline->controlPoints())
    {
        _controlPointsSeries->append(QPointF(control.x(), control.y()));
        _controlPolygonSeries->append(QPointF(control.x(), control.y()));
    }
}

void BSplineChartView::refreshBSpline()
{
    // 1- Display control points and polygon
    displayControlPoints();

    // 2- Display computed B-Spline
    _splineSeries->clear();
    std::vector<QVector3D> bspline = _spline->computedPoints();
    for (QVector3D value : bspline)
    {
        _splineSeries->append(QPointF(value.x(), value.y()));
    }

    _chart->createDefaultAxes();
}

void BSplineChartView::mousePressEvent(QMouseEvent *event)
{
    std::optional<QPointF> storedPoint = getStoredPointFromPosition(event);

    if (_drag)
    {
        // Ignore mouse events during drag
        return;
    }

    if (storedPoint.has_value())
    {
        if (event->button() == Qt::MouseButton::LeftButton)
        {
            // Start drag action with select control point
            _drag = true;
            _draggedPoint = storedPoint.value();
        }
        else if (event->button() == Qt::MouseButton::RightButton)
        {
            // Remove selected control point
            _spline->removeControlPoint(QVector3D(storedPoint->x(), storedPoint->y(), 0.));
        }
    }
    else
    {
        if (event->button() == Qt::MouseButton::LeftButton)
        {
            // Add new control point at click position
            QPointF clickPoint = getEventValue(event);
            _spline->addControlPoint(QVector3D(clickPoint.x(), clickPoint.y(), 0.));
        }
    }
}

void BSplineChartView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MouseButton::LeftButton && _drag)
    {
        // End drag
        _drag = false;
    }
}

void BSplineChartView::mouseMoveEvent(QMouseEvent *event)
{
    std::optional<QPointF> storedPoint = getStoredPointFromPosition(event);

    if (_drag)
    {
        setCursor(Qt::ClosedHandCursor);
    }
    else if (storedPoint.has_value())
    {
        setCursor(Qt::PointingHandCursor);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
    }

    if (_drag)
    {
        // Replace point by new drag position
        QPointF eventValue = getEventValue(event);
        QPointF tmp = _draggedPoint;

        _draggedPoint = eventValue;
        _spline->replaceControlPoint(QVector3D(tmp.x(), tmp.y(), 0.), QVector3D(_draggedPoint.x(), _draggedPoint.y(), 0.));
    }
}

void BSplineChartView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Plus:
        _chart->zoomIn();
        break;
    case Qt::Key_Minus:
        _chart->zoomOut();
        break;
    case Qt::Key_Left:
        _chart->scroll(5, 0);
        break;
    case Qt::Key_Right:
        _chart->scroll(-5, 0);
        break;
    case Qt::Key_Up:
        _chart->scroll(0, 5);
        break;
    case Qt::Key_Down:
        _chart->scroll(0, -5);
        break;
    default:
        QGraphicsView::keyPressEvent(event);
        break;
    }
}

std::shared_ptr<BSpline> BSplineChartView::spline() const
{
    return _spline;
}

QPointF BSplineChartView::getEventValue(QMouseEvent *event)
{
    QPointF eventPosition = event->pos();
    QPointF chartPosition = _chart->mapFromScene(eventPosition);
    return _chart->mapToValue(chartPosition);
}

std::optional<QPointF> BSplineChartView::getStoredPointFromPosition(QMouseEvent *event, float tolerance)
{
    QPointF eventValue = getEventValue(event);

    for (QVector3D storedPoint : _spline->controlPoints())
    {
        if (storedPoint.x() > eventValue.x() + eventValue.x() * tolerance || storedPoint.x() < eventValue.x() - eventValue.x() * tolerance ||
            storedPoint.y() > eventValue.y() + eventValue.y() * tolerance || storedPoint.y() < eventValue.y() - eventValue.y() * tolerance)
        {
            continue;
        }
        return QPointF(storedPoint.x(), storedPoint.y());
    }

    return {};
}
