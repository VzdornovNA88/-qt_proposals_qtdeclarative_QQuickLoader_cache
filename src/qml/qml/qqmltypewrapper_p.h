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

#ifndef QV8TYPEWRAPPER_P_H
#define QV8TYPEWRAPPER_P_H

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
#include <QtCore/qpointer.h>

#include <private/qv4value_p.h>
#include <private/qv4object_p.h>

QT_BEGIN_NAMESPACE

class QQmlTypeNameCache;
class QQmlType;
class QQmlTypePrivate;
struct QQmlImportRef;

namespace QV4 {

namespace Heap {

struct QQmlTypeWrapper : Object {
    enum TypeNameMode {
        IncludeEnums,
        ExcludeEnums
    };

    void init();
    void destroy();
    TypeNameMode mode;
    QV4QPointer<QObject> object;

    QQmlType type() const;

    const QQmlTypePrivate *typePrivate;
    QQmlTypeNameCache *typeNamespace;
    const QQmlImportRef *importNamespace;
};

struct QQmlScopedEnumWrapper : Object {
    void init() { Object::init(); }
    void destroy();
    int scopeEnumIndex;
    const QQmlTypePrivate *typePrivate;
    QQmlType type() const;
};

}

struct Q_QML_EXPORT QQmlTypeWrapper : Object
{
    V4_OBJECT2(QQmlTypeWrapper, Object)
    V4_NEEDS_DESTROY

    bool isSingleton() const;
    QObject *object() const;
    QObject *singletonObject() const;

    QVariant toVariant() const;

    static ReturnedValue create(ExecutionEngine *, QObject *, const QQmlType &,
                                Heap::QQmlTypeWrapper::TypeNameMode = Heap::QQmlTypeWrapper::IncludeEnums);
    static ReturnedValue create(ExecutionEngine *, QObject *, const QQmlRefPointer<QQmlTypeNameCache> &, const QQmlImportRef *,
                                Heap::QQmlTypeWrapper::TypeNameMode = Heap::QQmlTypeWrapper::IncludeEnums);

    static ReturnedValue virtualResolveLookupGetter(const Object *object, ExecutionEngine *engine, Lookup *lookup);
    static bool virtualResolveLookupSetter(Object *object, ExecutionEngine *engine, Lookup *lookup, const Value &value);

    static ReturnedValue lookupSingletonProperty(Lookup *l, ExecutionEngine *engine, const Value &base);
    static ReturnedValue lookupEnumValue(Lookup *l, ExecutionEngine *engine, const Value &base);
    static ReturnedValue lookupScopedEnum(Lookup *l, ExecutionEngine *engine, const Value &base);

protected:
    static ReturnedValue virtualGet(const Managed *m, PropertyKey id, const Value *receiver, bool *hasProperty);
    static bool virtualPut(Managed *m, PropertyKey id, const Value &value, Value *receiver);
    static PropertyAttributes virtualGetOwnProperty(const Managed *m, PropertyKey id, Property *p);
    static bool virtualIsEqualTo(Managed *that, Managed *o);
    static ReturnedValue virtualInstanceOf(const Object *typeObject, const Value &var);
};

struct Q_QML_EXPORT QQmlScopedEnumWrapper : Object
{
    V4_OBJECT2(QQmlScopedEnumWrapper, Object)
    V4_NEEDS_DESTROY

    static ReturnedValue virtualGet(const Managed *m, PropertyKey id, const Value *receiver, bool *hasProperty);
};

}

QT_END_NAMESPACE

#endif // QV8TYPEWRAPPER_P_H

