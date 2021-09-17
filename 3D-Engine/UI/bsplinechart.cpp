#include "bsplinechart.h"


BSplineChartView::BSplineChartView(QWidget *parent) : QChartView(parent)
{
    _chart = new QChart();
    _chart->legend()->hide();
    _chart->setTitle("B-Spline");

    _controlPointsSeries = new QScatterSeries();
    _controlPointsSeries->setMarkerShape(QScatterSeries::MarkerShape::MarkerShapeRectangle);
    _chart->addSeries(_controlPointsSeries);

    _splineSeries = new QLineSeries();
    _splineSeries->setUseOpenGL(true);
    _chart->addSeries(_splineSeries);

    _controlPolygonSeries = new QLineSeries();
    _controlPolygonSeries->setBrush(QBrush("blue"));
    _controlPolygonSeries->setPen(QPen(Qt::PenStyle::DashLine));
    _controlPolygonSeries->setUseOpenGL(true);
    _chart->addSeries(_controlPolygonSeries);

    setRubberBand(QChartView::RectangleRubberBand);
    setChart(_chart);

    // Init model bspline
    _spline = std::unique_ptr<BSpline>(new BSpline());

    refreshBSpline();
}

void BSplineChartView::displayControlPoints()
{
    _controlPointsSeries->clear();
    _controlPolygonSeries->clear();
    for (QPointF control : _spline->controlPoints())
    {
        _controlPointsSeries->append(control);
        _controlPolygonSeries->append(control);
    }
}

void BSplineChartView::refreshBSpline()
{
    // 1- Display control points and polygon
    displayControlPoints();

    // 2- Display computed B-Spline
    _splineSeries->clear();
    std::vector<QPointF> bspline = _spline->computedPoints();
    for (QPointF value : bspline)
    {
        _splineSeries->append(value);
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
            _spline->removeControlPoint(storedPoint.value());
        }
    }
    else
    {
        if (event->button() == Qt::MouseButton::LeftButton)
        {
            // Add new control point at click position
            _spline->addControlPoint(getEventValue(event));
        }
    }

    refreshBSpline();
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
        _spline->replaceControlPoint(tmp, _draggedPoint);

        refreshBSpline();
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
        _chart->scroll(-5, 0);
        break;
    case Qt::Key_Right:
        _chart->scroll(5, 0);
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

BSpline*BSplineChartView::spline() const
{
    return _spline.get();
}

QPointF BSplineChartView::getEventValue(QMouseEvent *event)
{
    QPointF eventPosition = event->position();
    QPointF chartPosition = _chart->mapFromScene(eventPosition);
    return _chart->mapToValue(chartPosition);
}

std::optional<QPointF> BSplineChartView::getStoredPointFromPosition(QMouseEvent *event, float tolerance)
{
    QPointF eventValue = getEventValue(event);

    for (QPointF storedPoint : _spline->controlPoints())
    {
        if (storedPoint.x() > eventValue.x() + eventValue.x() * tolerance || storedPoint.x() < eventValue.x() - eventValue.x() * tolerance ||
            storedPoint.y() > eventValue.y() + eventValue.y() * tolerance || storedPoint.y() < eventValue.y() - eventValue.y() * tolerance)
        {
            continue;
        }
        return storedPoint;
    }

    return {};
}
