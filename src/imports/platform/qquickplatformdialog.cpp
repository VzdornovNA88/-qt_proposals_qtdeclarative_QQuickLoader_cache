/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Labs Platform module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qquickplatformdialog_p.h"

#include <QtQuick/qquickitem.h>
#include <QtQuick/qquickwindow.h>

QT_BEGIN_NAMESPACE

/*!
    \qmltype Dialog
    \inherits QtObject
    \instantiates QQuickPlatformDialog
    \inqmlmodule Qt.labs.platform
    \since 5.8
    \brief The base class of native dialogs.

    The Dialog type provides common QML API for native platform dialogs.

    To show a native dialog, construct an instance of one of the concrete
    Dialog implementations, set the desired properties, and call \l open().
    Dialog emits \l accepted() or \l rejected() when the user is done with
    the dialog.

    \labs
*/

/*!
    \qmlsignal void Qt.labs.platform::Dialog::accepted()

    This signal is emitted when the dialog has been accepted either
    interactively or by calling \l accept().

    \note This signal is \e not emitted when closing the dialog with \l close().

    \sa rejected()
*/

/*!
    \qmlsignal void QtQuick.Controls::Dialog::rejected()

    This signal is emitted when the dialog has been rejected either
    interactively or by calling \l reject().

    \note This signal is \e not emitted when closing the dialog with \l close().

    \sa accepted()
*/

QQuickPlatformDialog::QQuickPlatformDialog(QObject *parent)
    : QObject(parent),
      m_complete(false),
      m_parentWindow(nullptr),
      m_flags(Qt::Dialog),
      m_modality(Qt::WindowModal),
      m_handle(nullptr)
{
}

QQuickPlatformDialog::~QQuickPlatformDialog()
{
    delete m_handle;
    m_handle = nullptr;
}

QPlatformDialogHelper *QQuickPlatformDialog::handle() const
{
    return m_handle;
}

void QQuickPlatformDialog::setHandle(QPlatformDialogHelper *handle)
{
    if (m_handle == handle)
        return;

    if (m_handle) {
        disconnect(m_handle, &QPlatformDialogHelper::accept, this, &QQuickPlatformDialog::accept);
        disconnect(m_handle, &QPlatformDialogHelper::reject, this, &QQuickPlatformDialog::reject);
    }
    m_handle = handle;
    if (handle) {
        connect(handle, &QPlatformDialogHelper::accept, this, &QQuickPlatformDialog::accept);
        connect(handle, &QPlatformDialogHelper::reject, this, &QQuickPlatformDialog::reject);
    }
}

/*!
    \default
    \qmlproperty list<Object> Qt.labs.platform::Dialog::data

    This default property holds the list of all objects declared as children of
    the dialog.
*/
QQmlListProperty<QObject> QQuickPlatformDialog::data()
{
    return QQmlListProperty<QObject>(this, m_data);
}

/*!
    \qmlproperty Window Qt.labs.platform::Dialog::parentWindow

    This property holds the parent window of the dialog.

    Unless explicitly set, the window is automatically resolved by iterating
    the QML parent objects until a \l Window or an \l Item that has a window
    is found.
*/
QWindow *QQuickPlatformDialog::parentWindow() const
{
    return m_parentWindow;
}

void QQuickPlatformDialog::setParentWindow(QWindow *window)
{
    if (m_parentWindow == window)
        return;

    m_parentWindow = window;
    emit parentWindowChanged();
}

/*!
    \qmlproperty string Qt.labs.platform::Dialog::title

    This property holds the title of the dialog.
*/
QString QQuickPlatformDialog::title() const
{
    return m_title;
}

void QQuickPlatformDialog::setTitle(const QString &title)
{
    if (m_title == title)
        return;

    m_title = title;
    emit titleChanged();
}

/*!
    \qmlproperty Qt::WindowFlags Qt.labs.platform::Dialog::flags

    This property holds the window flags of the dialog. The default value is \c Qt.Dialog.
*/
Qt::WindowFlags QQuickPlatformDialog::flags() const
{
    return m_flags;
}

void QQuickPlatformDialog::setFlags(Qt::WindowFlags flags)
{
    if (m_flags == flags)
        return;

    m_flags = flags;
    emit flagsChanged();
}

/*!
    \qmlproperty Qt::WindowModality Qt.labs.platform::Dialog::modality

    This property holds the modality of the dialog. The default value is \c Qt.WindowModal.

    Available values:
    \value Qt.NonModal The dialog is not modal and does not block input to other windows.
    \value Qt.WindowModal The dialog is modal to a single window hierarchy and blocks input to its parent window, all grandparent windows, and all siblings of its parent and grandparent windows.
    \value Qt.ApplicationModal The dialog is modal to the application and blocks input to all windows.
*/
Qt::WindowModality QQuickPlatformDialog::modality() const
{
    return m_modality;
}

void QQuickPlatformDialog::setModality(Qt::WindowModality modality)
{
    if (m_modality == modality)
        return;

    m_modality = modality;
    emit modalityChanged();
}

/*!
    \qmlmethod void Qt.labs.platform::Dialog::open()

    Opens the dialog.

    \sa close()
*/
void QQuickPlatformDialog::open()
{
    if (!m_handle)
        return;

    applyOptions();
    m_handle->show(m_flags, m_modality, m_parentWindow);
}

/*!
    \qmlmethod void Qt.labs.platform::Dialog::close()

    Closes the dialog.

    \sa open()
*/
void QQuickPlatformDialog::close()
{
    if (!m_handle)
        return;

    m_handle->hide();
}

/*!
    \qmlmethod void Qt.labs.platform::Dialog::accept()

    Closes the dialog and emits the \l accepted() signal.

    \sa reject()
*/
void QQuickPlatformDialog::accept()
{
    close();
    emit accepted();
}

/*!
    \qmlmethod void QtQuick.Controls::Dialog::reject()

    Closes the dialog and emits the \l rejected() signal.

    \sa accept()
*/
void QQuickPlatformDialog::reject()
{
    close();
    emit rejected();
}

void QQuickPlatformDialog::classBegin()
{
}

void QQuickPlatformDialog::componentComplete()
{
    m_complete = true;
    if (!m_parentWindow)
        setParentWindow(findParentWindow());
}

void QQuickPlatformDialog::applyOptions()
{
}

QWindow *QQuickPlatformDialog::findParentWindow() const
{
    QObject *obj = parent();
    while (obj) {
        QWindow *window = qobject_cast<QWindow *>(obj);
        if (window)
            return window;
        QQuickItem *item = qobject_cast<QQuickItem *>(obj);
        if (item && item->window())
            return item->window();
        obj = obj->parent();
    }
    return nullptr;
}

QT_END_NAMESPACE
