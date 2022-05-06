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

#ifndef QQUICKSTYLEDTEXT_H
#define QQUICKSTYLEDTEXT_H

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

#include <QSize>
#include <QPointF>
#include <QList>
#include <QUrl>
#include <QtQuick/private/qquickpixmapcache_p.h>

QT_BEGIN_NAMESPACE

class QQuickStyledTextImgTag;
class QQuickStyledTextPrivate;
class QString;
class QQmlContext;

class Q_AUTOTEST_EXPORT QQuickStyledTextImgTag
{
public:
    QQuickStyledTextImgTag() { }

    ~QQuickStyledTextImgTag() { delete pix; }

    enum Align {
        Bottom,
        Middle,
        Top
    };

    QUrl url;
    QPointF pos;
    QSize size;
    int position = 0;
    qreal offset = 0.0; // this offset allows us to compensate for flooring reserved space
    Align align = QQuickStyledTextImgTag::Bottom;
    QQuickPixmap *pix = nullptr;
};

class Q_AUTOTEST_EXPORT QQuickStyledText
{
public:
    static void parse(const QString &string, QTextLayout &layout,
                      QList<QQuickStyledTextImgTag*> &imgTags,
                      const QUrl &baseUrl,
                      QQmlContext *context,
                      bool preloadImages,
                      bool *fontSizeModified);

private:
    QQuickStyledText(const QString &string, QTextLayout &layout,
                           QList<QQuickStyledTextImgTag*> &imgTags,
                           const QUrl &baseUrl,
                           QQmlContext *context,
                           bool preloadImages,
                           bool *fontSizeModified);
    ~QQuickStyledText();

    QQuickStyledTextPrivate *d;
};

QT_END_NAMESPACE

#endif
