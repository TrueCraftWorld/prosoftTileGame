#include <QTimer>
#include <QPushButton>
#include <QRandomGenerator>
#include <QMap>
#include <QString>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(300, 600);

/// Your code here...

    const int tile_size = 30;
    const int highBorder = 100;

    const int minSpawnInterval = 100;
    const int maxSpawnInterval = 1000;

    const int lowestSpeed = 2;
    const int highestSpeed = 7;

    static bool isLost = false;

    int butNum = 0;

    auto deployTimer = new QTimer(this);
    auto bigDropTimer = new QTimer(this);
    QMap<QString, int> dropStepMap;

    this->setWindowTitle(QString("SQUARE FALLDOWN"));

    deployTimer->setInterval(QRandomGenerator::global()->bounded(minSpawnInterval, maxSpawnInterval));
    bigDropTimer->setInterval(50);

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

        QObject::connect(bigDropTimer, &QTimer::timeout,  newButton, [=] {
                int step = (newButton->underMouse()) ? 2*dropStepMap[newButton->accessibleName()] :
                                                        dropStepMap[newButton->accessibleName()];
                newButton->move(newButton->x(),newButton->y()+step);
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
        } );
        QObject::connect(newButton, &QPushButton::clicked, newButton, [=] { //makes it easier to play
            if (!isLost) newButton->deleteLater();
        } );
        newButton->show();
    } );

    QObject::connect(deployTimer, &QTimer::timeout,  this, [=] {
        deployTimer->setInterval(QRandomGenerator::global()->bounded(minSpawnInterval, maxSpawnInterval));
    } );
    deployTimer->start();
    bigDropTimer->start();
}

MainWindow::~MainWindow()
{
}


