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

#ifndef QCOMMONSTYLE_P_H
#define QCOMMONSTYLE_P_H

#include "qquickcommonstyle.h"
#include "qquickstyle_p.h"
#include "qquickstyleoption.h"

QT_BEGIN_NAMESPACE

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of qapplication_*.cpp, qwidget*.cpp and qfiledialog.cpp.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

class QTextOption;

namespace QQC2 {

class QCommonStylePrivate : public QStylePrivate
{
    Q_DECLARE_PUBLIC(QCommonStyle)
public:

    ~QCommonStylePrivate()
    {
        delete cachedOption;
    }

    QString calculateElidedText(const QString &text, const QTextOption &textOption,
                                const QFont &font, const QRect &textRect, const Qt::Alignment valign,
                                Qt::TextElideMode textElideMode, int flags,
                                bool lastVisibleLineShouldBeElided, QPointF *paintStartPosition) const;
    void viewItemDrawText(QPainter *p, const QStyleOptionViewItem *option, const QRect &rect) const;
    void viewItemLayout(const QStyleOptionViewItem *opt,  QRect *checkRect,
                        QRect *pixmapRect, QRect *textRect, bool sizehint) const;
    QSize viewItemSize(const QStyleOptionViewItem *option, int role) const;

    mutable QRect decorationRect, displayRect, checkRect;
    mutable QStyleOptionViewItem *cachedOption = nullptr;
    bool isViewItemCached(const QStyleOptionViewItem &option) const {
        return cachedOption
               && option.index == cachedOption->index
               && option.state == cachedOption->state
               && option.rect == cachedOption->rect
               && option.text == cachedOption->text
               && option.direction == cachedOption->direction
               && option.displayAlignment == cachedOption->displayAlignment
               && option.decorationAlignment == cachedOption->decorationAlignment
               && option.decorationPosition == cachedOption->decorationPosition
               && option.decorationSize == cachedOption->decorationSize
               && option.features == cachedOption->features
               && option.icon.isNull() == cachedOption->icon.isNull()
               && option.font == cachedOption->font
               && option.viewItemPosition == cachedOption->viewItemPosition;
    }
    QString toolButtonElideText(const QStyleOptionToolButton *toolbutton,
                                const QRect &textRect, int flags) const;

    mutable QIcon tabBarcloseButtonIcon;
    virtual void tabLayout(const QStyleOptionTab *opt, QRect *textRect, QRect *pixmapRect) const;
};

} // namespace QQC2

QT_END_NAMESPACE

#endif //QCOMMONSTYLE_P_H
