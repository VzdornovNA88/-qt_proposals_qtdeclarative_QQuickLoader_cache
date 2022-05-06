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

#ifndef QQMLABSTRACTBINDING_P_H
#define QQMLABSTRACTBINDING_P_H

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

#include <QtCore/qsharedpointer.h>
#include <QtCore/qshareddata.h>
#include <private/qtqmlglobal_p.h>
#include <private/qqmlproperty_p.h>

QT_BEGIN_NAMESPACE

class QQmlObjectCreator;

class Q_QML_PRIVATE_EXPORT QQmlAbstractBinding
{
protected:
    QQmlAbstractBinding();
public:
    virtual ~QQmlAbstractBinding();

    typedef QExplicitlySharedDataPointer<QQmlAbstractBinding> Ptr;

    virtual QString expression() const;

    virtual bool isValueTypeProxy() const;

    // Should return the encoded property index for the binding.  Should return this value
    // even if the binding is not enabled or added to an object.
    // Encoding is:  coreIndex | (valueTypeIndex << 16)
    QQmlPropertyIndex targetPropertyIndex() const { return m_targetIndex; }

    // Should return the object for the binding.  Should return this object even if the
    // binding is not enabled or added to the object.
    QObject *targetObject() const { return m_target.data(); }

    virtual void setEnabled(bool e, QQmlPropertyData::WriteFlags f = QQmlPropertyData::DontRemoveBinding) = 0;

    void addToObject();
    void removeFromObject();

    static void printBindingLoopError(QQmlProperty &prop);

    inline QQmlAbstractBinding *nextBinding() const;

    inline bool canUseAccessor() const
    { return m_nextBinding.tag().testFlag(CanUseAccessor); }

    struct RefCount {
        RefCount() {}
        int refCount = 0;
        void ref() { ++refCount; }
        int deref() { return --refCount; }
        operator int() const { return refCount; }
    };
    RefCount ref;

    enum TargetTag {
        NoTargetTag     = 0x0,
        UpdatingBinding = 0x1,
        BindingEnabled  = 0x2
    };
    Q_DECLARE_FLAGS(TargetTags, TargetTag)

    enum NextBindingTag {
        NoBindingTag   = 0x0,
        AddedToObject  = 0x1,
        CanUseAccessor = 0x2
    };
    Q_DECLARE_FLAGS(NextBindingTags, NextBindingTag)

protected:
    friend class QQmlData;
    friend class QQmlValueTypeProxyBinding;
    friend class QQmlObjectCreator;

    inline void setAddedToObject(bool v);
    inline bool isAddedToObject() const;

    inline void setNextBinding(QQmlAbstractBinding *);

    QQmlPropertyIndex m_targetIndex;

    // Pointer is the target object to which the binding binds
    QTaggedPointer<QObject, TargetTags> m_target;

    // Pointer to the next binding in the linked list of bindings.
    QTaggedPointer<QQmlAbstractBinding, NextBindingTags> m_nextBinding;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QQmlAbstractBinding::TargetTags)
Q_DECLARE_OPERATORS_FOR_FLAGS(QQmlAbstractBinding::NextBindingTags)

void QQmlAbstractBinding::setAddedToObject(bool v)
{
    m_nextBinding.setTag(m_nextBinding.tag().setFlag(AddedToObject, v));
}

bool QQmlAbstractBinding::isAddedToObject() const
{
    return m_nextBinding.tag().testFlag(AddedToObject);
}

QQmlAbstractBinding *QQmlAbstractBinding::nextBinding() const
{
    return m_nextBinding.data();
}

void QQmlAbstractBinding::setNextBinding(QQmlAbstractBinding *b)
{
    if (b)
        b->ref.ref();
    if (m_nextBinding.data() && !m_nextBinding->ref.deref())
        delete m_nextBinding.data();
    m_nextBinding = b;
}

QT_END_NAMESPACE

#endif // QQMLABSTRACTBINDING_P_H
