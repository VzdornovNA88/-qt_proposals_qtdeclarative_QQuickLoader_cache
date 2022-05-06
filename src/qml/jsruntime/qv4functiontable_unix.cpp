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

#include "qv4functiontable_p.h"
#include "qv4function_p.h"

#include <assembler/MacroAssemblerCodeRef.h>

#include <QtCore/qfile.h>
#include <QtCore/qcoreapplication.h>

QT_BEGIN_NAMESPACE

namespace QV4 {

void generateFunctionTable(Function *function, JSC::MacroAssemblerCodeRef *codeRef)
{
    // This implements writing of JIT'd addresses so that perf can find the
    // symbol names.
    //
    // Perf expects the mapping to be in a certain place and have certain
    // content, for more information, see:
    // https://github.com/torvalds/linux/blob/master/tools/perf/Documentation/jit-interface.txt
    static bool doProfile = !qEnvironmentVariableIsEmpty("QV4_PROFILE_WRITE_PERF_MAP");
    if (Q_UNLIKELY(doProfile)) {
        static QFile perfMapFile(QString::fromLatin1("/tmp/perf-%1.map")
                                 .arg(QCoreApplication::applicationPid()));
        static const bool isOpen = perfMapFile.open(QIODevice::WriteOnly);
        if (!isOpen) {
            qWarning("QV4::JIT::Assembler: Cannot write perf map file.");
            doProfile = false;
        } else {
            const void *address = codeRef->code().executableAddress();
            perfMapFile.write(QByteArray::number(reinterpret_cast<quintptr>(address), 16));
            perfMapFile.putChar(' ');
            perfMapFile.write(QByteArray::number(static_cast<qsizetype>(codeRef->size()), 16));
            perfMapFile.putChar(' ');
            perfMapFile.write(Function::prettyName(function, address).toUtf8());
            perfMapFile.putChar('\n');
            perfMapFile.flush();
        }
    }
}

void destroyFunctionTable(Function *function, JSC::MacroAssemblerCodeRef *codeRef)
{
    Q_UNUSED(function);
    Q_UNUSED(codeRef);

    // It's not advisable to remove things from the perf map file, as it's primarily used to analyze
    // a trace after the application has terminated. We want to know about all functions that were
    // ever jitted then. If the memory ranges overlap, we will have a problem when analyzing the
    // trace. The JIT should try to avoid this.
}

size_t exceptionHandlerSize()
{
    return 0;
}

} // QV4

QT_END_NAMESPACE
