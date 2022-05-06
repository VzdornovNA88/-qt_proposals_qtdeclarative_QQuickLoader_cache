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

#ifndef QQUICKRADIODELEGATE_P_H
#define QQUICKRADIODELEGATE_P_H

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

#include <QtQuickTemplates2/private/qquickitemdelegate_p.h>

QT_BEGIN_NAMESPACE

class QQuickRadioDelegatePrivate;

class Q_QUICKTEMPLATES2_PRIVATE_EXPORT QQuickRadioDelegate : public QQuickItemDelegate
{
    Q_OBJECT
    QML_NAMED_ELEMENT(RadioDelegate)
    QML_ADDED_IN_VERSION(2, 0)

public:
    explicit QQuickRadioDelegate(QQuickItem *parent = nullptr);

protected:
    QFont defaultFont() const override;

#if QT_CONFIG(accessibility)
    QAccessible::Role accessibleRole() const override;
#endif

private:
    Q_DECLARE_PRIVATE(QQuickRadioDelegate)
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QQuickRadioDelegate)

#endif // QQUICKRADIODELEGATE_P_H
