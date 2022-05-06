/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Quick Templates 2 module of the Qt Toolkit.
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

#ifndef QQUICKPOPUPPOSITIONER_P_P_H
#define QQUICKPOPUPPOSITIONER_P_P_H

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

#include <QtQuick/private/qquickitemchangelistener_p.h>

QT_BEGIN_NAMESPACE

class QQuickItem;
class QQuickPopup;

class QQuickPopupPositioner : public QQuickItemChangeListener
{
public:
    explicit QQuickPopupPositioner(QQuickPopup *popup);
    ~QQuickPopupPositioner();

    QQuickPopup *popup() const;

    QQuickItem *parentItem() const;
    void setParentItem(QQuickItem *parent);

    virtual void reposition();

protected:
    void itemGeometryChanged(QQuickItem *, QQuickGeometryChange, const QRectF &) override;
    void itemParentChanged(QQuickItem *, QQuickItem *parent) override;
    void itemChildRemoved(QQuickItem *, QQuickItem *child) override;

    void removeAncestorListeners(QQuickItem *item);
    void addAncestorListeners(QQuickItem *item);

    bool m_positioning = false;
    QQuickItem *m_parentItem = nullptr;
    QQuickPopup *m_popup = nullptr;
    qreal m_popupScale = 1.0;
};

QT_END_NAMESPACE

#endif // QQUICKPOPUPPOSITIONER_P_P_H
