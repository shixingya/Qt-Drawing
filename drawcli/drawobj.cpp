#include "drawobj.h"
#include <QPainter>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QCursor>
#include <QDebug>
#include <QGraphicsEffect>


static QPainterPath qt_graphicsItem_shapeFromPath(const QPainterPath &path, const QPen &pen)
{
    // We unfortunately need this hack as QPainterPathStroker will set a width of 1.0
    // if we pass a value of 0.0 to QPainterPathStroker::setWidth()
    const qreal penWidthZero = qreal(0.00000001);

    if (path == QPainterPath() || pen == Qt::NoPen)
        return path;
    QPainterPathStroker ps;
    ps.setCapStyle(pen.capStyle());
    if (pen.widthF() <= 0.0)
        ps.setWidth(penWidthZero);
    else
        ps.setWidth(pen.widthF());
    ps.setJoinStyle(pen.joinStyle());
    ps.setMiterLimit(pen.miterLimit());
    QPainterPath p = ps.createStroke(path);
    p.addPath(path);
    return p;
}


GraphicsItem::GraphicsItem(QGraphicsItem *parent)
    :QAbstractGraphicsShapeItem(parent)
{
}

void GraphicsItem::updateGeometry()
{
    const QRectF &geom = this->boundingRect();

    const int w = SELECTION_HANDLE_SIZE;
    const int h = SELECTION_HANDLE_SIZE;

    const Handles::iterator hend =  m_handles.end();
    for (Handles::iterator it = m_handles.begin(); it != hend; ++it) {
        SizeHandleRect *hndl = *it;;
        switch (hndl->dir()) {
        case SizeHandleRect::LeftTop:
            hndl->move(geom.x() - w / 2, geom.y() - h / 2);
            break;
        case SizeHandleRect::Top:
            hndl->move(geom.x() + geom.width() / 2 - w / 2, geom.y() - h / 2);
            break;
        case SizeHandleRect::RightTop:
            hndl->move(geom.x() + geom.width() - w / 2, geom.y() - h / 2);
            break;
        case SizeHandleRect::Right:
            hndl->move(geom.x() + geom.width() - w / 2, geom.y() + geom.height() / 2 - h / 2);
            break;
        case SizeHandleRect::RightBottom:
            hndl->move(geom.x() + geom.width() - w / 2, geom.y() + geom.height() - h / 2);
            break;
        case SizeHandleRect::Bottom:
            hndl->move(geom.x() + geom.width() / 2 - w / 2, geom.y() + geom.height() - h / 2);
            break;
        case SizeHandleRect::LeftBottom:
            hndl->move(geom.x() - w / 2, geom.y() + geom.height() - h / 2);
            break;
        case SizeHandleRect::Left:
            hndl->move(geom.x() - w / 2, geom.y() + geom.height() / 2 - h / 2);
            break;
        case SizeHandleRect::Center:
            hndl->move(geom.center().x()  - w / 2 , geom.center().y() - h / 2);
            break;
        default:
            break;
        }
    }

}

void GraphicsItem::setState(SelectionHandleState st)
{
    const Handles::iterator hend =  m_handles.end();
    for (Handles::iterator it = m_handles.begin(); it != hend; ++it)
        (*it)->setState(st);
}

SizeHandleRect::Direction GraphicsItem::hitTest(const QPointF &point) const
{
    const Handles::const_iterator hend =  m_handles.end();
    for (Handles::const_iterator it = m_handles.begin(); it != hend; ++it)
    {
        if ((*it)->hitTest(point) ){
            return (*it)->dir();
        }
    }
    return SizeHandleRect::None;
}

Qt::CursorShape GraphicsItem::getCursor(SizeHandleRect::Direction dir)
{
    switch (dir) {
    case SizeHandleRect::Right:
        return Qt::SizeHorCursor;
    case SizeHandleRect::RightTop:
        return Qt::SizeBDiagCursor;
    case SizeHandleRect::RightBottom:
        return Qt::SizeFDiagCursor;
    case SizeHandleRect::LeftBottom:
        return Qt::SizeBDiagCursor;
    case SizeHandleRect::Bottom:
        return Qt::SizeVerCursor;
    case SizeHandleRect::LeftTop:
        return Qt::SizeFDiagCursor;
    case SizeHandleRect::Left:
        return Qt::SizeHorCursor;
    case SizeHandleRect::Top:
        return Qt::SizeVerCursor;
    default:
        break;
    }
    return Qt::ArrowCursor;
}

