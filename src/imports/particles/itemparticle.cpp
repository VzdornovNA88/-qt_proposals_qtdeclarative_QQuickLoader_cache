/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Declarative module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "itemparticle.h"
#include <QtDeclarative/private/qsgvisualitemmodel_p.h>
#include <qsgnode.h>
#include <QDebug>

QT_BEGIN_NAMESPACE

ItemParticle::ItemParticle(QSGItem *parent) :
    ParticleType(parent), m_fade(true)
{
    setFlag(QSGItem::ItemHasContents);
}


void ItemParticle::freeze(QSGItem* item)
{
    m_stasis << item;
}


void ItemParticle::unfreeze(QSGItem* item)
{
    m_stasis.remove(item);
}

void ItemParticle::take(QSGItem *item, bool prioritize)
{
    if(prioritize)
        m_pendingItems.push_front(item);
    else
        m_pendingItems.push_back(item);
}

void ItemParticle::give(QSGItem *item)
{
    //TODO: This
}

void ItemParticle::load(ParticleData* d)
{
    if(m_pendingItems.isEmpty())
        return;
    int pos = particleTypeIndex(d);
    if(m_items[pos]){
        if(m_stasis.contains(m_items[pos]))
            qWarning() << "Current model particles prefers overwrite:false";
        //remove old item from the particle that is dying to make room for this one
        m_items[pos]->setOpacity(0.);
        ItemParticleAttached* mpa;
        if((mpa = qobject_cast<ItemParticleAttached*>(qmlAttachedPropertiesObject<ItemParticle>(m_items[pos], false))))
            mpa->detach();//reparent as well?
        m_items[pos] = 0;
        m_data[pos] = 0;
        m_activeCount--;
    }
    m_items[pos] = m_pendingItems.front();
    m_pendingItems.pop_front();
    m_items[pos]->setX(d->curX() - m_items[pos]->width()/2);
    m_items[pos]->setY(d->curY() - m_items[pos]->height()/2);
    ItemParticleAttached* mpa = qobject_cast<ItemParticleAttached*>(qmlAttachedPropertiesObject<ItemParticle>(m_items[pos]));
    if(mpa){
        mpa->m_mp = this;
        mpa->attach();
    }
    m_items[pos]->setParentItem(this);
    m_data[pos] = d;
    m_activeCount++;
}

void ItemParticle::reload(ParticleData* d)
{
    //No-op unless we start copying the data.
}

void ItemParticle::setCount(int c)
{
    ParticleType::setCount(c);//###Do we need our own?
    m_particleCount = c;
    reset();
}

int ItemParticle::count()
{
    return m_particleCount;
}

void ItemParticle::reset()
{
    ParticleType::reset();
    //TODO: Cleanup items?
    m_items.resize(m_particleCount);
    m_data.resize(m_particleCount);
    m_items.fill(0);
    m_data.fill(0);
    //m_pendingItems.clear();//TODO: Should this be done? If so, Emit signal?
}


QSGNode* ItemParticle::updatePaintNode(QSGNode* n, UpdatePaintNodeData* d)
{
    //Dummy update just to get painting tick
    if(m_pleaseReset){
        m_pleaseReset = false;
        reset();
    }
    prepareNextFrame();

    update();//Get called again
    if(n)
        n->markDirty(QSGNode::DirtyMaterial);
    return QSGItem::updatePaintNode(n,d);
}

void ItemParticle::prepareNextFrame()
{
    qint64 timeStamp = m_system->systemSync(this);
    qreal curT = timeStamp/1000.0;
    qreal dt = curT - m_lastT;
    m_lastT = curT;
    if(!m_activeCount)
        return;

    //TODO: Size, better fade?
    for(int i=0; i<m_particleCount; i++){
        QSGItem* item = m_items[i];
        ParticleData* data = m_data[i];
        if(!item || !data)
            continue;
        qreal t = ((timeStamp/1000.0) - data->pv.t) / data->pv.lifeSpan;
        if(m_stasis.contains(item)) {
            m_data[i]->pv.t += dt;//Stasis effect
            continue;
        }
        if(t >= 1.0){//Usually happens from load
            item->setOpacity(0.);
            ItemParticleAttached* mpa;
            if((mpa = qobject_cast<ItemParticleAttached*>(qmlAttachedPropertiesObject<ItemParticle>(m_items[i]))))
                mpa->detach();//reparent as well?
            m_items[i] = 0;
            m_data[i] = 0;
            m_activeCount--;
        }else{//Fade
            if(m_fade){
                qreal o = 1.;
                if(t<0.2)
                    o = t*5;
                if(t>0.8)
                    o = (1-t)*5;
                item->setOpacity(o);
            }else{
                item->setOpacity(1.);//###Without fade, it's just a binary toggle - if we turn it off we have to turn it back on
            }
        }
        item->setX(data->curX() - item->width()/2);
        item->setY(data->curY() - item->height()/2);
    }
}

ItemParticleAttached *ItemParticle::qmlAttachedProperties(QObject *object)
{
    return new ItemParticleAttached(object);
}

QT_END_NAMESPACE
