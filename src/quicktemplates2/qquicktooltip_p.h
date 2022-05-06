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

#ifndef QQUICKTOOLTIP_P_H
#define QQUICKTOOLTIP_P_H

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

#include <QtQuickTemplates2/private/qquickpopup_p.h>

QT_BEGIN_NAMESPACE

class QQuickToolTipPrivate;
class QQuickToolTipAttached;
class QQuickToolTipAttachedPrivate;

class Q_QUICKTEMPLATES2_PRIVATE_EXPORT QQuickToolTip : public QQuickPopup
{
    Q_OBJECT
    Q_PROPERTY(int delay READ delay WRITE setDelay NOTIFY delayChanged FINAL)
    Q_PROPERTY(int timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged FINAL)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged FINAL)
    QML_NAMED_ELEMENT(ToolTip)
    QML_ATTACHED(QQuickToolTipAttached)
    QML_ADDED_IN_VERSION(2, 0)

public:
    explicit QQuickToolTip(QQuickItem *parent = nullptr);

    QString text() const;
    void setText(const QString &text);

    int delay() const;
    void setDelay(int delay);

    int timeout() const;
    void setTimeout(int timeout);

    void setVisible(bool visible) override;

    static QQuickToolTipAttached *qmlAttachedProperties(QObject *object);

Q_SIGNALS:
    void textChanged();
    void delayChanged();
    void timeoutChanged();

public Q_SLOTS:
    Q_REVISION(2, 5) void show(const QString &text, int ms = -1);
    Q_REVISION(2, 5) void hide();

protected:
    QFont defaultFont() const override;

    void itemChange(QQuickItem::ItemChange change, const QQuickItem::ItemChangeData &data) override;
    void timerEvent(QTimerEvent *event) override;

#if QT_CONFIG(accessibility)
    QAccessible::Role accessibleRole() const override;
    void accessibilityActiveChanged(bool active) override;
#endif

private:
    Q_DISABLE_COPY(QQuickToolTip)
    Q_DECLARE_PRIVATE(QQuickToolTip)
};

class Q_QUICKTEMPLATES2_PRIVATE_EXPORT QQuickToolTipAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged FINAL)
    Q_PROPERTY(int delay READ delay WRITE setDelay NOTIFY delayChanged FINAL)
    Q_PROPERTY(int timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged FINAL)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged FINAL)
    Q_PROPERTY(QQuickToolTip *toolTip READ toolTip CONSTANT FINAL)

public:
    explicit QQuickToolTipAttached(QObject *parent = nullptr);

    QString text() const;
    void setText(const QString &text);

    int delay() const;
    void setDelay(int delay);

    int timeout() const;
    void setTimeout(int timeout);

    bool isVisible() const;
    void setVisible(bool visible);

    QQuickToolTip *toolTip() const;

Q_SIGNALS:
    void textChanged();
    void delayChanged();
    void timeoutChanged();
    void visibleChanged();

public Q_SLOTS:
    void show(const QString &text, int ms = -1);
    void hide();

private:
    Q_DISABLE_COPY(QQuickToolTipAttached)
    Q_DECLARE_PRIVATE(QQuickToolTipAttached)
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QQuickToolTip)
QML_DECLARE_TYPEINFO(QQuickToolTip, QML_HAS_ATTACHED_PROPERTIES)

#endif // QQUICKTOOLTIP_P_H
