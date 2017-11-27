#ifndef DRAWTOOL
#define DRAWTOOL

#include <QGraphicsScene>
#include <QList>
#include <QCursor>
#include "drawscene.h"

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
class QAbstractGraphicsShapeItem;
QT_END_NAMESPACE

class DrawScene;
class GraphicsItem;

enum DrawShape
{
    selection ,
    rotation  ,
    line ,
    rectangle ,
    roundrect ,
    ellipse ,
    poly
};

class DrawTool
{
public:
    DrawTool( DrawShape shape );

    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event , DrawScene * scene ) ;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event , DrawScene * scene ) ;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event , DrawScene * scene );

    DrawShape m_drawShape;
    static DrawTool * findTool( DrawShape drawShape );
    static QList<DrawTool*> c_tools;
    static QPointF c_down;
    static quint32 c_nDownFlags;
    static QPointF c_last;
    static DrawShape c_drawShape;
};

class SelectTool : public DrawTool
{
public:
    SelectTool();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event , DrawScene * scene ) ;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event , DrawScene * scene ) ;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event , DrawScene * scene );
    QSizeF  m_lastSize;
    QGraphicsPathItem * dashRect;
    QGraphicsLineItem * dashLine;
    bool m_hoverSizer;

};

class  RotationTool : public DrawTool
{
public:
    RotationTool();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event , DrawScene * scene ) ;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event , DrawScene * scene ) ;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event , DrawScene * scene );
    qreal lastAngle;
    QGraphicsPathItem * dashRect;
};

class RectTool : public DrawTool
{
public:
    RectTool(DrawShape drawShape);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event , DrawScene * scene ) ;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event , DrawScene * scene ) ;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event , DrawScene * scene );
    QAbstractGraphicsShapeItem * item;
};

#endif // DRAWTOOL

