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

#include "qquickitemdelegate_p.h"
#include "qquickitemdelegate_p_p.h"
#include "qquickcontrol_p_p.h"

#include <QtGui/qpa/qplatformtheme.h>

QT_BEGIN_NAMESPACE

/*!
    \qmltype ItemDelegate
    \inherits AbstractButton
//!     \instantiates QQuickItemDelegate
    \inqmlmodule QtQuick.Controls
    \since 5.7
    \ingroup qtquickcontrols2-delegates
    \brief Basic item delegate that can be used in various views and controls.

    \image qtquickcontrols2-itemdelegate.gif

    ItemDelegate presents a standard view item. It can be used as a delegate
    in various views and controls, such as \l ListView and \l ComboBox.

    ItemDelegate inherits its API from AbstractButton. For instance, you can set
    \l {AbstractButton::text}{text}, display an \l {Icons in Qt Quick Controls}{icon},
    and react to \l {AbstractButton::clicked}{clicks} using the AbstractButton API.

    \snippet qtquickcontrols2-itemdelegate.qml 1

    \sa {Customizing ItemDelegate}, {Delegate Controls}
*/

QQuickItemDelegate::QQuickItemDelegate(QQuickItem *parent)
    : QQuickAbstractButton(*(new QQuickItemDelegatePrivate), parent)
{
    setFocusPolicy(Qt::NoFocus);
}

QQuickItemDelegate::QQuickItemDelegate(QQuickItemDelegatePrivate &dd, QQuickItem *parent)
    : QQuickAbstractButton(dd, parent)
{
    setFocusPolicy(Qt::NoFocus);
}

/*!
    \qmlproperty bool QtQuick.Controls::ItemDelegate::highlighted

    This property holds whether the delegate is highlighted.

    A delegate can be highlighted in order to draw the user's attention towards
    it. It has no effect on keyboard interaction. For example, you can
    highlight the current item in a ListView using the following code:

    \code
    ListView {
        id: listView
        model: 10
        delegate: ItemDelegate {
            text: modelData
            highlighted: ListView.isCurrentItem
            onClicked: listView.currentIndex = index
        }
    }
    \endcode

    The default value is \c false.
*/
bool QQuickItemDelegate::isHighlighted() const
{
    Q_D(const QQuickItemDelegate);
    return d->highlighted;
}

void QQuickItemDelegate::setHighlighted(bool highlighted)
{
    Q_D(QQuickItemDelegate);
    if (highlighted == d->highlighted)
        return;

    d->highlighted = highlighted;
    emit highlightedChanged();
}

QFont QQuickItemDelegate::defaultFont() const
{
    return QQuickTheme::font(QQuickTheme::ItemView);
}

#if QT_CONFIG(accessibility)
QAccessible::Role QQuickItemDelegate::accessibleRole() const
{
    return QAccessible::ListItem;
}
#endif

QPalette QQuickItemDelegatePrivate::defaultPalette() const
{
    return QQuickTheme::palette(QQuickTheme::ItemView);
}

QT_END_NAMESPACE
