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

#ifndef QQUICKITEMCHANGELISTENER_P_H
#define QQUICKITEMCHANGELISTENER_P_H

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

#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

class QRectF;
class QQuickItem;
class QQuickAnchorsPrivate;

class QQuickGeometryChange
{
public:
    enum Kind: int {
        Nothing = 0x00,
        X       = 0x01,
        Y       = 0x02,
        Width   = 0x04,
        Height  = 0x08,

        Size = Width | Height,
        All = X | Y | Size
    };

    QQuickGeometryChange(int change = Nothing)
        : kind(change)
    {}

    bool noChange() const { return kind == Nothing; }
    bool anyChange() const { return !noChange(); }

    bool xChange() const { return kind & X; }
    bool yChange() const { return kind & Y; }
    bool widthChange() const { return kind & Width; }
    bool heightChange() const { return kind & Height; }

    bool positionChange() const { return xChange() || yChange(); }
    bool sizeChange() const { return widthChange() || heightChange(); }

    bool horizontalChange() const { return xChange() || widthChange(); }
    bool verticalChange() const { return yChange() || heightChange(); }

    void setXChange(bool enabled) { set(X, enabled); }
    void setYChange(bool enabled) { set(Y, enabled); }
    void setWidthChange(bool enabled) { set(Width, enabled); }
    void setHeightChange(bool enabled) { set(Height, enabled); }
    void setSizeChange(bool enabled) { set(Size, enabled); }
    void setAllChanged(bool enabled) { set(All, enabled); }
    void setHorizontalChange(bool enabled) { set(X | Width, enabled); }
    void setVerticalChange(bool enabled) { set(Y | Height, enabled); }

    void set(int bits, bool enabled)
    {
        if (enabled) {
            kind |= bits;
        } else {
            kind &= ~bits;
        }
    }

    bool matches(QQuickGeometryChange other) const { return kind & other.kind; }

private:
    int kind;
};

#define QT_QUICK_NEW_GEOMETRY_CHANGED_HANDLING

class QQuickItemChangeListener
{
public:
    virtual ~QQuickItemChangeListener() {}

    virtual void itemGeometryChanged(QQuickItem *, QQuickGeometryChange, const QRectF & /* oldGeometry */) {}
    virtual void itemSiblingOrderChanged(QQuickItem *) {}
    virtual void itemVisibilityChanged(QQuickItem *) {}
    virtual void itemEnabledChanged(QQuickItem *) {}
    virtual void itemOpacityChanged(QQuickItem *) {}
    virtual void itemDestroyed(QQuickItem *) {}
    virtual void itemChildAdded(QQuickItem *, QQuickItem * /* child */ ) {}
    virtual void itemChildRemoved(QQuickItem *, QQuickItem * /* child */ ) {}
    virtual void itemParentChanged(QQuickItem *, QQuickItem * /* parent */ ) {}
    virtual void itemRotationChanged(QQuickItem *) {}
    virtual void itemImplicitWidthChanged(QQuickItem *) {}
    virtual void itemImplicitHeightChanged(QQuickItem *) {}
    virtual void itemFocusChanged(QQuickItem *, Qt::FocusReason /* reason */) {}

    virtual QQuickAnchorsPrivate *anchorPrivate() { return nullptr; }
};

QT_END_NAMESPACE

#endif // QQUICKITEMCHANGELISTENER_P_H
