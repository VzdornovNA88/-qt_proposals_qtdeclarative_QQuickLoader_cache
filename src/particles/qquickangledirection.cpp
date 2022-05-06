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

#include "qquickangledirection_p.h"
#include <QRandomGenerator>
#include <qmath.h>

QT_BEGIN_NAMESPACE
const qreal CONV = 0.017453292519943295;
/*!
    \qmltype AngleDirection
    \instantiates QQuickAngleDirection
    \inqmlmodule QtQuick.Particles
    \ingroup qtquick-particles
    \inherits Direction
    \brief For specifying a direction that varies in angle.

    The AngledDirection element allows both the specification of a direction by angle and magnitude,
    as well as varying the parameters by angle or magnitude.
*/
/*!
    \qmlproperty real QtQuick.Particles::AngleDirection::angle
    This property specifies the base angle for the direction.
    The angle of this direction will vary by no more than angleVariation
    from this angle.

    Angle is specified by degrees clockwise from straight right.

    The default value is zero.
*/
/*!
    \qmlproperty real QtQuick.Particles::AngleDirection::magnitude
    This property specifies the base magnitude for the direction.
    The magnitude of this direction will vary by no more than magnitudeVariation
    from this magnitude.

    Magnitude is specified in units of pixels per second.

    The default value is zero.
*/
/*!
    \qmlproperty real QtQuick.Particles::AngleDirection::angleVariation
    This property specifies the maximum angle variation for the direction.
    The angle of the direction will vary by up to angleVariation clockwise
    and anticlockwise from the value specified in angle.

    Angle is specified by degrees clockwise from straight right.

    The default value is zero.
*/
/*!
    \qmlproperty real QtQuick.Particles::AngleDirection::magnitudeVariation
    This property specifies the base magnitude for the direction.
    The magnitude of this direction will vary by no more than magnitudeVariation
    from the base magnitude.

    Magnitude is specified in units of pixels per second.

    The default value is zero.
*/
QQuickAngleDirection::QQuickAngleDirection(QObject *parent) :
    QQuickDirection(parent)
  , m_angle(0)
  , m_magnitude(0)
  , m_angleVariation(0)
  , m_magnitudeVariation(0)
{

}

QPointF QQuickAngleDirection::sample(const QPointF &from)
{
    Q_UNUSED(from);
    QPointF ret;
    qreal theta = m_angle*CONV - m_angleVariation*CONV + QRandomGenerator::global()->generateDouble() * m_angleVariation*CONV * 2;
    qreal mag = m_magnitude- m_magnitudeVariation + QRandomGenerator::global()->generateDouble() * m_magnitudeVariation * 2;
    ret.setX(mag * qCos(theta));
    ret.setY(mag * qSin(theta));
    return ret;
}

QT_END_NAMESPACE

#include "moc_qquickangledirection_p.cpp"
