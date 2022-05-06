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

#ifndef QQMLTYPELOADER_P_H
#define QQMLTYPELOADER_P_H

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

#include <private/qqmldatablob_p.h>
#include <private/qqmlimport_p.h>
#include <private/qqmlmetatype_p.h>

#include <QtQml/qtqmlglobal.h>
#include <QtQml/qqmlerror.h>

#include <QtCore/qcache.h>
#include <QtCore/qmutex.h>

#include <memory>

QT_BEGIN_NAMESPACE

class QQmlScriptBlob;
class QQmlQmldirData;
class QQmlTypeData;
class QQmlEngineExtensionInterface;
class QQmlExtensionInterface;
class QQmlProfiler;
class QQmlTypeLoaderThread;
class QQmlEngine;

class Q_QML_PRIVATE_EXPORT QQmlTypeLoader
{
    Q_DECLARE_TR_FUNCTIONS(QQmlTypeLoader)
public:
    enum Mode { PreferSynchronous, Asynchronous, Synchronous };

    class Q_QML_PRIVATE_EXPORT Blob : public QQmlDataBlob
    {
    public:
        Blob(const QUrl &url, QQmlDataBlob::Type type, QQmlTypeLoader *loader);
        ~Blob() override;

        const QQmlImports &imports() const { return m_importCache; }

        void setCachedUnitStatus(QQmlMetaType::CachedUnitLookupError status) { m_cachedUnitStatus = status; }

        struct PendingImport
        {
            QV4::CompiledData::Import::ImportType type = QV4::CompiledData::Import::ImportType::ImportLibrary;

            QString uri;
            QString qualifier;

            QTypeRevision version;

            QV4::CompiledData::Location location;

            int priority = 0;

            PendingImport() = default;
            PendingImport(Blob *blob, const QV4::CompiledData::Import *import);
        };
        using PendingImportPtr = std::shared_ptr<PendingImport>;

    protected:
        bool addImport(const QV4::CompiledData::Import *import, uint flags,
                       QList<QQmlError> *errors);
        bool addImport(PendingImportPtr import, uint flags, QList<QQmlError> *errors);

        bool fetchQmldir(const QUrl &url, PendingImportPtr import, int priority, QList<QQmlError> *errors);
        bool updateQmldir(const QQmlRefPointer<QQmlQmldirData> &data, PendingImportPtr import, QList<QQmlError> *errors);

    private:
        virtual bool qmldirDataAvailable(const QQmlRefPointer<QQmlQmldirData> &, QList<QQmlError> *);

        virtual void scriptImported(const QQmlRefPointer<QQmlScriptBlob> &, const QV4::CompiledData::Location &, const QString &, const QString &) {}

        void dependencyComplete(QQmlDataBlob *) override;

        bool loadImportDependencies(PendingImportPtr currentImport, const QString &qmldirUri, QList<QQmlError> *errors);

    protected:
        virtual QString stringAt(int) const { return QString(); }

        bool isDebugging() const;
        bool diskCacheEnabled() const;

        QQmlImports m_importCache;
        QVector<PendingImportPtr> m_unresolvedImports;
        QVector<QQmlRefPointer<QQmlQmldirData>> m_qmldirs;
        QQmlMetaType::CachedUnitLookupError m_cachedUnitStatus = QQmlMetaType::CachedUnitLookupError::NoError;
    };

    QQmlTypeLoader(QQmlEngine *);
    ~QQmlTypeLoader();

    QQmlImportDatabase *importDatabase() const;

    static QUrl normalize(const QUrl &unNormalizedUrl);

    QQmlRefPointer<QQmlTypeData> getType(const QUrl &unNormalizedUrl, Mode mode = PreferSynchronous);
    QQmlRefPointer<QQmlTypeData> getType(const QByteArray &, const QUrl &url, Mode mode = PreferSynchronous);

    QQmlRefPointer<QQmlScriptBlob> getScript(const QUrl &unNormalizedUrl);
    QQmlRefPointer<QQmlQmldirData> getQmldir(const QUrl &);

    QString absoluteFilePath(const QString &path);
    bool fileExists(const QString &path, const QString &file);
    bool directoryExists(const QString &path);

