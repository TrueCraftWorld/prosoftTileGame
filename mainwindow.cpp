#include <QTimer>
#include <QPushButton>
#include <QRandomGenerator>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(300, 600);
    static bool isLost = false;
/// Your code here...
    auto deployTimer = new QTimer(this);
    deployTimer->setInterval(QRandomGenerator::global()->bounded(100, 1000));

    auto runAwayTimer = new QTimer(this);
    runAwayTimer->setInterval(5);

    QObject::connect(deployTimer, &QTimer::timeout, this, [=] {
        QPushButton* newButton = new QPushButton(this);
        newButton->setGeometry(QRect(QRandomGenerator::global()->bounded(this->width()-50), 100, 25, 25));
        newButton->setMouseTracking(true);
        // newButton->ge
        auto dropTimer = new QTimer(newButton);
        dropTimer->setInterval(QRandomGenerator::global()->bounded(60, 500));

        QObject::connect(dropTimer, &QTimer::timeout,  newButton, [=] {
            newButton->move(newButton->x(),newButton->y()+5);
            if (newButton->y() >= this->height()-25) {
                if (!isLost) {
                    QPalette palette = this->palette();
                    palette.setColor(QPalette::Window, QColor(255,0,0));
                    this->setAutoFillBackground(true);
                    this->setPalette(palette);
                    this->setWindowTitle(QString("LOSTLOSTLOST"));
                    this->update();
                    isLost = true;
                }
                newButton->deleteLater();
            }
        } );

        QObject::connect(newButton, &QPushButton::clicked, newButton, [=] {
            if (!isLost) newButton->deleteLater();
        } );

        newButton->show();
        dropTimer->start();
    } );


    QObject::connect(deployTimer, &QTimer::timeout,  this, [=] {
        deployTimer->setInterval(QRandomGenerator::global()->bounded(100, 1000));
    } );
    deployTimer->start();
}

MainWindow::~MainWindow()
{
}


