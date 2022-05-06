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
#ifndef HASHSET_H
#define HASHSET_H

#include <QtCore/qset.h>

namespace WTF {

template<typename Key>
class HashSet final : public QSet<Key>
{
public:
    struct SetAddResult {
        bool isNewEntry;
    };
    SetAddResult add(const Key &k)
    {
        if (QSet<Key>::find(k) == QSet<Key>::constEnd()) {
            QSet<Key>::insert(k);
            return { true };
        }
        return { false };
    }
};

}

using WTF::HashSet;

#endif
