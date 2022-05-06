/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Copyright (C) 2016 Gunnar Sletta <gunnar@sletta.org>
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

#ifndef QQUICKANIMATORCONTROLLER_P_H
#define QQUICKANIMATORCONTROLLER_P_H

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

#include "qquickanimatorjob_p.h"
#include <QtQuick/qsgnode.h>
#include <QtQuick/qquickitem.h>

#include <QtCore/qmutex.h>
#include <QtCore/qthread.h>

QT_BEGIN_NAMESPACE

class Q_AUTOTEST_EXPORT QQuickAnimatorController : public QObject, public QAnimationJobChangeListener
{
    Q_OBJECT

public:
    QQuickAnimatorController(QQuickWindow *window);
    ~QQuickAnimatorController();

    void advance();
    void beforeNodeSync();
    void afterNodeSync();

    void animationFinished(QAbstractAnimationJob *job) override;
    void animationStateChanged(QAbstractAnimationJob *job, QAbstractAnimationJob::State newState, QAbstractAnimationJob::State oldState) override;

    void requestSync();

    // These are called from the GUI thread (the proxy)
    void start(const QSharedPointer<QAbstractAnimationJob> &job);
    void cancel(const QSharedPointer<QAbstractAnimationJob> &job);
    bool isPendingStart(const QSharedPointer<QAbstractAnimationJob> &job) const { return m_rootsPendingStart.contains(job); }

    void lock() { m_mutex.lock(); }
    void unlock() { m_mutex.unlock(); }

    void windowNodesDestroyed();

    QQuickWindow *window() const { return m_window; }

private:
    friend class tst_Animators;

    void start_helper(QAbstractAnimationJob *job);
    void cancel_helper(QAbstractAnimationJob *job);

    QSet<QQuickAnimatorJob * > m_runningAnimators;
    QHash<QAbstractAnimationJob *, QSharedPointer<QAbstractAnimationJob> > m_animationRoots;
    QSet<QSharedPointer<QAbstractAnimationJob> > m_rootsPendingStop;
    QSet<QSharedPointer<QAbstractAnimationJob> > m_rootsPendingStart;

    QQuickWindow *m_window;
    QMutex m_mutex;
};



QT_END_NAMESPACE

#endif // QQUICKANIMATORCONTROLLER_P_H
