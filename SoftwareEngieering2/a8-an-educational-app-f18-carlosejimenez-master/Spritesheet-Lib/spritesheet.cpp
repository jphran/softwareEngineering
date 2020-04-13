#include <QDir>
#include "spritesheet.h"

Spritesheet::Spritesheet(const QString& filename, int width, int height, int numColumns, int numRows) {
    // get the full spritesheet as a QImage
    QImage image(filename);

    // populate 'frames' with all the frames in the spritesheet
    for (int rowIndex = 0; rowIndex < numRows; rowIndex++) {
        int originY = rowIndex * height;
        for (int colIndex = 0; colIndex < numColumns; colIndex++) {
            int originX = colIndex * width;
            QImage frame = image.copy(originX, originY, width, height);
            frames.append(frame);
        }
    }
}

QVector<QImage> Spritesheet::getFrames() {
    return frames;
}

void Spritesheet::writeToDir(const QString& dirName) {
    // create directory with desired output name if it doesn't already exist
    if (!QDir().exists(dirName)) {
        QDir().mkdir(dirName);
    }

    // write all of the frames to the output directory
    for (int i = 0; i < frames.length(); i++) {
        // determine the output filename for this frame
        char iChar = '0' + i;
        QString saveName = dirName + "/" + iChar + ".png";

        // retrieve and write the frame
        QImage frame = frames[i];
        frame.save(saveName);
    }
}
