#ifndef DRAWSCENE
#define DRAWSCENE

#include <QGraphicsScene>
#include "drawtool.h"
#include "drawobj.h"
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE


class DrawScene : public QGraphicsScene
{
public:
    explicit DrawScene(QObject *parent = 0);
    void mouseEvent(QGraphicsSceneMouseEvent *mouseEvent );
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;

};

#endif // DRAWSCENE

