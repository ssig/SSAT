#include "qdraggablerectitem.h"

QDraggableRectItem::QDraggableRectItem(QGraphicsItem* parent):
    QGraphicsRectItem(parent), m_dragged(false)
{
    setFlags(QGraphicsItem::ItemIsSelectable|
             QGraphicsItem::ItemIsMovable);
}

void QDraggableRectItem::setAnchorPoint(const QPointF &anchorPoint)
{
    this->anchorPoint = anchorPoint;
}

void QDraggableRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    this->m_dragged = true;
    QGraphicsRectItem::mouseMoveEvent(event);
}

void QDraggableRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(this->m_dragged)
    {
        QList<QGraphicsItem*> colItems = collidingItems();

        if(colItems.isEmpty())
        {
            this->setPos(anchorPoint);
        }
        else
        {
            QGraphicsItem* closestItem = colItems.at(0);
            qreal shortestDist = 100000;
            foreach(QGraphicsItem* item, colItems)
            {
                QLineF line(item->sceneBoundingRect().center(),
                            this->sceneBoundingRect().center());
                if(line.length() < shortestDist)
                {
                    shortestDist = line.length();
                    closestItem = item;
                }
            }
            this->setPos(closestItem->scenePos());
        }
        this->m_dragged = false;
    }
    QGraphicsRectItem::mouseReleaseEvent(event);
}
