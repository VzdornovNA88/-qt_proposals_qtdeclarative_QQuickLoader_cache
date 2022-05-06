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

#include "qquickgraphicsinfo_p.h"
#include <private/qquickitem_p.h>
#include <qopenglcontext.h>

QT_BEGIN_NAMESPACE

/*!
    \qmltype GraphicsInfo
    \instantiates QQuickGraphicsInfo
    \inqmlmodule QtQuick
    \ingroup qtquick-visual
    \since 5.8
    \since QtQuick 2.8
    \brief Provides information about the scenegraph backend and the graphics API used by Qt Quick.

    The GraphicsInfo attached type provides information about the scenegraph
    backend and the graphics API used to render the contents of the associated window.

    If the item to which the properties are attached is not currently
    associated with any window, the properties are set to default values. When
    the associated window changes, the properties will update.
 */

QQuickGraphicsInfo::QQuickGraphicsInfo(QQuickItem *item)
    : QObject(item)
    , m_window(nullptr)
    , m_api(Unknown)
    , m_shaderType(UnknownShadingLanguage)
    , m_shaderCompilationType(ShaderCompilationType(0))
    , m_shaderSourceType(ShaderSourceType(0))
    , m_majorVersion(2)
    , m_minorVersion(0)
    , m_profile(OpenGLNoProfile)
    , m_renderableType(SurfaceFormatUnspecified)
{
    if (Q_LIKELY(item)) {
        connect(item, &QQuickItem::windowChanged, this, &QQuickGraphicsInfo::setWindow);
        setWindow(item->window());
    }
}

QQuickGraphicsInfo *QQuickGraphicsInfo::qmlAttachedProperties(QObject *object)
{
    if (QQuickItem *item = qobject_cast<QQuickItem *>(object))
        return new QQuickGraphicsInfo(item);

    return nullptr;
}

/*!
    \qmlproperty enumeration QtQuick::GraphicsInfo::api

    This property describes the graphics API that is currently in use.

    The possible values are:
    \list
    \li GraphicsInfo.Unknown - the default value when no active scenegraph is associated with the item
    \li GraphicsInfo.Software - Qt Quick's software renderer based on QPainter with the raster paint engine
    \li GraphicsInfo.OpenVG - OpenVG
    \li GraphicsInfo.OpenGL - OpenGL or OpenGL ES on top of QRhi, a graphics abstraction layer
    \li GraphicsInfo.Direct3D11 - Direct3D 11 on top of QRhi, a graphics abstraction layer
    \li GraphicsInfo.Vulkan - Vulkan on top of QRhi, a graphics abstraction layer
    \li GraphicsInfo.Metal - Metal on top of QRhi, a graphics abstraction layer
    \li GraphicsInfo.Null - Null (no output) on top of QRhi, a graphics abstraction layer
    \endlist
 */

/*!
    \qmlproperty enumeration QtQuick::GraphicsInfo::shaderType

    This property contains the shading language supported by the Qt Quick
    backend the application is using.

    \list
    \li GraphicsInfo.UnknownShadingLanguage - Not yet known due to no window and scenegraph associated
    \li GraphicsInfo.GLSL - GLSL or GLSL ES
    \li GraphicsInfo.HLSL - HLSL
    \li GraphicsInfo.RhiShader - QShader
    \endlist

    \note The value is only up-to-date once the item is associated with a
    window. Bindings relying on the value have to keep this in mind since the
    value may change from GraphicsInfo.UnknownShadingLanguage to the actual
    value after component initialization is complete. This is particularly
    relevant for ShaderEffect items inside ShaderEffectSource items set as
    property values.

    \since 5.8
    \since QtQuick 2.8

    \sa shaderCompilationType, shaderSourceType
*/

/*!
    \qmlproperty enumeration QtQuick::GraphicsInfo::shaderCompilationType

    This property contains a bitmask of the shader compilation approaches
    supported by the Qt Quick backend the application is using.

    \list
    \li GraphicsInfo.RuntimeCompilation
    \li GraphicsInfo.OfflineCompilation
    \endlist

    With OpenGL the value is GraphicsInfo.RuntimeCompilation, which corresponds
    to the traditional way of using ShaderEffect. Non-OpenGL backends are
    expected to focus more on GraphicsInfo.OfflineCompilation, however.

    \note The value is only up-to-date once the item is associated with a
    window. Bindings relying on the value have to keep this in mind since the
    value may change from \c 0 to the actual bitmask after component
    initialization is complete. This is particularly relevant for ShaderEffect
    items inside ShaderEffectSource items set as property values.

    \since 5.8
    \since QtQuick 2.8

    \sa shaderType, shaderSourceType
*/

