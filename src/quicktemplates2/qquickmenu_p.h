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

#ifndef QQUICKMENU_P_H
#define QQUICKMENU_P_H

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

#include <QtQml/qqmllist.h>
#include <QtQml/qqml.h>

#include "qquickpopup_p.h"

QT_BEGIN_NAMESPACE

class QQuickAction;
class QQmlComponent;
class QQuickMenuItem;
class QQuickMenuPrivate;

class Q_QUICKTEMPLATES2_PRIVATE_EXPORT QQuickMenu : public QQuickPopup
{
    Q_OBJECT
    Q_PROPERTY(QVariant contentModel READ contentModel CONSTANT FINAL)
    Q_PROPERTY(QQmlListProperty<QObject> contentData READ contentData FINAL)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
    // 2.3 (Qt 5.10)
    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL REVISION(2, 3))
    Q_PROPERTY(bool cascade READ cascade WRITE setCascade RESET resetCascade NOTIFY cascadeChanged FINAL REVISION(2, 3))
    Q_PROPERTY(qreal overlap READ overlap WRITE setOverlap NOTIFY overlapChanged FINAL REVISION(2, 3))
    Q_PROPERTY(QQmlComponent *delegate READ delegate WRITE setDelegate NOTIFY delegateChanged FINAL REVISION(2, 3))
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged FINAL REVISION(2, 3))
    Q_CLASSINFO("DefaultProperty", "contentData")
    QML_NAMED_ELEMENT(Menu)
    QML_ADDED_IN_VERSION(2, 0)

public:
    explicit QQuickMenu(QObject *parent = nullptr);
    ~QQuickMenu();

    Q_INVOKABLE QQuickItem *itemAt(int index) const;
    Q_INVOKABLE void addItem(QQuickItem *item);
    Q_INVOKABLE void insertItem(int index, QQuickItem *item);
    Q_INVOKABLE void moveItem(int from, int to);
    Q_INVOKABLE void removeItem(QQuickItem *item);

    QVariant contentModel() const;
    QQmlListProperty<QObject> contentData();

    QString title() const;
    void setTitle(QString &title);

    bool cascade() const;
    void setCascade(bool cascade);
    void resetCascade();

    qreal overlap() const;
    void setOverlap(qreal overlap);

    QQmlComponent *delegate() const;
    void setDelegate(QQmlComponent *delegate);

    int currentIndex() const;
    void setCurrentIndex(int index);

    // 2.3 (Qt 5.10)
    int count() const;
    Q_REVISION(2, 3) Q_INVOKABLE QQuickItem *takeItem(int index);

    Q_REVISION(2, 3) Q_INVOKABLE QQuickMenu *menuAt(int index) const;
    Q_REVISION(2, 3) Q_INVOKABLE void addMenu(QQuickMenu *menu);
    Q_REVISION(2, 3) Q_INVOKABLE void insertMenu(int index, QQuickMenu *menu);
    Q_REVISION(2, 3) Q_INVOKABLE void removeMenu(QQuickMenu *menu);
    Q_REVISION(2, 3) Q_INVOKABLE QQuickMenu *takeMenu(int index);

    Q_REVISION(2, 3) Q_INVOKABLE QQuickAction *actionAt(int index) const;
    Q_REVISION(2, 3) Q_INVOKABLE void addAction(QQuickAction *action);
    Q_REVISION(2, 3) Q_INVOKABLE void insertAction(int index, QQuickAction *action);
    Q_REVISION(2, 3) Q_INVOKABLE void removeAction(QQuickAction *action);
    Q_REVISION(2, 3) Q_INVOKABLE QQuickAction *takeAction(int index);

    void popup(QQuickItem *menuItem = nullptr);
    void popup(const QPointF &pos, QQuickItem *menuItem = nullptr);

    Q_REVISION(2, 3) Q_INVOKABLE void popup(QQmlV4Function *args);
    Q_REVISION(2, 3) Q_INVOKABLE void dismiss();

protected:
    void componentComplete() override;
    void contentItemChange(QQuickItem *newItem, QQuickItem *oldItem) override;
    void itemChange(QQuickItem::ItemChange change, const QQuickItem::ItemChangeData &data) override;
    void keyPressEvent(QKeyEvent *event) override;

Q_SIGNALS:
    void titleChanged(const QString &title);
    // 2.3 (Qt 5.10)
    Q_REVISION(2, 3) void countChanged();
    Q_REVISION(2, 3) void cascadeChanged(bool cascade);
    Q_REVISION(2, 3) void overlapChanged();
    Q_REVISION(2, 3) void delegateChanged();
    Q_REVISION(2, 3) void currentIndexChanged();

protected:
    void timerEvent(QTimerEvent *event) override;

    QFont defaultFont() const override;

#if QT_CONFIG(accessibility)
    QAccessible::Role accessibleRole() const override;
#endif

private:
    Q_DISABLE_COPY(QQuickMenu)
    Q_DECLARE_PRIVATE(QQuickMenu)
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QQuickMenu)

#endif // QQUICKMENU_P_H
