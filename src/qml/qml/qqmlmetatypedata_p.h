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

#ifndef QQMLMETATYPEDATA_P_H
#define QQMLMETATYPEDATA_P_H

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

#include <private/qqmltype_p.h>
#include <private/qqmlmetatype_p.h>
#include <private/qhashedstring_p.h>
#include <private/qqmlvaluetype_p.h>

#include <QtCore/qset.h>
#include <QtCore/qvector.h>

QT_BEGIN_NAMESPACE

class QQmlTypePrivate;
struct QQmlMetaTypeData
{
    QQmlMetaTypeData();
    ~QQmlMetaTypeData();
    void registerType(QQmlTypePrivate *priv);
    QList<QQmlType> types;
    QSet<QQmlType> undeletableTypes;
    typedef QHash<int, QQmlTypePrivate *> Ids;
    Ids idToType;

    using Names = QMultiHash<QHashedString, QQmlTypePrivate *>;
    Names nameToType;

    typedef QHash<QUrl, QQmlTypePrivate *> Files; //For file imported composite types only
    Files urlToType;
    Files urlToNonFileImportType; // For non-file imported composite and composite
            // singleton types. This way we can locate any
            // of them by url, even if it was registered as
            // a module via QQmlPrivate::RegisterCompositeType
    typedef QMultiHash<const QMetaObject *, QQmlTypePrivate *> MetaObjects;
    MetaObjects metaObjectToType;
    QVector<QHash<QTypeRevision, QQmlRefPointer<QQmlPropertyCache>>> typePropertyCaches;
    QHash<int, QQmlValueType *> metaTypeToValueType;

    struct VersionedUri {
        VersionedUri() = default;
        VersionedUri(const QString &uri, QTypeRevision version)
            : uri(uri), majorVersion(version.majorVersion()) {}
        VersionedUri(const std::unique_ptr<QQmlTypeModule> &module);

        friend bool operator==(const VersionedUri &a, const VersionedUri &b)
        {
            return a.majorVersion == b.majorVersion && a.uri == b.uri;
        }

        friend size_t qHash(const VersionedUri &v, size_t seed = 0)
        {
            return qHashMulti(seed, v.uri, v.majorVersion);
        }

        friend bool operator<(const QQmlMetaTypeData::VersionedUri &a,
                              const QQmlMetaTypeData::VersionedUri &b)
        {
            const int diff = a.uri.compare(b.uri);
            return diff < 0 || (diff == 0 && a.majorVersion < b.majorVersion);
        }

        QString uri;
        quint8 majorVersion = 0;
    };

    typedef std::vector<std::unique_ptr<QQmlTypeModule>> TypeModules;
    TypeModules uriToModule;
    QQmlTypeModule *findTypeModule(const QString &module, QTypeRevision version);
    QQmlTypeModule *addTypeModule(std::unique_ptr<QQmlTypeModule> module);

    using ModuleImports = QMultiMap<VersionedUri, QQmlDirParser::Import>;
    ModuleImports moduleImports;

    QHash<QString, void (*)()> moduleTypeRegistrationFunctions;
    bool registerModuleTypes(const QString &uri);

    QSet<int> interfaces;

    QList<QQmlPrivate::AutoParentFunction> parentFunctions;
    QVector<QQmlPrivate::QmlUnitCacheLookupFunction> lookupCachedQmlUnit;

    QHash<const QMetaObject *, QQmlPropertyCache *> propertyCaches;

    QQmlPropertyCache *propertyCacheForVersion(int index, QTypeRevision version) const;
    void setPropertyCacheForVersion(int index, QTypeRevision version, QQmlPropertyCache *cache);
    void clearPropertyCachesForVersion(int index);

    QQmlRefPointer<QQmlPropertyCache> propertyCache(const QMetaObject *metaObject, QTypeRevision version);
    QQmlPropertyCache *propertyCache(const QQmlType &type, QTypeRevision version);

    void setTypeRegistrationFailures(QStringList *failures)
    {
        m_typeRegistrationFailures = failures;
    }

    void recordTypeRegFailure(const QString &message)
    {
        if (m_typeRegistrationFailures)
            m_typeRegistrationFailures->append(message);
        else
            qWarning("%s", message.toUtf8().constData());
    }

private:
    QStringList *m_typeRegistrationFailures = nullptr;
};

QT_END_NAMESPACE

#endif // QQMLMETATYPEDATA_P_H
