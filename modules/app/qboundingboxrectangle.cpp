#include "qboundingboxrectangle.h"

QBoundingBoxRectangle::QBoundingBoxRectangle() : QGraphicsRectItem()
{
}

QBoundingBoxRectangle::QBoundingBoxRectangle(unsigned int _id) : QGraphicsRectItem()
{
    this->setId(_id);
}

QBoundingBoxRectangle::QBoundingBoxRectangle(unsigned int _id, unsigned int _key)
{
    this->setId(_id);
    this->setKey(_key);
}

int QBoundingBoxRectangle::getId()
{
    return this->id;
}

int QBoundingBoxRectangle::getKey()
{
    return this->key;
}

void QBoundingBoxRectangle::setId(int _id)
{
    this->id = _id;
}

void QBoundingBoxRectangle::setKey(int _key)
{
    this->key = _key;
}

void QBoundingBoxRectangle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qreal mousePosX = event->pos().x();
    qreal mousePosY = event->pos().y();

    if( (this->isSelected()) && (event->button() == Qt::LeftButton) )
    {
        qreal adjust = 25.0;

        QRectF bbox = this->boundingRect();
        this->pointXa = bbox.x();
        this->pointYa = bbox.y();
        this->pointXb = bbox.x() + bbox.width();
        this->pointYb = bbox.y() + bbox.height();

        if( (mousePosX > this->boundingRect().left()) &&
                (mousePosX < this->boundingRect().right()) &&
                (mousePosY < this->boundingRect().bottom() + adjust) &&
                (mousePosY > this->boundingRect().bottom() - adjust) )
        {
            // bottom
            this->resizeMode = 1;
            this->mouseX = mousePosX;
            this->mouseY = mousePosY;
            this->setFlag(QGraphicsItem::ItemIsMovable, false);

            QApplication::setOverrideCursor(QCursor(Qt::SizeVerCursor));
        }
        else if( (mousePosX > this->boundingRect().right() - adjust) &&
                 (mousePosX < this->boundingRect().right() + adjust) &&
                 (mousePosY < this->boundingRect().bottom()) &&
                 (mousePosY > this->boundingRect().top()) )
        {
            // right
            this->resizeMode = 2;
            this->mouseX = mousePosX;
            this->mouseY = mousePosY;
            this->setFlag(QGraphicsItem::ItemIsMovable, false);

            QApplication::setOverrideCursor(QCursor(Qt::SizeHorCursor));
        }
        else if( (mousePosX > this->boundingRect().left() - adjust) &&
                 (mousePosX < this->boundingRect().left() + adjust) &&
                 (mousePosY < this->boundingRect().bottom()) &&
                 (mousePosY > this->boundingRect().top()) )
        {
            // left
            this->resizeMode = 3;
            this->mouseX = mousePosX;
            this->mouseY = mousePosY;
            this->setFlag(QGraphicsItem::ItemIsMovable, false);

            QApplication::setOverrideCursor(QCursor(Qt::SizeHorCursor));
        }
        else if( (mousePosX > this->boundingRect().left()) &&
                 (mousePosX < this->boundingRect().right()) &&
                 (mousePosY < this->boundingRect().top() + adjust) &&
                 (mousePosY > this->boundingRect().top() - adjust) )
        {
            // top
            this->resizeMode = 4;
            this->mouseX = mousePosX;
            this->mouseY = mousePosY;
            this->setFlag(QGraphicsItem::ItemIsMovable, false);

            QApplication::setOverrideCursor(QCursor(Qt::SizeVerCursor));
        }
        else
        {
            this->resizeMode = 0;
            this->mouseX = mousePosX;
            this->mouseY = mousePosY;
            this->setFlag(QGraphicsItem::ItemIsMovable, true);

            QApplication::setOverrideCursor(QCursor(Qt::OpenHandCursor));
        }
    }

    QGraphicsItem::mousePressEvent(event);
}

void QBoundingBoxRectangle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QApplication::setOverrideCursor(QCursor(Qt::ClosedHandCursor));

    qreal mousePosX = event->pos().x();
    qreal mousePosY = event->pos().y();

    if(this->resizeMode > 0)
    {
        switch(this->resizeMode)
        {
        case 1:
            // bottom
            if(mousePosY > this->pointYa)
            {
                qreal height = mousePosY - this->pointYa;
                qreal width = this->pointXb - this->pointXa;
                this->setRect(this->pointXa, this->pointYa, width, height);
                this->prepareGeometryChange();
            }
            break;

        case 2:
            // right
            if(mousePosX > this->pointXa)
            {
                qreal height = this->pointYb - this->pointYa;
                qreal width = mousePosX - this->pointXa;
                this->setRect(this->pointXa, this->pointYa, width, height);
                this->prepareGeometryChange();
            }
            break;

        case 3:
            // left
            if(mousePosX < this->pointXb)
            {
                qreal height = this->pointYb - this->pointYa;
                qreal width = (this->pointXb - mousePosX);
                this->setRect(mousePosX, this->pointYa, width, height);
                this->prepareGeometryChange();
            }
            break;

        case 4:
            // top
            if(mousePosY < this->pointYb)
            {
                qreal height = this->pointYb - mousePosY;
                qreal width = (this->pointXb - this->pointXa);
                this->setRect(this->pointXa, mousePosY, width, height);
                this->prepareGeometryChange();
            }
            break;
        }
    }

    QGraphicsItem::mouseMoveEvent(event);
}

void QBoundingBoxRectangle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
    QGraphicsItem::mouseReleaseEvent(event);

    this->prepareGeometryChange();
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->update();

    QRectF bbox = this->boundingRect();
    this->pointXa = bbox.x();
    this->pointYa = bbox.y();
    this->pointXb = bbox.x() + bbox.width();
    this->pointYb = bbox.y() + bbox.height();
    this->resizeMode = 0;

//    qDebug() << event->pos().toPoint();
//    qDebug() << event->scenePos().toPoint();
//    qDebug() << event->screenPos();

//    qDebug() << this->pos().toPoint();
//    qDebug() << this->x() << ":" << this->y();

//    qDebug() << this->rect();
//    qDebug() << this->boundingRect();
//    qDebug() << this->sceneBoundingRect();
}

