/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Quick Controls 2 module of the Qt Toolkit.
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

import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.Imagine
import QtQuick.Controls.Imagine.impl

T.GroupBox {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding,
                            implicitLabelWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    topPadding: ((background as NinePatchImage)?.topPadding ?? 0) + (implicitLabelWidth > 0 ? implicitLabelHeight + spacing : 0)
    leftPadding: ((background as NinePatchImage)?.leftPadding ?? 0)
    rightPadding: ((background as NinePatchImage)?.rightPadding ?? 0)
    bottomPadding: ((background as NinePatchImage)?.bottomPadding ?? 0)

    label: Label {
        width: control.width

        topPadding: background.topPadding
        leftPadding: background.leftPadding
        rightPadding: background.rightPadding
        bottomPadding: background.bottomPadding

        text: control.title
        font: control.font
        elide: Text.ElideRight
        verticalAlignment: Text.AlignVCenter

        color: control.palette.windowText

        background: NinePatchImage {
            width: parent.width
            height: parent.height

            source: Imagine.url + "groupbox-title"
            NinePatchImageSelector on source {
                states: [
                    {"disabled": !control.enabled},
                    {"mirrored": control.mirrored}
                ]
            }
        }
    }

    background: NinePatchImage {
        x: -leftInset
        y: control.topPadding - control.bottomPadding - topInset
        width: control.width + leftInset + rightInset
        height: control.height + topInset + bottomInset - control.topPadding + control.bottomPadding

        source: Imagine.url + "groupbox-background"
        NinePatchImageSelector on source {
            states: [
                {"disabled": !control.enabled},
                {"mirrored": control.mirrored}
            ]
        }
    }
}
