#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include <QImageReader>
#include <QImageWriter>
#include <QVector>

class Spritesheet {
private:
    QVector<QImage> frames;

public:
    Spritesheet(const QString& filename, int width, int height, int numColumns, int numRows);
    QVector<QImage> getFrames();
    void writeToDir(const QString& dirName);
};

#endif // SPRITESHEET_H
