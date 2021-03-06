/*  Copyright (C) Tim van Mourik, 2014-2016
    Copyright (C) Tim van Mourik, 2017, DCCN
    All rights reserved

 This file is part of the Porcupine pipeline tool, see
 https://github.com/TimVanMourik/Porcupine for the documentation and
 details.

    This toolbox is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This toolbox is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the fmri analysis toolbox. If not, see
    <http://www.gnu.org/licenses/>.
*/

#include <QGraphicsScene>
#include <QPainter>

#include "Node.hpp"
#include "Preferences.hpp"
#include "SelectionBox.hpp"

qreal SelectionBox::s_rounding = 12;

SelectionBox::SelectionBox(
        const QPointF& _mousePosition,
        QGraphicsScene* _scene
        ) :
    m_topLeftCorner(_mousePosition)
{
    ///@todo deleting a selection may crash the program
    _scene->addItem(this);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    reshape(m_topLeftCorner);
}

void SelectionBox::paint(
        QPainter* _painter,
        const QStyleOptionGraphicsItem* _option,
        QWidget* _widget
        )
{
    Q_UNUSED(_painter);
    Q_UNUSED(_option);
    Q_UNUSED(_widget);

    Preferences& preferences = Preferences::getInstance();
    if(isSelected())
    {
        _painter->setPen(preferences.m_selectionPenSelected);
        _painter->setBrush(preferences.m_selectionBrushSelected);
        _painter->setOpacity(preferences.m_selectionOpacity);
    }
    else
    {
        _painter->setPen(preferences.m_selectionPenUnselected);
        _painter->setBrush(preferences.m_selectionBrushUnselected);
        _painter->setOpacity(preferences.m_selectionOpacity);
    }
    _painter->drawPath(path());
}

int SelectionBox::type(
        ) const
{
    return Type;
}

void SelectionBox::reshape(
        const QPointF& _mousePosition
        )
{
    QPainterPath path;
    path.addRoundedRect(QRect((int)m_topLeftCorner.x(),(int) m_topLeftCorner.y(), _mousePosition.x() - m_topLeftCorner.x(), _mousePosition.y() - m_topLeftCorner.y()), s_rounding, s_rounding);
    setPath(path);
}

void SelectionBox::reshape(
        const qreal _x1,
        const qreal _y1,
        const qreal _x2,
        const qreal _y2
        )
{
    QPainterPath path;
    path.addRoundedRect(_x1, _y1, _x2 - _x1, _y2 - _y1, s_rounding, s_rounding);
    setPath(path);
}

void SelectionBox::updateOpacity(
        float _scalingFactor
        )
{
    float nodeOpacity = qMin(qMax((float)0.2, _scalingFactor), (float)1.0);
    foreach (Node* node, m_nodeList)
    {
        node->setOpacity(nodeOpacity);
    }
}

void SelectionBox::setNodeList(
        QList<Node*>& _nodeList
        )
{
    m_nodeList = _nodeList;
}

//void SelectionBox::saveToXml(
//        QDomElement& _xmlElement
//        )
//{
//    Q_UNUSED(_xmlElement);
//}

void SelectionBox::loadFromXml(
        QDomElement& _xmlNode,
        QMap<quint64, Port*>& o_portMap
        )
{
    Q_UNUSED(_xmlNode);
    Q_UNUSED(o_portMap);
}

SelectionBox::~SelectionBox()
{
    foreach (Node* node, m_nodeList)
    {
        node->setParentItem(0);
        node->setPos(node->pos() + this->pos());
    }
}
