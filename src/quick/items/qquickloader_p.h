// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QQUICKLOADER_P_H
#define QQUICKLOADER_P_H

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

#include "qquickimplicitsizeitem_p.h"

QT_BEGIN_NAMESPACE

class QQuickLoaderPrivate;
class QQmlV4Function;
class QQuickLoaderAttached;
class Q_QUICK_PRIVATE_EXPORT QQuickLoader : public QQuickImplicitSizeItem
{
    Q_OBJECT

    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QQmlComponent *sourceComponent READ sourceComponent WRITE setSourceComponent RESET resetSourceComponent NOTIFY sourceComponentChanged)
    Q_PROPERTY(QObject *item READ item NOTIFY itemChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(bool asynchronous READ asynchronous WRITE setAsynchronous NOTIFY asynchronousChanged)
    QML_NAMED_ELEMENT(Loader)
    QML_ADDED_IN_VERSION(2, 0)
    QML_ATTACHED(QQuickLoaderAttached)

public:
    QQuickLoader(QQuickItem *parent = nullptr);
    virtual ~QQuickLoader();

    bool active() const;
    void setActive(bool newVal);

    Q_INVOKABLE void setSource(QQmlV4Function *);

    QUrl source() const;
    void setSource(const QUrl &);

    QQmlComponent *sourceComponent() const;
    void setSourceComponent(QQmlComponent *);
    void resetSourceComponent();

    enum Status { Null, Ready, Loading, Error };
    Q_ENUM(Status)
    Status status() const;
    qreal progress() const;

    bool asynchronous() const;
    void setAsynchronous(bool a);

    QObject *item() const;

    static QQuickLoaderAttached *qmlAttachedProperties(QObject *);

Q_SIGNALS:
    void itemChanged();
    void activeChanged();
    void sourceChanged();
    void sourceComponentChanged();
    void statusChanged();
    void progressChanged();
    void loaded();
    void asynchronousChanged();

protected:
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void componentComplete() override;
    void itemChange(ItemChange change, const ItemChangeData &value) override;

private:
    void setSource(const QUrl &sourceUrl, bool needsClear);
    void loadFromSource();
    void loadFromSourceComponent();
    friend class QQuickLoaderAttached;
    Q_DISABLE_COPY(QQuickLoader)
    Q_DECLARE_PRIVATE(QQuickLoader)
    Q_PRIVATE_SLOT(d_func(), void _q_sourceLoaded())
    Q_PRIVATE_SLOT(d_func(), void _q_updateSize())
};

class QQmlOpenMetaObject;
class QQuickLoaderAttached : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isCacheable READ isCacheable WRITE isCacheable NOTIFY cacheableChanged)

public:
    QQuickLoaderAttached(QObject *parent);
    ~QQuickLoaderAttached();

    bool isCacheable() const { return m_isCacheable; }
    void isCacheable(bool st) {
        if (m_isCacheable != st) {
            m_isCacheable = st;
            Q_EMIT cacheableChanged();
        }
    }

Q_SIGNALS:
    void cacheableChanged();

private:
    bool m_isCacheable : 1;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QQuickLoader)

#endif // QQUICKLOADER_P_H
