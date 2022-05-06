/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
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

#include "qqmlloggingcategory_p.h"

#include <QtQml/qqmlinfo.h>

/*!
    \qmltype LoggingCategory
    \ingroup qml-utility-elements
    \inqmlmodule QtQml
    \brief Defines a logging category in QML.
    \since 5.8

    A logging category can be passed to console.log() and friends as the first argument.
    If supplied to to the logger the LoggingCategory's name will be used as Logging Category
    otherwise the default logging category will be used.

    \qml
    import QtQuick 2.8

    Item {
        LoggingCategory {
            id: category
            name: "com.qt.category"
            defaultLogLevel: LoggingCategory.Warning
        }

        Component.onCompleted: {
          console.log(category, "message");
        }
    }
    \endqml

    \note As the creation of objects is expensive, it is encouraged to put the needed
    LoggingCategory definitions into a singleton and import this where needed.

    \sa QLoggingCategory
*/

/*!
    \qmlproperty string QtQml::LoggingCategory::name

    Holds the name of the logging category.

    \note This property needs to be set when declaring the LoggingCategory
    and cannot be changed later.

    \sa QLoggingCategory::categoryName()
*/

/*!
    \qmlproperty enumeration QtQml::LoggingCategory::defaultLogLevel
    \since 5.12

    Holds the default log level of the logging category. By default it is
    created with the LoggingCategory.Debug log level.

    \note This property needs to be set when declaring the LoggingCategory
    and cannot be changed later.
*/

QQmlLoggingCategory::QQmlLoggingCategory(QObject *parent)
    : QObject(parent)
    , m_initialized(false)
{
}

QQmlLoggingCategory::~QQmlLoggingCategory()
{
}

QString QQmlLoggingCategory::name() const
{
    return QString::fromUtf8(m_name);
}

QQmlLoggingCategory::DefaultLogLevel QQmlLoggingCategory::defaultLogLevel() const
{
    return m_defaultLogLevel;
}

QLoggingCategory *QQmlLoggingCategory::category() const
{
    return m_category.data();
}

void QQmlLoggingCategory::classBegin()
{
}

void QQmlLoggingCategory::componentComplete()
{
    m_initialized = true;
    if (m_name.isNull()) {
        qmlWarning(this) << QLatin1String("Declaring the name of a LoggingCategory is mandatory and cannot be changed later");
    } else {
        QScopedPointer<QLoggingCategory> category(new QLoggingCategory(m_name.constData(), QtMsgType(m_defaultLogLevel)));
        m_category.swap(category);
    }
}

void QQmlLoggingCategory::setDefaultLogLevel(DefaultLogLevel defaultLogLevel)
{
    if (m_defaultLogLevel == defaultLogLevel)
        return;

    if (m_initialized) {
        qmlWarning(this) << QLatin1String("The defaultLogLevel of a LoggingCategory cannot be changed after the component is completed");
        return;
    }

    m_defaultLogLevel = defaultLogLevel;
}

void QQmlLoggingCategory::setName(const QString &name)
{
    const QByteArray newName = name.toUtf8();

    if (m_name == newName)
        return;

    if (m_initialized) {
        qmlWarning(this) << QLatin1String("The name of a LoggingCategory cannot be changed after the component is completed");
        return;
    }

    m_name = newName;
}

#include "moc_qqmlloggingcategory_p.cpp"
