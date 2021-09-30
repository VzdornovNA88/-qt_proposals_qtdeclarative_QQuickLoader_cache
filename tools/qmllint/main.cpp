/****************************************************************************
**
** Copyright (C) 2016 Klaralvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Sergio Martins <sergio.martins@kdab.com>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the plugins of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "findwarnings.h"
#include "codegen.h"
#include "codegenwarninginterface.h"
#include "../shared/qqmltoolingsettings.h"

#include <QtQmlCompiler/private/qqmljsresourcefilemapper_p.h>
#include <QtQmlCompiler/private/qqmljscompiler_p.h>

#include <QtQml/private/qqmljslexer_p.h>
#include <QtQml/private/qqmljsparser_p.h>
#include <QtQml/private/qqmljsengine_p.h>
#include <QtQml/private/qqmljsastvisitor_p.h>
#include <QtQml/private/qqmljsast_p.h>

#include <QtCore/qdebug.h>
#include <QtCore/qfile.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qcoreapplication.h>
#include <QtCore/qdiriterator.h>
#include <QtCore/qjsonobject.h>
#include <QtCore/qjsonarray.h>
#include <QtCore/qjsondocument.h>
#include <QtCore/qscopeguard.h>

#if QT_CONFIG(commandlineparser)
#include <QtCore/qcommandlineparser.h>
#endif

#include <QtCore/qlibraryinfo.h>

#include <cstdio>

constexpr int JSON_LOGGING_FORMAT_REVISION = 1;

static bool lint_file(const QString &filename, const bool silent, QJsonArray *json,
                      const QStringList &qmlImportPaths, const QStringList &qmltypesFiles,
                      const QStringList &resourceFiles,
                      const QMap<QString, QQmlJSLogger::Option> &options, QQmlJSImporter &importer)
{
    QJsonArray warnings;
    QJsonObject result;

    bool success = true;

    QScopeGuard jsonOutput([&] {
        if (!json)
            return;

        result[u"filename"_qs] = QFileInfo(filename).absoluteFilePath();
        result[u"warnings"] = warnings;
        result[u"success"] = success;

        json->append(result);
    });

    auto addJsonWarning = [&](const QQmlJS::DiagnosticMessage &message) {
        QJsonObject jsonMessage;

        QString type;
        switch (message.type) {
        case QtDebugMsg:
            type = "debug";
            break;
        case QtWarningMsg:
            type = "warning";
            break;
        case QtCriticalMsg:
            type = "critical";
            break;
        case QtFatalMsg:
            type = "fatal";
            break;
        case QtInfoMsg:
            type = "info";
            break;
        default:
            type = "unknown";
            break;
        }

        jsonMessage[u"type"_qs] = type;

        if (message.loc.isValid()) {
            jsonMessage[u"line"_qs] = static_cast<int>(message.loc.startLine);
            jsonMessage[u"column"_qs] = static_cast<int>(message.loc.startColumn);
            jsonMessage[u"charOffset"_qs] = static_cast<int>(message.loc.offset);
            jsonMessage[u"length"_qs] = static_cast<int>(message.loc.length);
        }

        jsonMessage[u"message"_qs] = message.message;

        warnings << jsonMessage;
    };

    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) {
        if (json) {
            result[u"openFailed"] = true;
            success = false;
        } else if (!silent) {
            qWarning() << "Failed to open file" << filename << file.error();
        }
        return false;
    }

    QString code = QString::fromUtf8(file.readAll());
    file.close();

    QQmlJS::Engine engine;
    QQmlJS::Lexer lexer(&engine);

    QFileInfo info(filename);
    const QString lowerSuffix = info.suffix().toLower();
    const bool isESModule = lowerSuffix == QLatin1String("mjs");
    const bool isJavaScript = isESModule || lowerSuffix == QLatin1String("js");

    lexer.setCode(code, /*lineno = */ 1, /*qmlMode=*/ !isJavaScript);
    QQmlJS::Parser parser(&engine);

    success = isJavaScript ? (isESModule ? parser.parseModule() : parser.parseProgram())
                           : parser.parse();

    if (!success && !silent) {
        const auto diagnosticMessages = parser.diagnosticMessages();
        for (const QQmlJS::DiagnosticMessage &m : diagnosticMessages) {
            if (json) {
                addJsonWarning(m);
            } else {
                qWarning().noquote() << QString::fromLatin1("%1:%2 : %3")
                                                .arg(filename)
                                                .arg(m.loc.startLine)
                                                .arg(m.message);
            }
        }
    }

    if (success && !isJavaScript) {
        const auto check = [&](QQmlJSResourceFileMapper *mapper) {
            if (importer.importPaths() != qmlImportPaths)
                importer.setImportPaths(qmlImportPaths);

            importer.setResourceFileMapper(mapper);

            QQmlJSLogger logger(filename, code, silent || json);
            FindWarningVisitor v {
                &importer,
                &logger,
                qmltypesFiles,
                engine.comments(),
            };

            for (auto it = options.cbegin(); it != options.cend(); ++it) {
                logger.setCategoryError(it.value().m_category, it.value().m_error);
                logger.setCategoryLevel(it.value().m_category, it.value().m_level);
            }

            parser.rootNode()->accept(&v);
            success = v.check();

            QQmlJSTypeInfo typeInfo;
            Codegen codegen { &importer, filename, qmltypesFiles, &logger, &typeInfo, code };
            QQmlJSSaveFunction saveFunction = [](const QV4::CompiledData::SaveableUnitPointer &,
                                                 const QQmlJSAotFunctionMap &,
                                                 QString *) { return true; };

            QQmlJSCompileError error;

            QLoggingCategory::setFilterRules(u"qt.qml.compiler=false"_qs);

            CodegenWarningInterface interface(&logger);
            qCompileQmlFile(filename, saveFunction, &codegen, &error, true, &interface);

            success &= !logger.hasWarnings() && !logger.hasErrors();

            if (json) {
                for (const auto &error : logger.errors())
                    addJsonWarning(error);
                for (const auto &warning : logger.warnings())
                    addJsonWarning(warning);
                for (const auto &info : logger.infos())
                    addJsonWarning(info);
            }
        };

        if (resourceFiles.isEmpty()) {
            check(nullptr);
        } else {
            QQmlJSResourceFileMapper mapper(resourceFiles);
            check(&mapper);
        }
    }

    return success;
}

