#include <QTimer>
#include <QPushButton>
#include <QRandomGenerator>
#include <QMap>
#include <QString>
#include <QDebug>
#include <iostream>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(300, 600);

/// Your code here...
    static bool isLost = false;
    const int tile_size = 30;
    const int highBorder = 100;

    const int minSpawnInterval = 100;
    const int maxSpawnInterval = 1000;
    int butNum = 0;
    const int lowestSpeed = 1;
    const int highestSpeed = 6;
    static int tick = 0;
    QMap<QString, int> dropStepMap;

    dropStepMap.insert(QString("test"), 1);
    this->setWindowTitle(QString("SQUARE FALLDOWN"));
    auto deployTimer = new QTimer(this);
    deployTimer->setInterval(QRandomGenerator::global()->bounded(minSpawnInterval, maxSpawnInterval));
    auto bigDropTimer = new QTimer(this);
    bigDropTimer->setInterval(40);

    QObject::connect(deployTimer,
                     &QTimer::timeout,
                     this,
                     [=]() mutable {
            QPushButton* newButton = new QPushButton(this);
            newButton->setAccessibleName(QString("But%1").arg(butNum));
            butNum++;
            int speed = QRandomGenerator::global()->bounded(lowestSpeed,highestSpeed);
            QString test(newButton->accessibleName());

            dropStepMap.insert(test, speed);
            QPalette palette1 = newButton->palette();
            palette1.setColor(QPalette::Window,
                             QColor(QRandomGenerator::global()->bounded(200),
                                    QRandomGenerator::global()->bounded(255),
                                    QRandomGenerator::global()->bounded(100, 255))); //to make them colorfull but with no chance of being same as BG
            newButton->setAutoFillBackground(true);
            newButton->setPalette(palette1);

            newButton->setGeometry(QRect(QRandomGenerator::global()->bounded(this->width()-tile_size),
            highBorder,
            tile_size,
            tile_size));

            QObject::connect(bigDropTimer, &QTimer::timeout,  newButton, [=] {

                if (tick % dropStepMap[newButton->accessibleName()] == 0) {
                    int step = 2;
                    if (newButton->underMouse()) step=4;

                    newButton->move(newButton->x(),newButton->y()+step);
//                    std::cout << newButton->accessibleName().toStdString() << "tick" << tick << std::endl;
                    if (newButton->y() >= this->height() - tile_size) {
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

                }


            } );
        QObject::connect(newButton, &QPushButton::pressed, newButton, [=] { //makes it easier to play
            if (!isLost) newButton->deleteLater();
        } );

        newButton->show();
    } );

    QObject::connect(bigDropTimer, &QTimer::timeout, this, [=] () mutable {
        tick++;
//        std::cout << "tick++" << tick << endl;
    });
    QObject::connect(deployTimer, &QTimer::timeout,  this, [=] {
        deployTimer->setInterval(QRandomGenerator::global()->bounded(minSpawnInterval, maxSpawnInterval));
    } );
    deployTimer->start();
    bigDropTimer->start();
}

MainWindow::~MainWindow()
{
}


