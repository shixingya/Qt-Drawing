#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "drawscene.h"
#include <QFileDialog>
#include <QPainter>
#include <QSvgGenerator>
#include <QPainter>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    group = new QActionGroup(this);
    group->addAction(ui->actionSelection);
    group->addAction(ui->actionEllipse);
    group->addAction(ui->actionRectangle);
    group->addAction(ui->actionRotation);
    group->addAction(ui->actionLine);
    ui->actionSelection->setChecked(true);



    m_scene = new DrawScene(this);
    m_scene->setSceneRect(QRectF(-800 / 2 , -600 / 2 , 800, 600));
    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    m_scene->setBackgroundBrush(QPixmap(":/image/background1.png"));

    m_scene->update();
    ui->graphicsView->update();

    connect(ui->actionEllipse,SIGNAL(triggered()),this,SLOT(addShape()));
    connect(ui->actionRectangle,SIGNAL(triggered()),this,SLOT(addShape()));
    connect(ui->actionSelection,SIGNAL(triggered()),this,SLOT(addShape()));
    connect(ui->actionRotation,SIGNAL(triggered()),this,SLOT(addShape()));
    connect(ui->actionLine,SIGNAL(triggered()),this,SLOT(addShape()));

    connect(&m_timer,SIGNAL(timeout()),this,SLOT(updateUI()));
    m_timer.start(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addShape()
{
    if ( sender() == ui->actionEllipse )
        DrawTool::c_drawShape = ellipse;
    if ( sender() == ui->actionRectangle )
        DrawTool::c_drawShape = rectangle;
    if ( sender() == ui->actionSelection )
        DrawTool::c_drawShape = selection;
    if (sender() == ui->actionRotation )
        DrawTool::c_drawShape = rotation;
    if ( sender() == ui->actionLine )
        DrawTool::c_drawShape = line;

}

void MainWindow::updateUI()
{
    if ( DrawTool::c_drawShape == ellipse )
        ui->actionEllipse->setChecked(true);
    else if ( DrawTool::c_drawShape == rectangle )
        ui->actionRectangle->setChecked(true);
    else if (DrawTool::c_drawShape == selection )
        ui->actionSelection->setChecked(true);
    else if ( DrawTool::c_drawShape == rotation )
        ui->actionRotation->setChecked(true);
    else if ( DrawTool::c_drawShape == line )
        ui->actionLine->setChecked(true);

    if (m_scene->selectedItems().count() > 0 ){
        ui->actionBringToFront->setEnabled(true);
        ui->actionSendToBack->setEnabled(true);
    }else{
        ui->actionBringToFront->setEnabled(false);
        ui->actionSendToBack->setEnabled(false);
    }
}

void MainWindow::on_actionBringToFront_triggered()
{

    if (m_scene->selectedItems().isEmpty())
        return;
    QGraphicsItem *selectedItem = m_scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() >= zValue && item->type() == GraphicsItem::Type)
            zValue = item->zValue() + 0.1;
    }
    selectedItem->setZValue(zValue);


    //QGraphicsItemGroup test

    QList<QGraphicsItem *> selectedItems = m_scene->selectedItems();

    QGraphicsItemGroup * group = new QGraphicsItemGroup();
    foreach (QGraphicsItem *item, selectedItems) {
        item->setSelected(false);
        group->addToGroup(item);
    }


//    group->setFlag(QGraphicsItem::ItemIsMovable, true);
//    group->setFlag(QGraphicsItem::ItemIsSelectable, true);
//    group->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    GraphicsItemGroup * proxy = new GraphicsItemGroup(group,NULL );
//    group->setParentItem(proxy);
//    proxy->setPos(group->pos());
    m_scene->addItem(proxy);
    m_scene->clearSelection();


}

void MainWindow::on_actionSendToBack_triggered()
{

    if (m_scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = m_scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() <= zValue && item->type() == GraphicsItem::Type)
            zValue = item->zValue() - 0.1;
    }
    selectedItem->setZValue(zValue);

}


void MainWindow::on_actionSave_triggered()
{
    QString newPath = QFileDialog::getSaveFileName(this, tr("Save SVG"),
        path, tr("SVG files (*.svg)"));

    if (newPath.isEmpty())
        return;

    path = newPath;

//![configure SVG generator]
    QSvgGenerator generator;
    generator.setFileName(path);
    generator.setSize(QSize(800, 600));
    generator.setViewBox(QRect(0, 0, 800, 600));
    generator.setTitle(tr("SVG Generator Example Drawing"));
    generator.setDescription(tr("An SVG drawing created by the SVG Generator "
                                "Example provided with Qt."));
//![configure SVG generator]
//![begin painting]
    QPainter painter;
    painter.begin(&generator);
//![begin painting]
//!
    m_scene->clearSelection();
    m_scene->render(&painter);
//![end painting]
    painter.end();
//![end painting]
}
