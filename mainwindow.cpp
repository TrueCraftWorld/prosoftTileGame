#include <QTimer>
#include <QPushButton>
#include <QRandomGenerator>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(300, 600);
    static bool isLost = false;
    const int tile_size = 30;
    const int minDropInterval = 40;
    const int maxDropInterval = 160;
    const int highBorder = 100;

    const int minSpawnInterval = 100;
    const int maxSpawnInterval = 1000;

/// Your code here...
    this->setWindowTitle(QString("SQUARE FALLDOWN"));
    auto deployTimer = new QTimer(this);
    deployTimer->setInterval(QRandomGenerator::global()->bounded(minSpawnInterval, maxSpawnInterval));

    QObject::connect(deployTimer,
                     &QTimer::timeout,
                     this,
                     [=] {
            QPushButton* newButton = new QPushButton(this);

            QPalette palette1 = newButton->palette();
            palette1.setColor(QPalette::Button,
                             QColor(QRandomGenerator::global()->bounded(200),
                                    QRandomGenerator::global()->bounded(255),
                                    QRandomGenerator::global()->bounded(100, 255))); //to make them colorfull but with no chance of being same as BG
            newButton->setAutoFillBackground(true);
            newButton->setPalette(palette1);

            newButton->setGeometry(QRect(QRandomGenerator::global()->bounded(this->width()-tile_size),
            highBorder,
            tile_size,
            tile_size));
//        newButton->setMouseTracking(true);

            auto dropTimer = new QTimer(newButton);
            dropTimer->setInterval(QRandomGenerator::global()->bounded(minDropInterval, maxDropInterval));

            QObject::connect(dropTimer, &QTimer::timeout,  newButton, [=] {
                int delta = 2;
                if (newButton->underMouse()) delta *=2;
                newButton->move(newButton->x(),newButton->y()+delta);
                if (newButton->y() >= this->height()-tile_size) {
                    if (!isLost) {
                        QPalette palette = this->palette();
                        palette.setColor(QPalette::Window, QColor(250,0,80));
                        this->setAutoFillBackground(true);
                        this->setPalette(palette);
                        this->setWindowTitle(QString("LOST-LOST-LOST"));
                        this->update();
                        isLost = true;
                    }
                    newButton->deleteLater();
                }
            } );

//        QObject::connect(newButton, &QPushButton::clicked, newButton, [=] {
//            if (!isLost) newButton->deleteLater();
//        } );
        QObject::connect(newButton, &QPushButton::pressed, newButton, [=] { //makes it easier to play
            if (!isLost) newButton->deleteLater();
        } );

        newButton->show();
        dropTimer->start();
    } );


    QObject::connect(deployTimer, &QTimer::timeout,  this, [=] {
        deployTimer->setInterval(QRandomGenerator::global()->bounded(minSpawnInterval, maxSpawnInterval));
    } );
    deployTimer->start();
}

MainWindow::~MainWindow()
{
}


