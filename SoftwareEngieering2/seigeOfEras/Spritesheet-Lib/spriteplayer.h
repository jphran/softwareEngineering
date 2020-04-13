#ifndef SPRITEPLAYER_H
#define SPRITEPLAYER_H

#include <QtWidgets/QLabel>
#include <QTimer>

class SpritePlayer
{
private:
    QLabel* label;
    QVector<QPixmap> frames;
    int currentIndex;

public:
    SpritePlayer(QVector<QImage> images, QLabel* label);
    void play(int ms);
};

#endif // SPRITEPLAYER_H