    const QQmlTypeLoaderQmldirContent qmldirContent(const QString &filePath);
    void setQmldirContent(const QString &filePath, const QString &content);

    void clearCache();
    void trimCache();

    bool isTypeLoaded(const QUrl &url) const;
    bool isScriptLoaded(const QUrl &url) const;

    void lock() { m_mutex.lock(); }
    void unlock() { m_mutex.unlock(); }

    void load(QQmlDataBlob *, Mode = PreferSynchronous);
    void loadWithStaticData(QQmlDataBlob *, const QByteArray &, Mode = PreferSynchronous);
    void loadWithCachedUnit(QQmlDataBlob *blob, const QQmlPrivate::CachedQmlUnit *unit, Mode mode = PreferSynchronous);

    QQmlEngine *engine() const;
    void initializeEngine(QQmlEngineExtensionInterface *, const char *);
    void initializeEngine(QQmlExtensionInterface *, const char *);
    void invalidate();

#if !QT_CONFIG(qml_debug)
    quintptr profiler() const { return 0; }
    void setProfiler(quintptr) {}
#else
    QQmlProfiler *profiler() const { return m_profiler.data(); }
    void setProfiler(QQmlProfiler *profiler);
#endif // QT_CONFIG(qml_debug)


private:
    friend class QQmlDataBlob;
    friend class QQmlTypeLoaderThread;
#if QT_CONFIG(qml_network)
    friend class QQmlTypeLoaderNetworkReplyProxy;
#endif // qml_network

    void shutdownThread();

    void loadThread(QQmlDataBlob *);
    void loadWithStaticDataThread(QQmlDataBlob *, const QByteArray &);
    void loadWithCachedUnitThread(QQmlDataBlob *blob, const QQmlPrivate::CachedQmlUnit *unit);
#if QT_CONFIG(qml_network)
    void networkReplyFinished(QNetworkReply *);
    void networkReplyProgress(QNetworkReply *, qint64, qint64);

    typedef QHash<QNetworkReply *, QQmlDataBlob *> NetworkReplies;
#endif

    void setData(QQmlDataBlob *, const QByteArray &);
    void setData(QQmlDataBlob *, const QString &fileName);
    void setData(QQmlDataBlob *, const QQmlDataBlob::SourceCodeData &);
    void setCachedUnit(QQmlDataBlob *blob, const QQmlPrivate::CachedQmlUnit *unit);

    template<typename T>
    struct TypedCallback
    {
        TypedCallback(T *object, void (T::*func)(QQmlTypeData *)) : o(object), mf(func) {}

        static void redirect(void *arg, QQmlTypeData *type)
        {
            TypedCallback<T> *self = reinterpret_cast<TypedCallback<T> *>(arg);
            ((self->o)->*(self->mf))(type);
        }

    private:
        T *o;
        void (T::*mf)(QQmlTypeData *);
    };

    typedef QHash<QUrl, QQmlTypeData *> TypeCache;
    typedef QHash<QUrl, QQmlScriptBlob *> ScriptCache;
    typedef QHash<QUrl, QQmlQmldirData *> QmldirCache;
    typedef QCache<QString, QCache<QString, bool> > ImportDirCache;
    typedef QStringHash<QQmlTypeLoaderQmldirContent *> ImportQmlDirCache;

    QQmlEngine *m_engine;
    QQmlTypeLoaderThread *m_thread;
    QMutex &m_mutex;

#if QT_CONFIG(qml_debug)
    QScopedPointer<QQmlProfiler> m_profiler;
#endif

#if QT_CONFIG(qml_network)
    NetworkReplies m_networkReplies;
#endif
    TypeCache m_typeCache;
    int m_typeCacheTrimThreshold;
    ScriptCache m_scriptCache;
    QmldirCache m_qmldirCache;
    ImportDirCache m_importDirCache;
    ImportQmlDirCache m_importQmlDirCache;

    template<typename Loader>
    void doLoad(const Loader &loader, QQmlDataBlob *blob, Mode mode);
    void updateTypeCacheTrimThreshold();

    friend struct PlainLoader;
    friend struct CachedLoader;
    friend struct StaticLoader;
};

QT_END_NAMESPACE

#endif // QQMLTYPELOADER_P_H