void GraphicsItem::resizeTo(SizeHandleRect::Direction dir, const QPointF &point)
{

}

void GraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{

}

QVariant GraphicsItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if ( change == QGraphicsItem::ItemSelectedHasChanged ) {
        qDebug()<<" Item Selected : " << value.toString();
        setState(value.toBool() ? SelectionHandleActive : SelectionHandleOff);
    }else if ( change == QGraphicsItem::ItemRotationHasChanged ){
        qDebug()<<"Item Rotation Changed:" << value.toString();
    }else if ( change == QGraphicsItem::ItemTransformOriginPointHasChanged ){
        qDebug()<<"ItemTransformOriginPointHasChanged:" << value.toPointF();
    }
    return value;
}


GraphicsRectItem::GraphicsRectItem(const QRect & rect ,QGraphicsItem *parent)
    :GraphicsItem(parent)
    ,m_width(rect.width())
    ,m_height(rect.height())
{

    // handles
    m_handles.reserve(SizeHandleRect::None);
    for (int i = SizeHandleRect::LeftTop; i <= SizeHandleRect::Left; ++i) {
        SizeHandleRect *shr = new SizeHandleRect(this, static_cast<SizeHandleRect::Direction>(i), this);
        m_handles.push_back(shr);
    }
    updateGeometry();
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    m_delta =rect;
    this->setAcceptHoverEvents(true);
}

QRectF GraphicsRectItem::boundingRect() const
{
    QRectF bounding ( QPointF(m_delta.left(), m_delta.top()),QPointF(m_delta.right(),m_delta.bottom()));
    return bounding;
}

QPainterPath GraphicsRectItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return qt_graphicsItem_shapeFromPath(path,pen());
}

void GraphicsRectItem::resizeTo(SizeHandleRect::Direction dir, const QPointF &point)
{
      qDebug()<<"777777777777777777777777777777777777777777"<<dir;
    QPointF local = mapFromScene(point);
    QString dirName;
    m_delta= this->rect().toRect();
    switch (dir) {

    case SizeHandleRect::Right:
        if(local.x() > m_delta.left())
            m_delta.setRight(local.x());
        else
            m_delta.setLeft(local.x());
        break;
    case SizeHandleRect::RightTop:
        dirName = "RightTop";
        m_delta.setTopRight(local.toPoint());
        break;
    case SizeHandleRect::RightBottom:
        dirName = "RightBottom";
        m_delta.setBottomRight(local.toPoint());
        break;
    case SizeHandleRect::LeftBottom:
        dirName = "LeftBottom";
        m_delta.setBottomLeft(local.toPoint());
        break;
    case SizeHandleRect::Bottom:
        dirName = "Bottom";
        m_delta.setBottom(local.y());
        break;
    case SizeHandleRect::LeftTop:
        dirName = "LeftTop";
        m_delta.setTopLeft(local.toPoint());
        break;
    case SizeHandleRect::Left:
        dirName = "Left";
        qDebug()<<"SizeHandleRect::Left  ==" <<m_delta.left()
               <<"    localx ="<<local.x();
        m_delta.setLeft(local.x());
        //emit MoveItemPos(local.x(),0);
        break;
    case SizeHandleRect::Top:
        dirName = "Top";
        m_delta.setTop(local.y());
        break;
   default:
        break;
    }
    if(m_delta.left()>m_delta.right())
    {
        double tmp = m_delta.right();
        m_delta.setRight(m_delta.left());
        m_delta.setLeft(tmp);
    }
    if(m_delta.top()>m_delta.bottom())
    {
        double tmp = m_delta.bottom();
        m_delta.setBottom(m_delta.top());
        m_delta.setTop(tmp);
    }
    prepareGeometryChange();

    m_width = m_delta.width();
    m_height = m_delta.height();

    qDebug()<<"resizeTo :"<<dirName<<"move:"<<local<< "newSize :" <<QSize(m_width,m_height);
    updateGeometry();
}



void GraphicsRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QColor c = QColor(Qt::red);
    c.setAlpha(160);
    QLinearGradient result(rect().topLeft(), rect().topRight());
    result.setColorAt(0, c.dark(150));
    result.setColorAt(0.5, c.light(200));
    result.setColorAt(1, c.dark(150));
    painter->setPen(pen());
    painter->setBrush(result);
    painter->drawRect(rect());
    /*
    QColor c1 = QColor(Qt::blue);
    c1.setAlpha(180);
    painter->setBrush(c1);
    painter->drawEllipse(rect.topLeft().x() - 3 , rect.topLeft().y() - 3 ,6,6);
   */
}


