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

#ifndef QSGTEXTURE_P_H
#define QSGTEXTURE_P_H

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

#include <QtQuick/private/qtquickglobal_p.h>
#include <private/qobject_p.h>
#include "qsgtexture.h"

QT_BEGIN_NAMESPACE

struct QSGSamplerDescription
{
    QSGTexture::Filtering filtering = QSGTexture::Nearest;
    QSGTexture::Filtering mipmapFiltering = QSGTexture::None;
    QSGTexture::WrapMode horizontalWrap = QSGTexture::ClampToEdge;
    QSGTexture::WrapMode verticalWrap = QSGTexture::ClampToEdge;
    QSGTexture::AnisotropyLevel anisotropylevel = QSGTexture::AnisotropyNone;

    static QSGSamplerDescription fromTexture(QSGTexture *t);
};

Q_DECLARE_TYPEINFO(QSGSamplerDescription, Q_RELOCATABLE_TYPE);

bool operator==(const QSGSamplerDescription &a, const QSGSamplerDescription &b) noexcept;
bool operator!=(const QSGSamplerDescription &a, const QSGSamplerDescription &b) noexcept;
size_t qHash(const QSGSamplerDescription &s, size_t seed = 0) noexcept;

#if QT_CONFIG(opengl)
class Q_QUICK_PRIVATE_EXPORT QSGTexturePlatformOpenGL : public QNativeInterface::QSGOpenGLTexture
{
public:
    QSGTexturePlatformOpenGL(QSGTexture *t) : m_texture(t) { }
    QSGTexture *m_texture;

    GLuint nativeTexture() const override;
};
#endif

#ifdef Q_OS_WIN
class Q_QUICK_PRIVATE_EXPORT QSGTexturePlatformD3D11 : public QNativeInterface::QSGD3D11Texture
{
public:
    QSGTexturePlatformD3D11(QSGTexture *t) : m_texture(t) { }
    QSGTexture *m_texture;

    void *nativeTexture() const override;
};
#endif

#if defined(__OBJC__)
class Q_QUICK_PRIVATE_EXPORT QSGTexturePlatformMetal : public QNativeInterface::QSGMetalTexture
{
public:
    QSGTexturePlatformMetal(QSGTexture *t) : m_texture(t) { }
    QSGTexture *m_texture;

    id<MTLTexture> nativeTexture() const override;
};
#endif

#if QT_CONFIG(vulkan)
class Q_QUICK_PRIVATE_EXPORT QSGTexturePlatformVulkan : public QNativeInterface::QSGVulkanTexture
{
public:
    QSGTexturePlatformVulkan(QSGTexture *t) : m_texture(t) { }
    QSGTexture *m_texture;

    VkImage nativeImage() const override;
    VkImageLayout nativeImageLayout() const override;
};
#endif

class Q_QUICK_PRIVATE_EXPORT QSGTexturePrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QSGTexture)
public:
    QSGTexturePrivate(QSGTexture *t);
    static QSGTexturePrivate *get(QSGTexture *t) { return t->d_func(); }
    void resetDirtySamplerOptions();
    bool hasDirtySamplerOptions() const;

    uint wrapChanged : 1;
    uint filteringChanged : 1;
    uint anisotropyChanged : 1;

    uint horizontalWrap : 2;
    uint verticalWrap : 2;
    uint mipmapMode : 2;
    uint filterMode : 2;
    uint anisotropyLevel: 3;

    // While we could make QSGTexturePrivate implement all the interfaces, we
    // rather choose to use separate objects to avoid clashes in the function
    // names and signatures.
#if QT_CONFIG(opengl)
    QSGTexturePlatformOpenGL m_openglTextureAccessor;
#endif
#ifdef Q_OS_WIN
    QSGTexturePlatformD3D11 m_d3d11TextureAccessor;
#endif
#if defined(__OBJC__)
    QSGTexturePlatformMetal m_metalTextureAccessor;
#endif
#if QT_CONFIG(vulkan)
    QSGTexturePlatformVulkan m_vulkanTextureAccessor;
#endif
};

Q_QUICK_PRIVATE_EXPORT bool qsg_safeguard_texture(QSGTexture *);

QT_END_NAMESPACE

#endif // QSGTEXTURE_P_H