int main(int argv, char *argc[])
{
    qSetGlobalQHashSeed(0);
    QMap<QString, QQmlJSLogger::Option> options = QQmlJSLogger::options();

    QCoreApplication app(argv, argc);
    QCoreApplication::setApplicationName("qmllint");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
#if QT_CONFIG(commandlineparser)
    QCommandLineParser parser;
    QQmlToolingSettings settings(QLatin1String("qmllint"));
    parser.setApplicationDescription(QLatin1String(R"(QML syntax verifier and analyzer

All warnings can be set to three levels:
    disabled - Fully disables the warning.
    info - Displays the warning but does not influence the return code.
    warning - Displays the warning and leads to a non-zero exit code if encountered.
)"));

    for (auto it = options.cbegin(); it != options.cend(); ++it) {
        QCommandLineOption option(
                it.key(),
                it.value().m_description
                        + QStringLiteral(" (default: %1)").arg(it.value().levelToString()),
                QStringLiteral("level"), it.value().levelToString());
        parser.addOption(option);
        settings.addOption(QStringLiteral("Warnings/") + it.value().m_settingsName,
                           it.value().levelToString());
    }

    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption silentOption(QStringList() << "s" << "silent",
                                    QLatin1String("Don't output syntax errors"));
    parser.addOption(silentOption);

    QCommandLineOption jsonOption(QStringList() << "json",
                                  QLatin1String("Output linting errors as JSON"));
    parser.addOption(jsonOption);

    QCommandLineOption writeDefaultsOption(
            QStringList() << "write-defaults",
            QLatin1String("Writes defaults settings to .qmllint.ini and exits (Warning: This "
                          "will overwrite any existing settings and comments!)"));
    parser.addOption(writeDefaultsOption);

    QCommandLineOption ignoreSettings(QStringList() << "ignore-settings",
                                      QLatin1String("Ignores all settings files and only takes "
                                                    "command line options into consideration"));
    parser.addOption(ignoreSettings);

    QCommandLineOption resourceOption(
                { QStringLiteral("resource") },
                QStringLiteral("Look for related files in the given resource file"),
                QStringLiteral("resource"));
    parser.addOption(resourceOption);
    const QString &resourceSetting = QLatin1String("ResourcePath");
    settings.addOption(resourceSetting);

    QCommandLineOption qmlImportPathsOption(
            QStringList() << "I"
                          << "qmldirs",
            QLatin1String("Look for QML modules in specified directory"),
            QLatin1String("directory"));
    parser.addOption(qmlImportPathsOption);
    const QString qmlImportPathsSetting = QLatin1String("AdditionalQmlImportPaths");
    settings.addOption(qmlImportPathsSetting);

    QCommandLineOption qmlImportNoDefault(
                QStringList() << "bare",
                QLatin1String("Do not include default import directories or the current directory. "
                              "This may be used to run qmllint on a project using a different Qt version."));
    parser.addOption(qmlImportNoDefault);
    settings.addOption(QLatin1String("DisableDefaultImports"), false);

    QCommandLineOption qmltypesFilesOption(
            QStringList() << "i"
                          << "qmltypes",
            QLatin1String("Import the specified qmltypes files. By default, all qmltypes files "
                          "found in the current directory are used. When this option is set, you "
                          "have to explicitly add files from the current directory if you want "
                          "them to be used."),
            QLatin1String("qmltypes"));
    parser.addOption(qmltypesFilesOption);
    const QString qmltypesFilesSetting = QLatin1String("OverwriteImportTypes");
    settings.addOption(qmltypesFilesSetting);

    parser.addPositionalArgument(QLatin1String("files"),
                                 QLatin1String("list of qml or js files to verify"));

    parser.process(app);

    if (parser.isSet(writeDefaultsOption)) {
        return settings.writeDefaults() ? 0 : 1;
    }

    auto updateLogLevels = [&]() {
        for (auto it = options.begin(); it != options.end(); ++it) {
            const QString &key = it.key();
            const QString &settingsName = QStringLiteral("Warnings/") + it.value().m_settingsName;
            if (parser.isSet(key) || settings.isSet(settingsName)) {
                const QString value = parser.isSet(key) ? parser.value(key)
                                                        : settings.value(settingsName).toString();
                auto &option = it.value();
                if (!option.setLevel(value)) {
                    qWarning() << "Invalid logging level" << value << "provided for" << it.key()
                               << "(allowed are: disable, info, warning)";
                    parser.showHelp(-1);
                }
            }
        }
    };

    updateLogLevels();

    const auto positionalArguments = parser.positionalArguments();
    if (positionalArguments.isEmpty()) {
        parser.showHelp(-1);
    }

    bool silent = parser.isSet(silentOption);
    bool useJson = parser.isSet(jsonOption);

    // use host qml import path as a sane default if not explicitly disabled
    QStringList qmlImportPaths = parser.isSet(qmlImportNoDefault)
            ? QStringList {}
            : QStringList { QLibraryInfo::path(QLibraryInfo::QmlImportsPath), QDir::currentPath() };

    if (parser.isSet(qmlImportPathsOption))
        qmlImportPaths << parser.values(qmlImportPathsOption);

    qmlImportPaths << settings.value(qmlImportPathsSetting).toStringList();

    QStringList qmltypesFiles;
    if (parser.isSet(qmltypesFilesOption)) {
        qmltypesFiles = parser.values(qmltypesFilesOption);
    } else if (settings.isSet(qmltypesFilesSetting)
               && !settings.value(qmltypesFilesSetting).toStringList().isEmpty()) {
        qmltypesFiles = parser.values(qmltypesFilesSetting);
    } else {
        // If none are given explicitly, use the qmltypes files from the current directory.
        QDirIterator it(".", {"*.qmltypes"}, QDir::Files);
        while (it.hasNext()) {
            it.next();
            qmltypesFiles.append(it.fileInfo().absoluteFilePath());
        }
    }

    QStringList resourceFiles;
    if (parser.isSet(resourceOption))
        resourceFiles << parser.values(resourceOption);
    else if (settings.isSet(resourceSetting))
        resourceFiles << settings.value(resourceSetting).toStringList();

#else
    bool silent = false;
    bool useJson = false;
    bool warnUnqualified = true;
    bool warnWithStatement = true;
    bool warnInheritanceCycle = true;
    QStringList qmlImportPaths {};
    QStringList qmltypesFiles {};
    QStringList resourceFiles {};
#endif
    bool success = true;
    QQmlJSImporter importer(qmlImportPaths, nullptr);

    QJsonArray jsonFiles;

#if QT_CONFIG(commandlineparser)
    for (const QString &filename : positionalArguments) {
        if (!parser.isSet(ignoreSettings)) {
            settings.search(filename);
            updateLogLevels();
        }
#else
    const auto arguments = app.arguments();
    for (const QString &filename : arguments) {
#endif
        success &= lint_file(filename, silent, useJson ? &jsonFiles : nullptr, qmlImportPaths,
                             qmltypesFiles, resourceFiles, options, importer);
    }

    if (useJson) {
        QJsonObject result;

        result[u"revision"_qs] = JSON_LOGGING_FORMAT_REVISION;
        result[u"files"_qs] = jsonFiles;

        QTextStream(stdout) << QString::fromUtf8(
                QJsonDocument(result).toJson(QJsonDocument::Compact));
    }

    return success ? 0 : -1;
}