GraphicsEllipseItem::GraphicsEllipseItem(const QRect &rect, QGraphicsItem *parent)
    :GraphicsRectItem(rect,parent)
{

}

QPainterPath GraphicsEllipseItem::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return qt_graphicsItem_shapeFromPath(path,pen());
}

void GraphicsEllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QColor c = QColor(Qt::red);
    c.setAlpha(160);
    QRectF rc = rect();
    qreal radius = qMax(rc.width(),rc.height());
    QRadialGradient result(rc.center(),radius);
    result.setColorAt(0, c.light(200));
    result.setColorAt(0.5, c.dark(150));
    result.setColorAt(1, c);
    painter->setPen(pen());
    painter->setBrush(result);
    painter->drawEllipse(rc);
}




GraphicsLineItem::GraphicsLineItem(QGraphicsItem *parent)
    :GraphicsRectItem(QRect(0,0,0,0),parent)
{
    // handles
    m_handles.reserve(SizeHandleRect::None);

    Handles::iterator hend =  m_handles.end();
    for (Handles::iterator it = m_handles.begin(); it != hend; ++it)
        delete (*it);
    m_handles.clear();

    SizeHandleRect *shr = new SizeHandleRect(this,SizeHandleRect::LeftTop, this);
    m_handles.push_back(shr);
    shr = new SizeHandleRect(this,SizeHandleRect::RightBottom, this);
    m_handles.push_back(shr);

    updateGeometry();
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
}



QPainterPath GraphicsLineItem::shape() const
{
    QPainterPath path;
    path.moveTo(rect().topLeft());
    path.lineTo(rect().bottomRight());
    return qt_graphicsItem_shapeFromPath(path,pen());
}

void GraphicsLineItem::resizeTo(SizeHandleRect::Direction dir, const QPointF &point)
{
    QPointF local = mapFromScene(point);
    prepareGeometryChange();

    const QRectF &geom = this->boundingRect();
    QRect delta = this->rect().toRect();

    switch(dir)
    {
    case SizeHandleRect::LeftTop:
        delta.setTopLeft(local.toPoint());
        break;
    case SizeHandleRect::RightBottom:
        delta.setBottomRight(local.toPoint());
        break;
    default:
        break;
    }
    m_width = delta.width();
    m_height = delta.height();

    updateGeometry();
}


void GraphicsLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QColor c = QColor(Qt::red);
    painter->setPen(pen());
    painter->drawLine(rect().topLeft(),rect().bottomRight());
}

GraphicsItemGroup::GraphicsItemGroup(QGraphicsItemGroup *group, QGraphicsItem *parent)
    :GraphicsRectItem(QRect(0,0,0,0),parent )
    ,m_group(group)
{
    if ( m_group ){
        m_group->setParentItem(this);
        QRectF bound = m_group->boundingRect();

        qDebug()<<" group rect :" << m_group->boundingRect() << " new Rect:" << bound;
        bound.normalized();
        prepareGeometryChange();
        m_width = bound.width();
        m_height = bound.height();

        QPointF delta = pos() - bound.center() ;

        setTransform(transform().translate(delta.x(),delta.y()));
        setTransformOriginPoint( bound.center());
        moveBy(-delta.x(),-delta.y());

        m_group->setTransformOriginPoint(bound.center());

        qDebug()<<"pos :" << delta;
        updateGeometry();
    }
}

QRectF GraphicsItemGroup::boundingRect() const
{

    QRectF bounding = m_group->mapRectToParent(m_group->boundingRect());

    return bounding;
}

QPainterPath GraphicsItemGroup::shape() const
{
    return m_group->shape() ;
}

void GraphicsItemGroup::resizeTo(SizeHandleRect::Direction dir, const QPointF &point)
{

}

void GraphicsItemGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPointF origin = mapFromScene(pos());

    QPointF origin1 = m_group->transformOriginPoint();

    QColor c1 = QColor(Qt::blue);
  //  c1.setAlpha(180);
    painter->setBrush(c1);
    painter->drawEllipse(origin.x() - 3 , origin.y() - 3 ,6,6);


    QColor c2 = QColor(Qt::green);
  //  c2.setAlpha(180);
    painter->setBrush(c2);
    painter->drawEllipse(origin1.x() - 3 , origin1.y() - 3 ,6,6);

}

