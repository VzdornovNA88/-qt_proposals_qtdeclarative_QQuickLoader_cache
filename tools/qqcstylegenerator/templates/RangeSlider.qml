import QtQuick
import QtQuick.FigmaStyle
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.RangeSlider {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            first.implicitHandleWidth + leftPadding + rightPadding,
                            second.implicitHandleWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             first.implicitHandleHeight + topPadding + bottomPadding,
                             second.implicitHandleHeight + topPadding + bottomPadding)

    topPadding: (control.horizontal ? config.topPadding : config.rightPadding) || 0
    leftPadding: (control.horizontal ? config.leftPadding : config.bottomPadding) || 0
    rightPadding: (control.horizontal ? config.rightPadding : config.topPadding) || 0
    bottomPadding: (control.horizontal ? config.bottomPadding : config.leftPadding) || 0

    topInset: (control.horizontal ? -config.topInset : config.rightInset) || 0
    leftInset: (control.horizontal ? -config.leftInset : config.bottomInset) || 0
    rightInset: (control.horizontal ? -config.rightInset : config.topInset) || 0
    bottomInset: (control.horizontal ? -config.bottomInset : config.leftInset) || 0

    property string __controlState: [
        visualFocus && "focused",
        !control.enabled && "disabled",
        control.hovered && !(first.pressed || second.pressed) && "hovered",
    ].filter(Boolean).join("-") || "normal"
    readonly property var config: ConfigReader.controls.rangeslider[__controlState] || {}

    property string __firstHandleState: [
        first.hovered && !first.pressed && "hovered",
        first.pressed && "handle-pressed",
        visualFocus && "focused",
    ].filter(Boolean).join("-") || "normal"
    readonly property var firstHandleConfig: ConfigReader.controls.rangeslider[__firstHandleState] || {}

    property string __secondHandleState: [
        second.hovered && !second.pressed && "hovered",
        second.pressed && "handle-pressed",
        visualFocus && "focused",
    ].filter(Boolean).join("-") || "normal"
    readonly property var secondHandleConfig: ConfigReader.controls.rangeslider[__secondHandleState] || {}

    first.handle: BorderImage {
        x: Math.round(control.leftPadding + (control.horizontal ? control.first.visualPosition * (control.availableWidth - width) : (control.availableWidth - width) / 2))
        y: Math.round(control.topPadding + (control.horizontal ? (control.availableHeight - height) / 2 : control.first.visualPosition * (control.availableHeight - height)))
        source: Qt.resolvedUrl(control.firstHandleConfig.first_handle.filePath)
        border {
            top: control.firstHandleConfig.first_handle?.topOffset || 0
            bottom: control.firstHandleConfig.first_handle?.bottomOffset || 0
            left: control.firstHandleConfig.first_handle?.leftOffset || 0
            right: control.firstHandleConfig.first_handle?.rightOffset || 0
        }
    }

    second.handle: BorderImage {
        x: control.leftPadding + (control.horizontal ? control.second.visualPosition * (control.availableWidth - width) : (control.availableWidth - width) / 2)
        y: control.topPadding + (control.horizontal ? (control.availableHeight - height) / 2 : control.second.visualPosition * (control.availableHeight - height))
        source: Qt.resolvedUrl(control.secondHandleConfig.second_handle.filePath)
        border {
            top: control.secondHandleConfig.second_handle?.topOffset || 0
            bottom: control.secondHandleConfig.second_handle?.bottomOffset || 0
            left: control.secondHandleConfig.second_handle?.leftOffset || 0
            right: control.secondHandleConfig.second_handle?.rightOffset || 0
        }
    }

    background: Item {
        implicitWidth: control.horizontal ? _background.implicitWidth : _background.implicitHeight
        implicitHeight: control.horizontal ? _background.implicitHeight : _background.implicitWidth

        property BorderImage _background: BorderImage {
            parent: control.background
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: control.horizontal ? background.width : background.height
            height: control.horizontal ? background.height : background.width
            rotation: control.horizontal ? 0 : -90
            scale: control.horizontal && control.mirrored ? -1 : 1
            source: Qt.resolvedUrl(control.config.background.filePath)
            border {
                top: control.config.background?.topOffset || 0
                bottom: control.config.background?.bottomOffset || 0
                left: control.config.background?.leftOffset || 0
                right: control.config.background?.rightOffset || 0
            }

            property BorderImage groove: BorderImage {
                parent: control.background._background
                x: (control.horizontal ? control.leftPadding : control.bottomPadding) + ((control.horizontal ? control.availableWidth : control.availableHeight) - width) / 2
                y: (control.horizontal ? control.topPadding : control.rightPadding) + ((control.horizontal ? control.availableHeight : control.availableWidth) - height) / 2
                width: control.horizontal ? control.availableWidth : control.availableHeight
                height: implicitHeight
                source: Qt.resolvedUrl(control.config.groove.filePath)
                border {
                    top: control.config.groove?.topOffset || 0
                    bottom: control.config.groove?.bottomOffset || 0
                    left: control.config.groove?.leftOffset || 0
                    right: control.config.groove?.rightOffset || 0
                }

                property BorderImage track: BorderImage {
                    parent: control.background._background.groove
                    x: control.first.position * (parent.width - control.first.handle.width / 2)
                    y: (parent.height - height) / 2
                    width: control.second.position * (parent.width - control.first.handle.width) - control.first.position * (parent.width - control.first.handle.width)
                    height: parent.height
                    source: Qt.resolvedUrl(control.config.track.filePath)
                    border {
                        top: control.config.track?.topOffset || 0
                        bottom: control.config.track?.bottomOffset || 0
                        left: control.config.track?.leftOffset || 0
                        right: control.config.track?.rightOffset || 0
                    }
                }
            }
        }
    }
}
