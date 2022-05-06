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

#ifndef QQMLJSMEMORYPOOL_P_H
#define QQMLJSMEMORYPOOL_P_H

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
#include <QtCore/qshareddata.h>
#include <QtCore/qdebug.h>

#include <cstring>

QT_BEGIN_NAMESPACE

namespace QQmlJS {

class Managed;

class MemoryPool : public QSharedData
{
    MemoryPool(const MemoryPool &other);
    void operator =(const MemoryPool &other);

public:
    MemoryPool() {}

    ~MemoryPool()
    {
        if (_blocks) {
            for (int i = 0; i < _allocatedBlocks; ++i) {
                if (char *b = _blocks[i])
                    free(b);
            }

            free(_blocks);
        }
        qDeleteAll(strings);
    }

    inline void *allocate(size_t size)
    {
        size = (size + 7) & ~size_t(7);
        if (Q_LIKELY(_ptr && (_ptr + size < _end))) {
            void *addr = _ptr;
            _ptr += size;
            return addr;
        }
        return allocate_helper(size);
    }

    void reset()
    {
        _blockCount = -1;
        _ptr = _end = nullptr;
    }

    template <typename Tp> Tp *New() { return new (this->allocate(sizeof(Tp))) Tp(); }
    template <typename Tp, typename... Ta> Tp *New(Ta... args)
    { return new (this->allocate(sizeof(Tp))) Tp(args...); }

    QStringView newString(const QString &string) {
        strings.append(new QString(string));
        return QStringView(*strings.last());
    }

private:
    Q_NEVER_INLINE void *allocate_helper(size_t size)
    {
        size_t currentBlockSize = DEFAULT_BLOCK_SIZE;
        while (Q_UNLIKELY(size >= currentBlockSize))
            currentBlockSize *= 2;

        if (++_blockCount == _allocatedBlocks) {
            if (! _allocatedBlocks)
                _allocatedBlocks = DEFAULT_BLOCK_COUNT;
            else
                _allocatedBlocks *= 2;

            _blocks = reinterpret_cast<char **>(realloc(_blocks, sizeof(char *) * size_t(_allocatedBlocks)));
            Q_CHECK_PTR(_blocks);

            for (int index = _blockCount; index < _allocatedBlocks; ++index)
                _blocks[index] = nullptr;
        }

        char *&block = _blocks[_blockCount];

        if (! block) {
            block = reinterpret_cast<char *>(malloc(currentBlockSize));
            Q_CHECK_PTR(block);
        }

        _ptr = block;
        _end = _ptr + currentBlockSize;

        void *addr = _ptr;
        _ptr += size;
        return addr;
    }

private:
    char **_blocks = nullptr;
    int _allocatedBlocks = 0;
    int _blockCount = -1;
    char *_ptr = nullptr;
    char *_end = nullptr;
    QVector<QString*> strings;

    enum
    {
        DEFAULT_BLOCK_SIZE = 8 * 1024,
        DEFAULT_BLOCK_COUNT = 8
    };
};

class Managed
{
    Q_DISABLE_COPY(Managed)
public:
    Managed() = default;
    ~Managed() = default;

    void *operator new(size_t size, MemoryPool *pool) { return pool->allocate(size); }
    void operator delete(void *) {}
    void operator delete(void *, MemoryPool *) {}
};

} // namespace QQmlJS

QT_END_NAMESPACE

#endif
