/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
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

#ifndef QSGTEXTURE_PLATFORM_H
#define QSGTEXTURE_PLATFORM_H

#include <QtCore/qnativeinterface.h>
#include <QtQuick/qquickwindow.h>

#if QT_CONFIG(opengl)
#include <QtGui/qopengl.h>
#endif

#if QT_CONFIG(vulkan)
#include <QtGui/qvulkaninstance.h>
#endif

#if defined(__OBJC__) || defined(Q_CLANG_QDOC)
@protocol MTLTexture;
#endif

QT_BEGIN_NAMESPACE

namespace QNativeInterface {

#if QT_CONFIG(opengl) || defined(Q_CLANG_QDOC)
struct Q_QUICK_EXPORT QSGOpenGLTexture
{
    QT_DECLARE_NATIVE_INTERFACE(QSGOpenGLTexture, 1, QSGTexture)
    virtual GLuint nativeTexture() const = 0;
    static QSGTexture *fromNative(GLuint textureId,
                                  QQuickWindow *window,
                                  const QSize &size,
                                  QQuickWindow::CreateTextureOptions options = {});
    static QSGTexture *fromNativeExternalOES(GLuint textureId,
                                             QQuickWindow *window,
                                             const QSize &size,
                                             QQuickWindow::CreateTextureOptions options = {});
};
#endif

#if defined(Q_OS_WIN) || defined(Q_CLANG_QDOC)
struct Q_QUICK_EXPORT QSGD3D11Texture
{
    QT_DECLARE_NATIVE_INTERFACE(QSGD3D11Texture, 1, QSGTexture)
    virtual void *nativeTexture() const = 0;
    static QSGTexture *fromNative(void *texture,
                                  QQuickWindow *window,
                                  const QSize &size,
                                  QQuickWindow::CreateTextureOptions options = {});
};
#endif

#if defined(__OBJC__) || defined(Q_CLANG_QDOC)
struct Q_QUICK_EXPORT QSGMetalTexture
{
    QT_DECLARE_NATIVE_INTERFACE(QSGMetalTexture, 1, QSGTexture)
    virtual id<MTLTexture> nativeTexture() const = 0;
    static QSGTexture *fromNative(id<MTLTexture> texture,
                                  QQuickWindow *window,
                                  const QSize &size,
                                  QQuickWindow::CreateTextureOptions options = {});
};
#endif

#if QT_CONFIG(vulkan) || defined(Q_CLANG_QDOC)
struct Q_QUICK_EXPORT QSGVulkanTexture
{
    QT_DECLARE_NATIVE_INTERFACE(QSGVulkanTexture, 1, QSGTexture)
    virtual VkImage nativeImage() const = 0;
    virtual VkImageLayout nativeImageLayout() const = 0;
    static QSGTexture *fromNative(VkImage image,
                                  VkImageLayout layout,
                                  QQuickWindow *window,
                                  const QSize &size,
                                  QQuickWindow::CreateTextureOptions options = {});
};
#endif

} // QNativeInterface

QT_END_NAMESPACE

#endif // QSGTEXTURE_PLATFORM_H
