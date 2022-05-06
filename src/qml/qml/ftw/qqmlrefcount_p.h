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

#ifndef QQMLREFCOUNT_P_H
#define QQMLREFCOUNT_P_H

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

#include <QtCore/qglobal.h>
#include <QtCore/qatomic.h>
#include <private/qv4global_p.h>

QT_BEGIN_NAMESPACE


class Q_QML_PRIVATE_EXPORT QQmlRefCount
{
    Q_DISABLE_COPY_MOVE(QQmlRefCount)
public:
    inline QQmlRefCount();
    inline void addref() const;
    inline void release() const;
    inline int count() const;

protected:
    inline virtual ~QQmlRefCount();

private:
    mutable QAtomicInt refCount;
};

template<class T>
class QQmlRefPointer
{
public:
    enum Mode {
        AddRef,
        Adopt
    };
    inline QQmlRefPointer();
    inline QQmlRefPointer(T *, Mode m = AddRef);
    inline QQmlRefPointer(const QQmlRefPointer<T> &);
    inline QQmlRefPointer(QQmlRefPointer<T> &&);
    inline ~QQmlRefPointer();

    inline QQmlRefPointer<T> &operator=(const QQmlRefPointer<T> &o);
    inline QQmlRefPointer<T> &operator=(QQmlRefPointer<T> &&o);

    inline bool isNull() const { return !o; }

    inline T* operator->() const { return o; }
    inline T& operator*() const { return *o; }
    explicit inline operator bool() const { return o != nullptr; }
    inline T* data() const { return o; }

    inline QQmlRefPointer<T> &adopt(T *);

    inline T* take() { T *res = o; o = nullptr; return res; }

    friend bool operator==(const QQmlRefPointer &a, const QQmlRefPointer &b) { return a.o == b.o; }
private:
    T *o;
};

QQmlRefCount::QQmlRefCount()
: refCount(1)
{
}

QQmlRefCount::~QQmlRefCount()
{
    Q_ASSERT(refCount.loadRelaxed() == 0);
}

void QQmlRefCount::addref() const
{
    Q_ASSERT(refCount.loadRelaxed() > 0);
    refCount.ref();
}

void QQmlRefCount::release() const
{
    Q_ASSERT(refCount.loadRelaxed() > 0);
    if (!refCount.deref())
        delete this;
}

int QQmlRefCount::count() const
{
    return refCount.loadRelaxed();
}

template<class T>
QQmlRefPointer<T>::QQmlRefPointer()
: o(nullptr)
{
}

template<class T>
QQmlRefPointer<T>::QQmlRefPointer(T *o, Mode m)
: o(o)
{
    if (m == AddRef && o)
        o->addref();
}

template<class T>
QQmlRefPointer<T>::QQmlRefPointer(const QQmlRefPointer<T> &other)
: o(other.o)
{
    if (o) o->addref();
}

template <class T>
QQmlRefPointer<T>::QQmlRefPointer(QQmlRefPointer<T> &&other)
    : o(other.take())
{
}

template<class T>
QQmlRefPointer<T>::~QQmlRefPointer()
{
    if (o) o->release();
}

template<class T>
QQmlRefPointer<T> &QQmlRefPointer<T>::operator=(const QQmlRefPointer<T> &other)
{
    if (other.o) other.o->addref();
    if (o) o->release();
    o = other.o;
    return *this;
}

template <class T>
QQmlRefPointer<T> &QQmlRefPointer<T>::operator=(QQmlRefPointer<T> &&other)
{
    QQmlRefPointer<T> m(std::move(other));
    qSwap(o, m.o);
    return *this;
}

/*!
Takes ownership of \a other.  take() does *not* add a reference, as it assumes ownership
of the callers reference of other.
*/
template<class T>
QQmlRefPointer<T> &QQmlRefPointer<T>::adopt(T *other)
{
    if (o) o->release();
    o = other;
    return *this;
}

QT_END_NAMESPACE

#endif // QQMLREFCOUNT_P_H
