/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionEllipse;
    QAction *actionRectangle;
    QAction *actionSelection;
    QAction *actionBringToFront;
    QAction *actionSendToBack;
    QAction *actionRotation;
    QAction *actionLine;
    QAction *actionSave;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QGraphicsView *graphicsView;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setWindowModality(Qt::WindowModal);
        MainWindow->resize(600, 446);
        MainWindow->setContextMenuPolicy(Qt::DefaultContextMenu);
        actionEllipse = new QAction(MainWindow);
        actionEllipse->setObjectName(QStringLiteral("actionEllipse"));
        actionEllipse->setCheckable(true);
        QIcon icon;
        icon.addFile(QStringLiteral(":/image/circle.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionEllipse->setIcon(icon);
        actionRectangle = new QAction(MainWindow);
        actionRectangle->setObjectName(QStringLiteral("actionRectangle"));
        actionRectangle->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/image/rectangle.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRectangle->setIcon(icon1);
        actionSelection = new QAction(MainWindow);
        actionSelection->setObjectName(QStringLiteral("actionSelection"));
        actionSelection->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/image/pointer.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSelection->setIcon(icon2);
        actionBringToFront = new QAction(MainWindow);
        actionBringToFront->setObjectName(QStringLiteral("actionBringToFront"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/image/sendtoback.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBringToFront->setIcon(icon3);
        actionSendToBack = new QAction(MainWindow);
        actionSendToBack->setObjectName(QStringLiteral("actionSendToBack"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/image/bringtofront.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSendToBack->setIcon(icon4);
        actionRotation = new QAction(MainWindow);
        actionRotation->setObjectName(QStringLiteral("actionRotation"));
        actionRotation->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/image/rotation.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRotation->setIcon(icon5);
        actionLine = new QAction(MainWindow);
        actionLine->setObjectName(QStringLiteral("actionLine"));
        actionLine->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/image/linecolor.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLine->setIcon(icon6);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/image/save-5.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon7);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy);
        graphicsView->setAutoFillBackground(false);
        graphicsView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

        horizontalLayout->addWidget(graphicsView);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        mainToolBar->addAction(actionSave);
        mainToolBar->addAction(actionSelection);
        mainToolBar->addAction(actionLine);
        mainToolBar->addAction(actionEllipse);
        mainToolBar->addAction(actionRectangle);
        mainToolBar->addAction(actionBringToFront);
        mainToolBar->addAction(actionSendToBack);
        mainToolBar->addAction(actionRotation);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "drawcli", Q_NULLPTR));
        actionEllipse->setText(QApplication::translate("MainWindow", "Ellipse", Q_NULLPTR));
        actionRectangle->setText(QApplication::translate("MainWindow", "Rectangle", Q_NULLPTR));
        actionSelection->setText(QApplication::translate("MainWindow", "Selection", Q_NULLPTR));
        actionBringToFront->setText(QApplication::translate("MainWindow", "BringToFront", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionBringToFront->setToolTip(QApplication::translate("MainWindow", "BringToFront", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionSendToBack->setText(QApplication::translate("MainWindow", "SendToBack", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionSendToBack->setToolTip(QApplication::translate("MainWindow", "sendToBack", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionRotation->setText(QApplication::translate("MainWindow", "rotation", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionRotation->setToolTip(QApplication::translate("MainWindow", "rotation", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionLine->setText(QApplication::translate("MainWindow", "line", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionLine->setToolTip(QApplication::translate("MainWindow", "line", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionSave->setText(QApplication::translate("MainWindow", "save", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionSave->setToolTip(QApplication::translate("MainWindow", "save svg", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
