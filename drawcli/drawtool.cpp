#include "drawtool.h"
#include "drawobj.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QtMath>

#define PI 3.14

QList<DrawTool*> DrawTool::c_tools;
QPointF DrawTool::c_down;
QPointF DrawTool::c_last;
quint32 DrawTool::c_nDownFlags;

DrawShape DrawTool::c_drawShape = selection;

static SelectTool selectTool;
static RectTool   lineTool(line);
static RectTool   rectTool(rectangle);
static RectTool   roundRectTool(roundrect);
static RectTool   ellipseTool(ellipse);
static RotationTool rotationTool;

enum SelectMode
{
    none,
    netSelect,
    move,
    size,
    rotate
};

SelectMode selectMode = none;

SizeHandleRect::Direction nDragHandle = SizeHandleRect::None;

static void setCursor(QGraphicsScene * scene , const QCursor & cursor )
{
    QList<QGraphicsView*> views  = scene->views();
    if ( views.count() > 0 ){
        QGraphicsView * view = views.first();
        view->setCursor(cursor);
    }
}

DrawTool::DrawTool(DrawShape shape)
{
    m_drawShape = shape ;
    c_tools.push_back(this);
}

void DrawTool::mousePressEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{
    c_down = event->scenePos();
    c_last = event->scenePos();
}

void DrawTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{
    c_last = event->scenePos();
}

void DrawTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{
    if (event->scenePos() == c_down)
        c_drawShape = selection;
    setCursor(scene,Qt::ArrowCursor);

}

DrawTool *DrawTool::findTool(DrawShape drawShape)
{
    QList<DrawTool*>::const_iterator iter = c_tools.constBegin();
    for ( ; iter != c_tools.constEnd() ; ++iter ){

        if ((*iter)->m_drawShape == drawShape )
            return (*iter);
    }
    return 0;
}

SelectTool::SelectTool()
    :DrawTool(selection)
{
    m_lastSize.setHeight(0);
    m_lastSize.setWidth(0);
    dashRect = 0;
    dashLine = 0;
    m_hoverSizer = false;

}

void SelectTool::mousePressEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{
    DrawTool::mousePressEvent(event,scene);
    if (!m_hoverSizer)
      scene->mouseEvent(event);

    selectMode = none;
    QList<QGraphicsItem *> items = scene->selectedItems();
    GraphicsItem *item = 0;

    if ( items.count() == 1 )
        item = qgraphicsitem_cast<GraphicsItem*>(items.first());

    if ( item != 0 ){

        nDragHandle = item->hitTest(event->scenePos());
        if ( nDragHandle !=SizeHandleRect::None)
             selectMode = size;
        else
            selectMode =  move;
        m_lastSize = item->boundingRect().size();
    }
    if( selectMode == none ){
        selectMode = netSelect;
        QList<QGraphicsView*> views  = scene->views();
        if ( views.count() > 0 ){
            QGraphicsView * view = views.first();
            view->setDragMode(QGraphicsView::RubberBandDrag);
        }
    }

    if ( selectMode == move && items.count() == 1 ){

        if (dashRect ){
            scene->removeItem(dashRect);
            delete dashRect;
            dashRect = 0;
        }
        if (dashLine ) {
            scene->removeItem(dashLine);
            delete dashLine;
            dashLine = 0;
        }

        dashRect = new QGraphicsPathItem(item->shape());
        dashRect->setPen(Qt::DashLine);
        dashRect->setPos(item->pos());
//        dashRect->setTransformOriginPoint(item->transformOriginPoint());
//        dashRect->setTransform(item->transform());
//        dashRect->setRotation(item->rotation());
        scene->addItem(dashRect);

        dashLine = new QGraphicsLineItem();
        dashLine->setPos(item->pos());
        dashLine->setPen(Qt::DashLine);
        scene->addItem(dashLine);

    }
    qDebug() << " selectTool press:" <<event->scenePos() << " mode :" << selectMode << " items :" << items.count();
}

void SelectTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{

    DrawTool::mouseMoveEvent(event,scene);
    bool isGroup = false;
    QList<QGraphicsItem *> items = scene->selectedItems();
    GraphicsItem * item = 0;
    if ( items.count() == 1 ){
        item = qgraphicsitem_cast<GraphicsItem*>(items.first());
        if ( item != 0 ){
            if ( nDragHandle != SizeHandleRect::None && selectMode == size ){
                item->resizeTo(nDragHandle,c_last);
            }
            else if(nDragHandle == SizeHandleRect::None && selectMode == selection ){
                 SizeHandleRect::Direction handle = item->hitTest(event->scenePos());
                 if ( handle != SizeHandleRect::None){
                     setCursor(scene,item->getCursor(handle));
                     m_hoverSizer = true;
                 }else{
                     setCursor(scene,Qt::ArrowCursor);
                     m_hoverSizer = false;
                 }
             }
        }
        QGraphicsItemGroup *item1 = qgraphicsitem_cast<QGraphicsItemGroup*>(items.first());
        if ( item1 != NULL ){
            isGroup = true;
        }
    }
    if ( selectMode == move ){
        if ( dashRect ){

            dashLine->setLine(QLineF(dashLine->mapFromItem(item,item->boundingRect().topLeft()),
                                     dashLine->mapFromScene(c_last)));
             dashRect->setPos( dashLine->mapFromScene(c_last)) ;
            scene->update();
        }
    }
    if ( (selectMode != size  && items.count() > 1) || isGroup )
    {
        scene->mouseEvent(event);

    }

}

void SelectTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{
    DrawTool::mouseReleaseEvent(event,scene);

    QList<QGraphicsItem *> items = scene->selectedItems();
    if ( items.count() == 1 ){
        GraphicsItem * item = qgraphicsitem_cast<GraphicsItem*>(items.first());
        if ( item != 0  && selectMode == move && c_last != c_down ){
             item->setPos(c_last);
        }
    }

    if (selectMode == netSelect ){
        QList<QGraphicsView*> views  = scene->views();
        if ( views.count() > 0 ){
            QGraphicsView * view = views.first();
            view->setDragMode(QGraphicsView::NoDrag);
        }
    }

    if (dashRect ){
        scene->removeItem(dashRect);
        delete dashRect;
        dashRect = 0;

        scene->removeItem(dashLine);
        delete dashLine;
        dashLine = 0;
    }

    selectMode = none;
    nDragHandle = SizeHandleRect::None;
    m_hoverSizer = false;
    scene->mouseEvent(event);

    qDebug() << " selectTool release:" <<event->scenePos() << "mode :" << selectMode;

}

RotationTool::RotationTool()
    :DrawTool(rotation)
{
    lastAngle == 0;
    dashRect = 0;
}

void RotationTool::mousePressEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{
    DrawTool::mousePressEvent(event,scene);
    QList<QGraphicsItem *> items = scene->selectedItems();
    if ( items.count() == 1 ){
        GraphicsItem * item = qgraphicsitem_cast<GraphicsItem*>(items.first());
        if ( item != 0 ){
            nDragHandle = item->hitTest(event->scenePos());
            if ( nDragHandle !=SizeHandleRect::None)
            {
                QPointF origin = item->mapToScene(item->boundingRect().center());

                qreal len_y = c_last.y() - origin.y();
                qreal len_x = c_last.x() - origin.x();

                qreal angle = atan2(len_y,len_x)*180/PI;

                lastAngle = angle;
                selectMode = rotate;

                if (dashRect ){
                    scene->removeItem(dashRect);
                    delete dashRect;
                    dashRect = 0;
                }

                dashRect = new QGraphicsPathItem(item->shape());
                dashRect->setPen(Qt::DashLine);
                dashRect->setPos(item->pos());
                dashRect->setTransformOriginPoint(item->transformOriginPoint());
                dashRect->setTransform(item->transform());
                dashRect->setRotation(item->rotation());
                scene->addItem(dashRect);

                setCursor(scene,QCursor(QPixmap(":/image/rotation.png")));
            }
            else{

                    scene->clearSelection();
                    c_drawShape = selection;
                }
        }
    }
}

void RotationTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{
    DrawTool::mouseMoveEvent(event,scene);

    QList<QGraphicsItem *> items = scene->selectedItems();
    if ( items.count() == 1 ){
        GraphicsItem * item = qgraphicsitem_cast<GraphicsItem*>(items.first());
        if ( item != 0  && nDragHandle !=SizeHandleRect::None && selectMode == rotate ){


             QPointF origin = item->mapToScene(item->boundingRect().center());

             qreal len_y = c_last.y() - origin.y();
             qreal len_x = c_last.x() - origin.x();

             qreal angle = atan2(len_y,len_x)*180/PI;

             angle = item->rotation() + int(angle - lastAngle) ;

             if ( angle > 360 )
                 angle -= 360;

             if ( dashRect ){
                dashRect->setRotation( angle );
                 scene->update();
             }

             setCursor(scene,QCursor(QPixmap(":/image/rotation.png")));
        }
        else if ( item )
        {
            SizeHandleRect::Direction handle = item->hitTest(event->scenePos());
            if ( handle != SizeHandleRect::None)
                setCursor(scene,QCursor(QPixmap(":/image/rotation.png")));
            else
                setCursor(scene,Qt::ArrowCursor);
        }
    }
}

void RotationTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{
    DrawTool::mouseReleaseEvent(event,scene);

    QList<QGraphicsItem *> items = scene->selectedItems();
    if ( items.count() == 1 ){
        GraphicsItem * item = qgraphicsitem_cast<GraphicsItem*>(items.first());
        if ( item != 0  && nDragHandle !=SizeHandleRect::None && selectMode == rotate ){

             QPointF origin = item->mapToScene(item->boundingRect().center());

             QPointF delta = c_last - origin ;
             qreal len_y = c_last.y() - origin.y();
             qreal len_x = c_last.x() - origin.x();
             qreal angle = atan2(len_y,len_x)*180/PI;
             angle = item->rotation() + int(angle - lastAngle) ;
             if ( angle > 360 )
                 angle -= 360;
             item->setRotation( angle );
        }
    }

    setCursor(scene,Qt::ArrowCursor);
    selectMode = none;
    nDragHandle = SizeHandleRect::None;
    lastAngle = 0;
    if (dashRect ){
        scene->removeItem(dashRect);
        delete dashRect;
        dashRect = 0;
    }
    qDebug() << " RotationTool release:" <<event->scenePos();
}

RectTool::RectTool(DrawShape drawShape)
    :DrawTool(drawShape)
{
}

void RectTool::mousePressEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{
    DrawTool::mousePressEvent(event,scene);
    scene->clearSelection();
    switch ( c_drawShape ){
    case rectangle:
        item = new GraphicsRectItem(QRect(0,0,0,0),NULL);
        item->setPen(Qt::NoPen);
        break;
    case ellipse:
        item = new GraphicsEllipseItem(QRect(0,0,0,0),NULL);
        item->setPen(Qt::NoPen);
        break;
    case line:
        item = new GraphicsLineItem(0);
        item->setPen(QPen(Qt::black,2));
        break;
    }
    if ( item == 0) return;
    item->setPos(event->scenePos());
    scene->addItem(item);
    item->setSelected(true);
    selectMode = size;
    nDragHandle = SizeHandleRect::RightBottom;

    qDebug() << " RectTool shape :" <<m_drawShape << "  " <<event->scenePos();
}

void RectTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{
    setCursor(scene,Qt::CrossCursor);
    selectTool.mouseMoveEvent(event,scene);
}

void RectTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{
    if ( event->scenePos() == c_down ){

       if ( item != 0)
         scene->removeItem(item);
       selectTool.mousePressEvent(event,scene);
       qDebug()<<"RectTool removeItem:";
    }
    selectTool.mouseReleaseEvent(event,scene);
    qDebug() << " RectTool shape :" <<m_drawShape << "  "<<event->scenePos();

}
