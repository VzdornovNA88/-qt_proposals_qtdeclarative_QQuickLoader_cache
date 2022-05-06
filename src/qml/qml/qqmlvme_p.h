/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
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

#ifndef QQMLVME_P_H
#define QQMLVME_P_H

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

#include "qqmlerror.h"
#include <private/qrecursionwatcher_p.h>

#include <QtCore/QStack>
#include <QtCore/QString>
#include <QtCore/qelapsedtimer.h>
#include <QtCore/qcoreapplication.h>
#include <QtCore/qtypeinfo.h>

#include <private/qqmlengine_p.h>
#include <private/qfinitestack_p.h>

#include <atomic>

QT_BEGIN_NAMESPACE

class QObject;

class QQmlInstantiationInterrupt {
public:
    inline QQmlInstantiationInterrupt();
    inline QQmlInstantiationInterrupt(std::atomic<bool> *runWhile, qint64 nsecs = 0);
    inline QQmlInstantiationInterrupt(qint64 nsecs);

    inline void reset();
    inline bool shouldInterrupt() const;
private:
    enum Mode { None, Time, Flag };
    Mode mode;
    QElapsedTimer timer;
    qint64 nsecs = 0;
    std::atomic<bool> *runWhile = nullptr;
};

class Q_QML_PRIVATE_EXPORT QQmlVME
{
public:
    static void enableComponentComplete();
    static void disableComponentComplete();
    static bool componentCompleteEnabled();

private:
    static bool s_enableComponentComplete;
};

// Used to check that a QQmlVME that is interrupted mid-execution
// is still valid.  Checks all the objects and contexts have not been
// deleted.
//
// VME stands for Virtual Machine Execution. QML files used to
// be compiled to a byte code data structure that a virtual machine executed
// (for constructing the tree of QObjects and setting properties).
class QQmlVMEGuard
{
public:
    QQmlVMEGuard();
    ~QQmlVMEGuard();

    void guard(QQmlObjectCreator *);
    void clear();

    bool isOK() const;

private:
    int m_objectCount;
    QQmlGuard<QObject> *m_objects;
    int m_contextCount;
    QQmlGuardedContextData *m_contexts;
};

QQmlInstantiationInterrupt::QQmlInstantiationInterrupt()
    : mode(None)
{
}

QQmlInstantiationInterrupt::QQmlInstantiationInterrupt(std::atomic<bool> *runWhile, qint64 nsecs)
    : mode(Flag), nsecs(nsecs), runWhile(runWhile)
{
}

QQmlInstantiationInterrupt::QQmlInstantiationInterrupt(qint64 nsecs)
    : mode(Time), nsecs(nsecs)
{
}

void QQmlInstantiationInterrupt::reset()
{
    if (mode == Time || nsecs)
        timer.start();
}

bool QQmlInstantiationInterrupt::shouldInterrupt() const
{
    switch (mode) {
    case None:
        return false;
    case Time:
        return timer.nsecsElapsed() > nsecs;
    case Flag:
        return !runWhile->load(std::memory_order_acquire) || (nsecs && timer.nsecsElapsed() > nsecs);
    }
    Q_UNREACHABLE();
    return false;
}

QT_END_NAMESPACE

#endif // QQMLVME_P_H
