/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtQuick module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
******************************************************************************/

#ifndef QQUICKDRAGHANDLER_H
#define QQUICKDRAGHANDLER_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qquickmultipointhandler_p.h"
#include "qquickdragaxis_p.h"

QT_BEGIN_NAMESPACE

class Q_QUICK_PRIVATE_EXPORT QQuickDragHandler : public QQuickMultiPointHandler
{
    Q_OBJECT
    Q_PROPERTY(QQuickDragAxis * xAxis READ xAxis CONSTANT)
    Q_PROPERTY(QQuickDragAxis * yAxis READ yAxis CONSTANT)
    Q_PROPERTY(QVector2D translation READ translation NOTIFY translationChanged)
    Q_PROPERTY(QVector2D activeTranslation READ activeTranslation NOTIFY translationChanged REVISION(6, 2))
    Q_PROPERTY(QVector2D persistentTranslation READ persistentTranslation WRITE setPersistentTranslation NOTIFY translationChanged REVISION(6, 2))
    Q_PROPERTY(SnapMode snapMode READ snapMode WRITE setSnapMode NOTIFY snapModeChanged REVISION(2, 14))
    QML_NAMED_ELEMENT(DragHandler)
    QML_ADDED_IN_VERSION(2, 12)

public:
    enum SnapMode {
        NoSnap = 0,
        SnapAuto,
        SnapIfPressedOutsideTarget,
        SnapAlways
    };
    Q_ENUM(SnapMode)

    explicit QQuickDragHandler(QQuickItem *parent = nullptr);

    void handlePointerEventImpl(QPointerEvent *event) override;

    QQuickDragAxis *xAxis() { return &m_xAxis; }
    QQuickDragAxis *yAxis() { return &m_yAxis; }

    QVector2D translation() const { return m_activeTranslation; }
    QVector2D activeTranslation() const { return m_activeTranslation; }
    void setActiveTranslation(const QVector2D &trans);
    QVector2D persistentTranslation() const { return m_persistentTranslation; }
    void setPersistentTranslation(const QVector2D &trans);
    QQuickDragHandler::SnapMode snapMode() const;
    void setSnapMode(QQuickDragHandler::SnapMode mode);

    void enforceConstraints();

Q_SIGNALS:
    void translationChanged();
    Q_REVISION(2, 14) void snapModeChanged();

protected:
    bool wantsPointerEvent(QPointerEvent *event) override;
    void onActiveChanged() override;
    void onGrabChanged(QQuickPointerHandler *grabber, QPointingDevice::GrabTransition transition, QPointerEvent *event, QEventPoint &point) override;

private:
    void ungrab();
    void enforceAxisConstraints(QPointF *localPos);
    QPointF targetCentroidPosition();

private:
    QPointF m_pressTargetPos;   // We must also store the local targetPos, because we cannot deduce
                                // the press target pos from the scene pos in case there was e.g a
                                // flick in one of the ancestors during the drag.
    QVector2D m_activeTranslation;
    QVector2D m_persistentTranslation;
    QVector2D m_startTranslation;

    QQuickDragAxis m_xAxis;
    QQuickDragAxis m_yAxis;
    QQuickDragHandler::SnapMode m_snapMode = SnapAuto;
    bool m_pressedInsideTarget = false;

    friend class QQuickDragAxis;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QQuickDragHandler)
QML_DECLARE_TYPE(QQuickDragAxis)

#endif // QQUICKDRAGHANDLER_H
