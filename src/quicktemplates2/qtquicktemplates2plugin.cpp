/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Quick Templates 2 module of the Qt Toolkit.
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
******************************************************************************/

#include <QtQml/qqmlextensionplugin.h>
#include <QtQml/private/qqmlglobal_p.h>
#include <QtQuickTemplates2/private/qtquicktemplates2global_p.h>

#if QT_CONFIG(shortcut)
#include <QtQuickTemplates2/private/qquickshortcutcontext_p_p.h>

// qtdeclarative/src/quick/util/qquickshortcut.cpp
typedef bool (*ShortcutContextMatcher)(QObject *, Qt::ShortcutContext);
extern ShortcutContextMatcher qt_quick_shortcut_context_matcher();
extern void qt_quick_set_shortcut_context_matcher(ShortcutContextMatcher matcher);
#endif

Q_GHS_KEEP_REFERENCE(qml_register_types_QtQuick_Templates);
Q_GHS_KEEP_REFERENCE(QQuickTemplates_initializeModule);


QT_BEGIN_NAMESPACE

class QtQuickTemplates2Plugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    QtQuickTemplates2Plugin(QObject *parent = nullptr);
    ~QtQuickTemplates2Plugin();

    void registerTypes(const char *uri) override;
    void unregisterTypes() override;

private:
    bool registered;
#if QT_CONFIG(shortcut)
    ShortcutContextMatcher originalContextMatcher;
#endif
};

QtQuickTemplates2Plugin::QtQuickTemplates2Plugin(QObject *parent)
    : QQmlExtensionPlugin(parent), registered(false)
{
    volatile auto registration = &qml_register_types_QtQuick_Templates;
    volatile auto initialization = &QQuickTemplates_initializeModule;

    Q_UNUSED(registration)
    Q_UNUSED(initialization)
}

QtQuickTemplates2Plugin::~QtQuickTemplates2Plugin()
{
    // Intentionally empty: we use register/unregisterTypes() to do
    // initialization and cleanup, as plugins are not unloaded on macOS.
}

void QtQuickTemplates2Plugin::registerTypes(const char * /*uri*/)
{
#if QT_CONFIG(shortcut)
    originalContextMatcher = qt_quick_shortcut_context_matcher();
    qt_quick_set_shortcut_context_matcher(QQuickShortcutContext::matcher);
#endif

    registered = true;
}

void QtQuickTemplates2Plugin::unregisterTypes()
{
#if QT_CONFIG(shortcut)
    qt_quick_set_shortcut_context_matcher(originalContextMatcher);
#endif
}

QT_END_NAMESPACE

#include "qtquicktemplates2plugin.moc"