/*!
    \qmlproperty enumeration QtQuick::GraphicsInfo::shaderSourceType

    This property contains a bitmask of the supported ways of providing shader
    sources.

    \list
    \li GraphicsInfo.ShaderSourceString
    \li GraphicsInfo.ShaderSourceFile
    \li GraphicsInfo.ShaderByteCode
    \endlist

    With OpenGL the value is GraphicsInfo.ShaderSourceString, which corresponds
    to the traditional way of inlining GLSL source code into QML. Other,
    non-OpenGL Qt Quick backends may however decide not to support inlined
    shader sources, or even shader sources at all. In this case shaders are
    expected to be pre-compiled into formats like SPIR-V or D3D shader
    bytecode.

    \note The value is only up-to-date once the item is associated with a
    window. Bindings relying on the value have to keep this in mind since the
    value may change from \c 0 to the actual bitmask after component
    initialization is complete. This is particularly relevant for ShaderEffect
    items inside ShaderEffectSource items set as property values.

    \since 5.8
    \since QtQuick 2.8

    \sa shaderType, shaderCompilationType
*/

/*!
    \qmlproperty int QtQuick::GraphicsInfo::majorVersion

    This property holds the major version of the graphics API in use.

    With OpenGL the default version is \c 2.0.

    \note This is applicable only to OpenGL.

    \sa minorVersion, profile
 */

/*!
    \qmlproperty int QtQuick::GraphicsInfo::minorVersion

    This property holds the minor version of the graphics API in use.

    With OpenGL the default version is \c 2.0.

    \note This is applicable only to OpenGL.

    \sa majorVersion, profile
 */

/*!
    \qmlproperty enumeration QtQuick::GraphicsInfo::profile

    This property holds the configured OpenGL context profile.

    The possible values are:
    \list
    \li GraphicsInfo.OpenGLNoProfile (default) - OpenGL version is lower than 3.2 or OpenGL is not in use.
    \li GraphicsInfo.OpenGLCoreProfile - Functionality deprecated in OpenGL version 3.0 is not available.
    \li GraphicsInfo.OpenGLCompatibilityProfile - Functionality from earlier OpenGL versions is available.
    \endlist

    Reusable QML components will typically use this property in bindings in order to
    choose between core and non core profile compatible shader sources.

    \note This is applicable only to OpenGL.

    \sa majorVersion, minorVersion, QSurfaceFormat
 */

/*!
    \qmlproperty enumeration QtQuick::GraphicsInfo::renderableType

    This property holds the renderable type. The value has no meaning for APIs
    other than OpenGL.

    The possible values are:
    \list
    \li GraphicsInfo.SurfaceFormatUnspecified (default) - Unspecified rendering method
    \li GraphicsInfo.SurfaceFormatOpenGL - Desktop OpenGL or other graphics API
    \li GraphicsInfo.SurfaceFormatOpenGLES - OpenGL ES
    \endlist

    \note This is applicable only to OpenGL.

    \sa QSurfaceFormat
 */

void QQuickGraphicsInfo::updateInfo()
{
    // The queries via the RIF do not depend on isSceneGraphInitialized(), they only need a window.
    if (m_window) {
        QSGRendererInterface *rif = m_window->rendererInterface();
        if (rif) {
            GraphicsApi newAPI = GraphicsApi(rif->graphicsApi());
            if (m_api != newAPI) {
                m_api = newAPI;
                emit apiChanged();
                m_shaderType = ShaderType(rif->shaderType());
                emit shaderTypeChanged();
                m_shaderCompilationType = ShaderCompilationType(int(rif->shaderCompilationType()));
                emit shaderCompilationTypeChanged();
                m_shaderSourceType = ShaderSourceType(int(rif->shaderSourceType()));
                emit shaderSourceTypeChanged();
            }
        }
    }

    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
#if QT_CONFIG(opengl)
    if (m_window && m_window->isSceneGraphInitialized()) {
        QOpenGLContext *context = QQuickWindowPrivate::get(m_window)->openglContext();
        if (context)
            format = context->format();
    }
#endif
    if (m_majorVersion != format.majorVersion()) {
        m_majorVersion = format.majorVersion();
        emit majorVersionChanged();
    }
    if (m_minorVersion != format.minorVersion()) {
        m_minorVersion = format.minorVersion();
        emit minorVersionChanged();
    }
    OpenGLContextProfile profile = static_cast<OpenGLContextProfile>(format.profile());
    if (m_profile != profile) {
        m_profile = profile;
        emit profileChanged();
    }
    RenderableType renderableType = static_cast<RenderableType>(format.renderableType());
    if (m_renderableType != renderableType) {
        m_renderableType = renderableType;
        emit renderableTypeChanged();
    }
}

void QQuickGraphicsInfo::setWindow(QQuickWindow *window)
{
    if (m_window != window) {
        if (m_window) {
            disconnect(m_window, SIGNAL(sceneGraphInitialized()), this, SLOT(updateInfo()));
            disconnect(m_window, SIGNAL(sceneGraphInvalidated()), this, SLOT(updateInfo()));
        }
        if (window) {
            connect(window, SIGNAL(sceneGraphInitialized()), this, SLOT(updateInfo()));
            connect(window, SIGNAL(sceneGraphInvalidated()), this, SLOT(updateInfo()));
        }
        m_window = window;
    }
    updateInfo();
}

QT_END_NAMESPACE

#include "moc_qquickgraphicsinfo_p.cpp"
