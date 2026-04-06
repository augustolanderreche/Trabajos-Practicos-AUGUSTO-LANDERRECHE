#include "canvasmodel.h"
#include <QPainter>
#include <QPainterPath>

CanvasModel::CanvasModel(QObject *parent) : QObject(parent) {}

QVector<Stroke> CanvasModel::strokes() const {
    return m_strokes;
}

void CanvasModel::clear() {
    m_strokes.clear();
    emit modelChanged();
}

Stroke &CanvasModel::startStroke(bool eraser, const QColor &baseColor, qreal width) {
    Q_UNUSED(baseColor)
    Q_UNUSED(width)

    Stroke s;
    s.id = QUuid::createUuid();
    s.eraser = eraser;
    s.width = width;
    s.color = baseColor;
    m_strokes.append(s);
    emit modelChanged();
    return m_strokes.last();
}

void CanvasModel::addPointToStroke(Stroke &stroke, const QPointF &pt) {
    StrokePoint p;
    p.pos = pt;
    p.width = stroke.width;

    if (stroke.eraser) {
        p.color = QColor(Qt::white);
    } else {
        // Mantener color del trazo seleccionado (1-9) de forma consistente
        p.color = stroke.color;
    }

    stroke.points.append(p);
}

void CanvasModel::finishStroke(const Stroke &stroke) {
    Q_UNUSED(stroke)
    emit modelChanged();
}

void CanvasModel::mergeFrom(const CanvasModel &other) {
    QSet<QUuid> existing;
    for (const Stroke &s : m_strokes) {
        existing.insert(s.id);
    }
    bool changed = false;
    for (const Stroke &s : other.strokes()) {
        if (!existing.contains(s.id)) {
            m_strokes.append(s);
            changed = true;
        }
    }
    if (changed) emit modelChanged();
}

QJsonObject CanvasModel::toJson() const {
    QJsonObject root;
    QJsonArray arr;
    for (const Stroke &s : m_strokes) {
        QJsonObject so;
        so["id"] = s.id.toString();
        so["eraser"] = s.eraser;
        QJsonArray pa;
        for (const StrokePoint &p : s.points) {
            QJsonObject po;
            po["x"] = p.pos.x();
            po["y"] = p.pos.y();
            po["width"] = p.width;
            po["color"] = p.color.name();
            pa.append(po);
        }
        so["points"] = pa;
        arr.append(so);
    }
    root["strokes"] = arr;
    return root;
}

bool CanvasModel::fromJson(const QJsonObject &obj) {
    if (!obj.contains("strokes")) return false;
    QJsonArray arr = obj["strokes"].toArray();
    QVector<Stroke> newStrokes;
    for (const QJsonValue &v : arr) {
        if (!v.isObject()) continue;
        QJsonObject so = v.toObject();
        Stroke s;
        s.id = QUuid(so["id"].toString());
        s.eraser = so["eraser"].toBool();
        QJsonArray pa = so["points"].toArray();
        for (const QJsonValue &pv : pa) {
            if (!pv.isObject()) continue;
            QJsonObject po = pv.toObject();
            StrokePoint p;
            p.pos = QPointF(po["x"].toDouble(), po["y"].toDouble());
            p.width = po["width"].toDouble();
            p.color = QColor(po["color"].toString());
            s.points.append(p);
        }
        newStrokes.append(s);
    }
    if (!newStrokes.isEmpty()) {
        m_strokes = newStrokes;
        emit modelChanged();
        return true;
    }
    return false;
}

QImage CanvasModel::renderImage(const QSize &size, const QColor &bg) const {
    QImage img(size, QImage::Format_ARGB32_Premultiplied);
    img.fill(bg);
    QPainter painter(&img);
    painter.setRenderHint(QPainter::Antialiasing);

    for (const Stroke &s : m_strokes) {
        if (s.points.isEmpty()) continue;

        QPainterPath path;
        path.moveTo(s.points.first().pos);

        for (int i = 1; i < s.points.size(); ++i) {
            QPointF p0 = s.points[i-1].pos;
            QPointF p1 = s.points[i].pos;
            QPointF mid = (p0 + p1) / 2.0;
            path.quadTo(p0, mid);
        }

        QPen pen;
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        pen.setWidthF(s.points.last().width);

        if (s.eraser) {
            pen.setColor(bg);
            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        } else {
            pen.setColor(s.points.last().color);
            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        }

        painter.setPen(pen);
        painter.drawPath(path);
    }

    return img;
}
