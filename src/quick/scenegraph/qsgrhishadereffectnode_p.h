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

#ifndef QSGRHISHADEREFFECTNODE_P_H
#define QSGRHISHADEREFFECTNODE_P_H

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

#include <private/qsgadaptationlayer_p.h>
#include <qsgmaterial.h>
#include <QUrl>

QT_BEGIN_NAMESPACE

class QSGDefaultRenderContext;
class QSGPlainTexture;
class QSGRhiShaderEffectNode;
class QFileSelector;

class QSGRhiShaderLinker
{
public:
    void reset(const QShader &vs, const QShader &fs);

    void feedConstants(const QSGShaderEffectNode::ShaderData &shader, const QSet<int> *dirtyIndices = nullptr);
    void feedSamplers(const QSGShaderEffectNode::ShaderData &shader, const QSet<int> *dirtyIndices = nullptr);
    void linkTextureSubRects();

    void dump();

    struct Constant {
        uint size;
        QSGShaderEffectNode::VariableData::SpecialType specialType;
        QVariant value;
        bool operator==(const Constant &other) const {
            return size == other.size && specialType == other.specialType
                    && (specialType == QSGShaderEffectNode::VariableData::None ? value == other.value : true);
        }
    };

    bool m_error;
    QShader m_vs;
    QShader m_fs;
    uint m_constantBufferSize;
    QHash<uint, Constant> m_constants; // offset -> Constant
    QHash<int, QVariant> m_samplers; // binding -> value (source ref)
    QHash<QByteArray, int> m_samplerNameMap; // name -> binding
    QSet<int> m_subRectBindings;
};

QDebug operator<<(QDebug debug, const QSGRhiShaderLinker::Constant &c);

class QSGRhiShaderEffectMaterial : public QSGMaterial
{
public:
    QSGRhiShaderEffectMaterial(QSGRhiShaderEffectNode *node);
    ~QSGRhiShaderEffectMaterial();

    int compare(const QSGMaterial *other) const override;
    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader(QSGRendererInterface::RenderMode renderMode) const override;

    void updateTextureProviders(bool layoutChange);

    bool usesSubRectUniform(int binding) const { return m_linker.m_subRectBindings.contains(binding); }

    static const int MAX_BINDINGS = 32;

    QSGRhiShaderEffectNode *m_node;
    QSGMaterialType *m_materialType = nullptr;
    QSGRhiShaderLinker m_linker;
    QVector<QSGTextureProvider *> m_textureProviders; // [binding] = QSGTextureProvider
    bool m_geometryUsesTextureSubRect = false;
    QSGShaderEffectNode::CullMode m_cullMode = QSGShaderEffectNode::NoCulling;
    bool m_hasCustomVertexShader = false;
    bool m_hasCustomFragmentShader = false;
    QShader m_vertexShader;
    QShader m_fragmentShader;
    QSGPlainTexture *m_dummyTexture = nullptr;
};

class QSGRhiShaderEffectNode : public QSGShaderEffectNode
{
    Q_OBJECT

public:
    QSGRhiShaderEffectNode(QSGDefaultRenderContext *rc);

    QRectF updateNormalizedTextureSubRect(bool supportsAtlasTextures) override;
    void syncMaterial(SyncData *syncData) override;
    void preprocess() override;

    static void cleanupMaterialTypeCache();

private Q_SLOTS:
    void handleTextureChange();
    void handleTextureProviderDestroyed(QObject *object);

private:
    QSGDefaultRenderContext *m_rc;
    QSGRhiShaderEffectMaterial m_material;
};

class QSGRhiGuiThreadShaderEffectManager : public QSGGuiThreadShaderEffectManager
{
public:
    bool hasSeparateSamplerAndTextureObjects() const override;
    QString log() const override;
    Status status() const override;
    void prepareShaderCode(ShaderInfo::Type typeHint, const QUrl &src, ShaderInfo *result) override;

private:
    bool reflect(ShaderInfo *result);
    Status m_status = Uncompiled;
    QFileSelector *m_fileSelector = nullptr;
};

QT_END_NAMESPACE

#endif // QSGRHISHADEREFFECTNODE_P_H
