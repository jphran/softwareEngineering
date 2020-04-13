#include "spriteplayer.h"

SpritePlayer::SpritePlayer(QVector<QImage> images, QLabel* label) : label(label) {
    for (int i = 0; i < frames.length(); i++) {
        QPixmap frame;
        frame.convertFromImage(images[i]);
        frames.append(frame);
    }

    currentIndex = 0;
}

void SpritePlayer::play(int ms) {
    if (currentIndex >= frames.length()) {
        currentIndex = 0;
    }
    label->setPixmap(frames[currentIndex]);
    currentIndex++;

    QTimer::singleShot(ms, [=](){ play(ms); });
}
