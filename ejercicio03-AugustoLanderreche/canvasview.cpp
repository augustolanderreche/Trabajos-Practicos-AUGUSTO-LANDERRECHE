#include "canvasview.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QHash>
#include <QStringList>

CanvasView::CanvasView(CanvasModel *model, const QString &userId, QWidget *parent) :
    QWidget(parent),
    m_model(model),
    m_userId(userId),
    m_baseColor(192, 19, 76),
    m_penWidth(4),
    m_eraserWidth(6),
    m_drawing(false),
    m_tool(Pen),
    m_currentStroke(nullptr)
{
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
    setAttribute(Qt::WA_StaticContents);
    setAutoFillBackground(true);

    connect(m_model, &CanvasModel::modelChanged, this, &CanvasView::updateModel);
}

void CanvasView::setBaseColor(const QColor &color) {
    m_baseColor = color;
}

void CanvasView::setPenWidth(qreal width) {
    m_penWidth = qBound(1.0, width, 60.0);
}

void CanvasView::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);
    painter.setRenderHint(QPainter::Antialiasing);

    const QVector<Stroke> strokes = m_model->strokes();
    QHash<QString, QImage> userLayers;
    QStringList userOrder;

    for (const Stroke &stroke : strokes) {
        const QString layerKey = stroke.userId.isEmpty() ? QStringLiteral("__anonymous__") : stroke.userId;
        if (!userLayers.contains(layerKey)) {
            QImage layer(size(), QImage::Format_ARGB32_Premultiplied);
            layer.fill(Qt::transparent);
            userLayers.insert(layerKey, layer);
            userOrder.append(layerKey);
        }
    }

    for (const Stroke &s : strokes) {
        if (s.points.isEmpty()) continue;

        const QString layerKey = s.userId.isEmpty() ? QStringLiteral("__anonymous__") : s.userId;
        QPainter layerPainter(&userLayers[layerKey]);
        layerPainter.setRenderHint(QPainter::Antialiasing);

        if (s.eraser) {
            layerPainter.setCompositionMode(QPainter::CompositionMode_Clear);
        } else {
            layerPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        }

        if (s.points.size() == 1) {
            QPen pen;
            pen.setCapStyle(Qt::RoundCap);
            pen.setJoinStyle(Qt::RoundJoin);
            pen.setWidthF(s.points.first().width);
            pen.setColor(s.points.first().color);
            layerPainter.setPen(pen);
            layerPainter.drawPoint(s.points.first().pos);
            continue;
        }

        for (int i = 1; i < s.points.size(); ++i) {
            const StrokePoint &p0 = s.points.at(i - 1);
            const StrokePoint &p1 = s.points.at(i);

            QPen pen;
            pen.setCapStyle(Qt::RoundCap);
            pen.setJoinStyle(Qt::RoundJoin);
            pen.setWidthF((p0.width + p1.width) / 2.0);
            pen.setColor(p1.color);
            layerPainter.setPen(pen);

            layerPainter.drawLine(p0.pos, p1.pos);
        }
    }

    for (const QString &layerKey : userOrder) {
        painter.drawImage(QPoint(0, 0), userLayers.value(layerKey));
    }
}

void CanvasView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_drawing = true;
        m_tool = Pen;
    } else if (event->button() == Qt::RightButton) {
        m_drawing = true;
        m_tool = Eraser;
    }

    setFocus();

    if (m_drawing) {
        qreal width = (m_tool == Eraser) ? m_eraserWidth : m_penWidth;
        bool isEraser = (m_tool == Eraser);
        m_currentStroke = &m_model->startStroke(isEraser, m_baseColor, width, m_userId);
        m_model->addPointToStroke(*m_currentStroke, event->pos());
        update();
    }
}

void CanvasView::mouseMoveEvent(QMouseEvent *event) {
    if (!m_drawing || !m_currentStroke) return;
    m_model->addPointToStroke(*m_currentStroke, event->pos());
    update();
}

void CanvasView::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event)
    if (m_drawing && m_currentStroke) {
        m_model->finishStroke(*m_currentStroke);
    }

    m_currentStroke = nullptr;
    m_drawing = false;
}

void CanvasView::wheelEvent(QWheelEvent *event) {
    const int delta = event->angleDelta().y();
    if (delta == 0) return;

    if (m_tool == Eraser) {
        m_eraserWidth = qBound(1.0, m_eraserWidth + (delta > 0 ? 1.0 : -1.0), 120.0);
        setStatusTip(QString("Goma: %1").arg(m_eraserWidth));
    } else {
        m_penWidth = qBound(1.0, m_penWidth + (delta > 0 ? 1.0 : -1.0), 120.0);
        setStatusTip(QString("Pincel: %1").arg(m_penWidth));
    }
}

void CanvasView::keyPressEvent(QKeyEvent *event) {
    if (event->key() >= Qt::Key_1 && event->key() <= Qt::Key_9) {
        m_baseColor = colorForKey(event->key());
        setStatusTip(QString("Color %1: %2").arg(event->key() - Qt::Key_0).arg(m_baseColor.name()));
        update();
    }
}

void CanvasView::updateModel() {
    update();
}

QColor CanvasView::colorForKey(int key) {
    switch (key) {
    case Qt::Key_1: return QColor(255, 0, 0);      // rojo
    case Qt::Key_2: return QColor(0, 0, 255);      // azul
    case Qt::Key_3: return QColor(0, 0, 0);        // negro
    case Qt::Key_4: return QColor(128, 0, 128);    // violeta
    case Qt::Key_5: return QColor(255, 165, 0);    // naranja
    case Qt::Key_6: return QColor(0, 128, 0);      // verde
    case Qt::Key_7: return QColor(255, 255, 0);    // amarillo
    case Qt::Key_8: return QColor(255, 105, 180);  // rosa
    case Qt::Key_9: return QColor(190, 190, 190);  // gris suave
    default: return QColor(192, 19, 76);
    }
}
