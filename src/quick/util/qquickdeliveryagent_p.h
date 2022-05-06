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

#ifndef QQUICKDELIVERYAGENT_P_H
#define QQUICKDELIVERYAGENT_P_H

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

#include <QtQuick/private/qtquickglobal_p.h>

#include <QtQml/qqml.h>
#include <QtQml/private/qqmlglobal_p.h>

#include <QtCore/qobject.h>
#include <QtGui/qevent.h>

QT_BEGIN_NAMESPACE

class QQuickItem;
class QQuickDeliveryAgentPrivate;

class Q_QUICK_EXPORT QQuickDeliveryAgent : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QQuickDeliveryAgent)

public:
    struct Q_QUICK_EXPORT Transform
    {
        virtual ~Transform();
        virtual QPointF map(const QPointF &point) = 0;
    };

    explicit QQuickDeliveryAgent(QQuickItem *rootItem);
    virtual ~QQuickDeliveryAgent();

    QQuickItem *rootItem() const;

    Transform *sceneTransform() const;
    void setSceneTransform(Transform *transform);
    bool event(QEvent *ev) override;

Q_SIGNALS:

private:
    Q_DISABLE_COPY(QQuickDeliveryAgent)
};

#ifndef QT_NO_DEBUG_STREAM
QDebug Q_QUICK_EXPORT operator<<(QDebug debug, const QQuickDeliveryAgent *da);
#endif

QT_END_NAMESPACE

#endif // QQUICKDELIVERYAGENT_P_H
