#include "qboundingboxscene.h"

QBoundingBoxScene::QBoundingBoxScene(QObject* parent): QGraphicsScene(parent)
{
    this->itemToDraw = 0;

    this->openDialog = false;
    this->drawEnabled = false;
    this->moveEnabled = false;

    this->heightD = 0;
    this->widthD = 0;
}

QBoundingBoxScene::QBoundingBoxScene(Core &_singleton, QObject* parent): QGraphicsScene(parent)
{
    this->itemToDraw = 0;

    this->drawEnabled = false;
    this->moveEnabled = false;
    this->openDialog = false;

    this->heightD = 0;
    this->widthD = 0;

    this->singleton = &(_singleton);
}

void QBoundingBoxScene::keyPressEvent(QKeyEvent* e)
{
    switch(e->key())
    {
    case Qt::Key_Escape:
        this->drawEnabled = false;
        break;

    case Qt::Key_Delete:
        if (this->selectedItems().size() == 1)
        {
            QBoundingBoxRectangle *bbox = static_cast<QBoundingBoxRectangle*>(this->selectedItems().first());
            this->removeItem(selectedItems().front());
            emit this->signal_removeBoundingBoxFromCore(bbox->getKey());
        }
        break;
    }
}

vector<unsigned int> QBoundingBoxScene::selectedBBox()
{
    vector<unsigned int> bboxKeys;

    foreach(QGraphicsItem *item, this->selectedItems())
    {
        QBoundingBoxRectangle *bbox = static_cast<QBoundingBoxRectangle*>(item);
        bboxKeys.push_back(bbox->getKey());
    }

    return bboxKeys;
}

void QBoundingBoxScene::setSingleton(Core &_singleton)
{
    this->singleton = &(_singleton);
}

void QBoundingBoxScene::updateSceneSize()
{
    this->sceneSize = this->sceneRect();
}

void QBoundingBoxScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    this->pointXa = event->scenePos().x();
    this->pointYa = event->scenePos().y();
    this->openDialog = false;

    QGraphicsScene::mousePressEvent(event);
}

void QBoundingBoxScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (this->selectedItems().size() == 1)
    {
        QBoundingBoxRectangle *bbox = static_cast<QBoundingBoxRectangle*>(this->selectedItems().first());

        this->box.x = static_cast<int>(bbox->sceneBoundingRect().x());
        this->box.y = static_cast<int>(bbox->sceneBoundingRect().y());
        this->box.width = static_cast<int>(bbox->sceneBoundingRect().width());
        this->box.height = static_cast<int>(bbox->sceneBoundingRect().height());

        this->openDialog = true;
        emit this->signal_openBoundingBoxDialog(bbox->getKey());
    }

    QGraphicsScene::mouseDoubleClickEvent(event);
}

void QBoundingBoxScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(this->drawEnabled)
    {
        double mouse_posX = event->scenePos().x();
        double mouse_posY = event->scenePos().y();

        if(this->widthD == 0)
        {
            this->widthD = this->sceneRect().width();
        }
        if(this->heightD == 0)
        {
            this->heightD = this->sceneRect().height();
        }

        // the initial point must be inside the video resolution
        if((this->pointXa >= 0) && (this->pointYa >= 0) &&
                (this->pointXa <= this->widthD) && (this->pointYa <= this->heightD))
        {
            delete(this->itemToDraw);
            this->itemToDraw = NULL;

            this->itemToDraw = new QBoundingBoxRectangle(qrand() % 100);
            this->itemToDraw->setPen(QPen(Qt::yellow, 0, Qt::SolidLine));
            this->itemToDraw->setBrush(QBrush(QColor(255, 255, 0, 50)));
            this->addItem(itemToDraw);

            if(mouse_posX < 0) mouse_posX = 0;
            if(mouse_posY < 0) mouse_posY = 0;
            if(mouse_posX > this->widthD) mouse_posX = this->widthD;
            if(mouse_posY > this->heightD) mouse_posY = this->heightD;

            this->pointXb = mouse_posX;
            this->pointYb = mouse_posY;
            this->width = this->pointXb - this->pointXa;
            this->height = this->pointYb - this->pointYa;

            if((this->width < 0) && (this->height < 0))
            {
                this->width = this->pointXa - this->pointXb;
                this->height = this->pointYa - this->pointYb;
                this->itemToDraw->setRect(
                            this->pointXb,
                            this->pointYb,
                            this->width,
                            this->height
                            );
                this->box.x = static_cast<int>(this->pointXb);
                this->box.y = static_cast<int>(this->pointYb);
                this->box.width = static_cast<int>(this->width);
                this->box.height = static_cast<int>(this->height);
            }
            else if((this->width > 0) && (this->height > 0))
            {
                this->itemToDraw->setRect(
                            this->pointXa,
                            this->pointYa,
                            this->width,
                            this->height);
                this->box.x = static_cast<int>(this->pointXa);
                this->box.y = static_cast<int>(this->pointYa);
                this->box.width = static_cast<int>(this->width);
                this->box.height = static_cast<int>(this->height);
            }
            else if((this->width < 0) && (this->height > 0))
            {
                this->width = this->pointXa - this->pointXb;
                this->itemToDraw->setRect(
                            this->pointXb,
                            this->pointYa,
                            this->width,
                            this->height
                            );
                this->box.x = static_cast<int>(this->pointXb);
                this->box.y = static_cast<int>(this->pointYa);
                this->box.width = static_cast<int>(this->width);
                this->box.height = static_cast<int>(this->height);
            }
            else
            {
                this->height = this->pointYa - this->pointYb;
                this->itemToDraw->setRect(
                            this->pointXa,
                            this->pointYb,
                            this->width,
                            this->height
                            );
                this->box.x = static_cast<int>(this->pointXa);
                this->box.y = static_cast<int>(this->pointYb);
                this->box.width = static_cast<int>(this->width);
                this->box.height = static_cast<int>(this->height);
            }
        }
    }
    else
    {
        QGraphicsScene::mouseMoveEvent(event);

        if (this->selectedItems().size() == 1)
        {
            QBoundingBoxRectangle *bbox = static_cast<QBoundingBoxRectangle*>(this->selectedItems().first());

            QRectF oldBoxPos = bbox->boundingRect();
            QRectF newBoxPos = bbox->sceneBoundingRect();

            if(newBoxPos.x() < 0.0)
            {
                bbox->setX((-1.0 * oldBoxPos.x()) + 1);
            }
            if(newBoxPos.y() < 0)
            {
                bbox->setY((-1.0 * oldBoxPos.y()) + 1);
            }

            if(newBoxPos.x() + newBoxPos.width() > this->sceneSize.width())
            {
                bbox->setX(this->sceneSize.width() - oldBoxPos.width() - oldBoxPos.x() - 1);
            }
            if(newBoxPos.y() + newBoxPos.height() > this->sceneSize.height())
            {
                bbox->setY(this->sceneSize.height() - oldBoxPos.height() - oldBoxPos.y() - 1);
            }
        }
    }
}

void QBoundingBoxScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if((this->drawEnabled) && (this->itemToDraw != NULL))
    {
        this->drawEnabled = false;
        this->itemToDraw->setFlag(QGraphicsItem::ItemIsSelectable, true);
        this->itemToDraw->setFlag(QGraphicsItem::ItemIsMovable, true);

        emit this->signal_addBoundingBoxToCore(this->box);
        this->signal_openBoundingBoxDialog(this->singleton->getLatestKey());
    }
    else
    {
        if (this->selectedItems().size() == 1)
        {
            QBoundingBoxRectangle *bbox = static_cast<QBoundingBoxRectangle*>(this->selectedItems().first());

            this->box.x = static_cast<int>(bbox->sceneBoundingRect().x());
            this->box.y = static_cast<int>(bbox->sceneBoundingRect().y());
            this->box.width = static_cast<int>(bbox->sceneBoundingRect().width());
            this->box.height = static_cast<int>(bbox->sceneBoundingRect().height());

            emit this->signal_moveBoundingBoxInCore(bbox->getKey(), this->box);

            if(this->openDialog)
            {
                emit this->signal_openBoundingBoxDialog(bbox->getKey());
                this->openDialog = false;
            }

//            qDebug() << "Clicked Box " << bbox->getId() << " : " << bbox->getKey()
//                     << "=" << bbox->sceneBoundingRect().x()
//                     << ":" << bbox->sceneBoundingRect().y()
//                     << ":" << bbox->sceneBoundingRect().width()
//                     << ":" << bbox->sceneBoundingRect().height();
        }
    }

    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
    QGraphicsScene::mouseReleaseEvent(event);
}

void QBoundingBoxScene::slot_drawFrameBboxes(const Frame &_frame)
{
    map<unsigned int, unsigned int> idCounter = _frame.countIdOccurence();
    map<unsigned int, BoundingBox> bboxes = _frame.getBoxes();
    map<unsigned int, BoundingBox>::iterator it;

    for(it = bboxes.begin(); it != bboxes.end(); it++)
    {
        int id = it->second.getId();
        int key = it->second.getKey();

        this->itemToDraw = new QBoundingBoxRectangle(id, key);
        // Paint with different colors when frame has conflicting bbox ids
        if(idCounter[id] == 1)
        {
            this->itemToDraw->setPen(QPen(Qt::black, 0, Qt::SolidLine));
            this->itemToDraw->setBrush(QBrush(QColor(255, 255, 0, 50)));
            this->itemToDraw->setRect(it->second.getX(),
                                      it->second.getY(),
                                      it->second.getW(),
                                      it->second.getH());
        }
        else
        {
            this->itemToDraw->setPen(QPen(Qt::black, 0, Qt::SolidLine));
            this->itemToDraw->setBrush(QBrush(QColor(255, 0, 0, 50)));
            this->itemToDraw->setRect(it->second.getX(),
                                      it->second.getY(),
                                      it->second.getW(),
                                      it->second.getH());
        }

        // Going beyond HORIZONTAL limit
        if(it->second.getX() + it->second.getW() > this->sceneRect().width())
        {
            int width = static_cast<int>(this->sceneRect().width() - it->second.getX()) - 1;
            this->itemToDraw->setRect(it->second.getX(),
                                      it->second.getY(),
                                      width,
                                      it->second.getH());
        }

        // Going beyond VERTICAl limit
        if(it->second.getY() + it->second.getH() > this->sceneRect().height())
        {
            int height = static_cast<int>(this->sceneRect().height() - it->second.getY()) - 1;
            this->itemToDraw->setRect(it->second.getX(),
                                      it->second.getY(),
                                      it->second.getW(),
                                      height);
        }

        // when going back to a frame, is possible to select and move the BBox already created
        this->itemToDraw->setFlag(QGraphicsItem::ItemIsSelectable, true);
        this->itemToDraw->setFlag(QGraphicsItem::ItemIsMovable, true);
        this->addItem(this->itemToDraw);
    }
}

void QBoundingBoxScene::slot_enableDraw()
{
    this->itemToDraw = 0;
    this->drawEnabled = true;
}
