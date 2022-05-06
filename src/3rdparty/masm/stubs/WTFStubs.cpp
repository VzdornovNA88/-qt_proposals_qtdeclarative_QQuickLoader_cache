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
#include <config.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <qdebug.h>
#include <qbytearray.h> // qvsnprintf
#include <FilePrintStream.h>

namespace WTF {

void* fastMalloc(size_t size)
{
    return malloc(size);
}

void* fastRealloc(void* ptr, size_t size)
{
    return realloc(ptr, size);
}

void fastFree(void* ptr)
{
    free(ptr);
}

uint32_t cryptographicallyRandomNumber()
{
    return 0;
}

static FilePrintStream* s_dataFile = nullptr;

void setDataFile(FilePrintStream *ps)
{
    delete s_dataFile;
    s_dataFile = ps;
}

void setDataFile(FILE* f)
{
    delete s_dataFile;
    s_dataFile = new FilePrintStream(f, FilePrintStream::Borrow);
}

FilePrintStream& dataFile()
{
    if (!s_dataFile)
        s_dataFile = new FilePrintStream(stderr, FilePrintStream::Borrow);
    return *s_dataFile;
}

void dataLogFV(const char* format, va_list args)
{
    char buffer[1024];
    qvsnprintf(buffer, sizeof(buffer), format, args);
    qDebug().nospace().noquote() << buffer;
}

void dataLogF(const char* format, ...)
{
    char buffer[1024];
    va_list args;
    va_start(args, format);
    qvsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    qDebug().nospace().noquote() << buffer;
}

void dataLogFString(const char* str)
{
    qDebug().nospace().noquote() << str;
}

}

extern "C" {
// When adding a new stub here do not forget to add
// DEFINES += StubFunctionName=qmlStubFunctionName
// for example:
// DEFINES += WTFReportAssertionFailureWithMessage=qmlWTFReportAssertionFailureWithMessage
// to prevent "duplicate symbol" error during static library linking. See bugs QTBUG-35041 and QTBUG-63050

void WTFReportAssertionFailure(const char* file, int line, const char* function, const char*assertion)
{
    fprintf(stderr, "WTF failing assertion in %s, line %d, function %s: %s\n", file, line, function, assertion);
}

void WTFReportAssertionFailureWithMessage(const char* file, int line, const char* function, const char* assertion, const char* format, ...)
{
    // TODO: show the message, or remove this function completely. (The latter would probably be best.)
    Q_UNUSED(format);
    fprintf(stderr, "WTF failing assertion in %s, line %d, function %s: %s\n", file, line, function, assertion);
}

void WTFReportBacktrace()
{
}

void WTFInvokeCrashHook()
{
}

}


#if ENABLE(ASSEMBLER) && CPU(X86) && !OS(MAC_OS_X)
#include <MacroAssemblerX86Common.h>

JSC::MacroAssemblerX86Common::SSE2CheckState JSC::MacroAssemblerX86Common::s_sse2CheckState = JSC::MacroAssemblerX86Common::NotCheckedSSE2;
#endif

