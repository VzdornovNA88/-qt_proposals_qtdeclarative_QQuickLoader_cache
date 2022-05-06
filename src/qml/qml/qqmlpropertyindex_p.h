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

#ifndef QQMLPROPERTYINDEX_P_H
#define QQMLPROPERTYINDEX_P_H

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

#include <private/qglobal_p.h>

QT_BEGIN_NAMESPACE

class QQmlPropertyIndex
{
    qint32 index;

public:
    QQmlPropertyIndex()
    { index = -1; }

    static QQmlPropertyIndex fromEncoded(qint32 encodedIndex)
    {
        QQmlPropertyIndex idx;
        idx.index = encodedIndex;
        return idx;
    }

    explicit QQmlPropertyIndex(int coreIndex)
    { index = encode(coreIndex, -1); }

    explicit QQmlPropertyIndex(int coreIndex, int valueTypeIndex)
        : index(encode(coreIndex, valueTypeIndex))
    {}

    bool isValid() const
    { return index != -1; }

    int coreIndex() const
    {
        if (index == -1)
            return -1;
        return index & 0xffff;
    }

    int valueTypeIndex() const
    {
        if (index == -1)
            return -1;
        return (index >> 16) - 1;
    }

    bool hasValueTypeIndex() const
    {
        if (index == -1)
            return false;
        return index >> 16;
    }

    qint32 toEncoded() const
    { return index; }

    int intValue() const
    { return index; }

    bool operator==(const QQmlPropertyIndex &other) const
    { return index == other.index; }

    bool operator!=(const QQmlPropertyIndex &other) const
    { return !operator==(other); }

private:
    static qint32 encode(int coreIndex, int valueTypeIndex)
    {
        Q_ASSERT(coreIndex >= -1);
        Q_ASSERT(coreIndex <= 0xffff);
        Q_ASSERT(valueTypeIndex >= -1);
        Q_ASSERT(valueTypeIndex < 0xffff);

        if (coreIndex == -1)
            return -1;
        else
            return coreIndex | ((valueTypeIndex + 1) << 16);
    }
};

QT_END_NAMESPACE

#endif // QQMLPROPERTYINDEX_P_H
