/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Quick Controls 2 module of the Qt Toolkit.
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
******************************************************************************/

#ifndef QQUICKMACFOCUSFRAME_H
#define QQUICKMACFOCUSFRAME_H

#include <QtQuick/qquickitem.h>
#include <QtQuick/private/qquicktextedit_p.h>
#include "qquickstyleitem.h"

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(lcFocusFrame)

struct QQuickFocusFrameDescription {
    QQuickItem *target;
    QQuickStyleMargins margins;
    const qreal radius = 3;
    bool isValid() const { return target != nullptr; }
    static QQuickFocusFrameDescription Invalid;
};

class QQuickMacFocusFrame : public  QObject
{
    Q_OBJECT

public:
    QQuickMacFocusFrame();

private:
    static QScopedPointer<QQuickItem> m_focusFrame;

    void createFocusFrame(QQmlContext *context);
    void moveToItem(QQuickItem *item);
    QQuickFocusFrameDescription getDescriptionForItem(QQuickItem *focusItem) const;
};

QT_END_NAMESPACE

#endif // QQUICKMACFOCUSFRAME_H
