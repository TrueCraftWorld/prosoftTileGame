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
    static bool isLost = false;
    constexpr int tile_size = 30;
    constexpr int highBorder = 100;

    constexpr int minSpawnInterval = 100;
    constexpr int maxSpawnInterval = 1000;

    constexpr int lowestSpeed = 1;
    constexpr int highestSpeed = 8;



    this->setWindowTitle(QString("SQUARE FALLDOWN"));
    auto deployTimer = new QTimer();
    deployTimer->setInterval(QRandomGenerator::global()->bounded(minSpawnInterval, maxSpawnInterval));
    auto bigDropTimer = new QTimer();
    bigDropTimer->setInterval(100);

    QObject::connect(deployTimer,
                     &QTimer::timeout,
                     this,
                     [this, highBorder, tile_size, bigDropTimer, deployTimer]() {
        QPushButton* newButton = new QPushButton(this);
        const int speed = QRandomGenerator::global()->bounded(lowestSpeed,highestSpeed);
        newButton->setAccessibleName(QString("%1").arg(speed));

        newButton->setGeometry(QRect(QRandomGenerator::global()->bounded(width()-tile_size),
        highBorder,
        tile_size,
        tile_size));

        QObject::connect(bigDropTimer, &QTimer::timeout,  newButton, [this, newButton, tile_size] {

            int step = newButton->accessibleName().toInt();
            if (newButton->underMouse()) step*=2;

            newButton->move(newButton->x(),newButton->y()+step);

            if (newButton->y() < height() - tile_size) return;

            newButton->deleteLater();

            if (isLost) return;

            QPalette palette = this->palette();
            palette.setColor(QPalette::Window, QColor(250,0,80));
            setAutoFillBackground(true);
            setPalette(palette);
            setWindowTitle(QString("LOST-LOST-LOST"));
            update();
            isLost = true;
        } );
        deployTimer->setInterval(QRandomGenerator::global()->bounded(minSpawnInterval, maxSpawnInterval));
        if (!isLost) QObject::connect(newButton, &QPushButton::pressed, newButton, &QPushButton::deleteLater);

        newButton->show();
    } );


    deployTimer->start();
    bigDropTimer->start();
}

MainWindow::~MainWindow()
{
}


