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

#ifndef QQUICKSTYLEITEMSCROLLVIEWCORNER_H
#define QQUICKSTYLEITEMSCROLLVIEWCORNER_H

#include "qquickstyleitem.h"
#include <QtQuickTemplates2/private/qquickscrollbar_p.h>

class QQuickStyleItemScrollViewCorner : public QQuickStyleItem
{
    Q_OBJECT
    QML_NAMED_ELEMENT(ScrollViewCorner)

protected:
    void paintEvent(QPainter *painter) const override;
    StyleItemGeometry calculateGeometry() override;

private:
    void initStyleOption(QStyleOptionSlider &styleOption) const;
};

#endif // QQUICKSTYLEITEMSCROLLVIEWCORNER_H
