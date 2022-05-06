/****************************************************************************
**
** Copyright (C) 2016 Research In Motion.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtQml module of the Qt Toolkit.
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

#ifndef QQMLABSTRACTURLINTERCEPTOR_H
#define QQMLABSTRACTURLINTERCEPTOR_H

#include <QtCore/qurl.h>
#include <QtQml/qtqmlglobal.h>

QT_BEGIN_NAMESPACE

class Q_QML_EXPORT QQmlAbstractUrlInterceptor
{
public:
    enum DataType { //Matches QQmlDataBlob::Type
        QmlFile = 0,
        JavaScriptFile = 1,
        QmldirFile = 2,
        UrlString = 0x1000
    };

    QQmlAbstractUrlInterceptor() = default;
    virtual ~QQmlAbstractUrlInterceptor() = default;
    virtual QUrl intercept(const QUrl &path, DataType type) = 0;
};

QT_END_NAMESPACE
#endif
