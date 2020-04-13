#include "projectmanager.h"

#include <QDebug>

#include <vector>
#include <cstdint>
#include "gifanim.h"

bool ProjectManager::saveAsPng(Frame *frame, QString uri)
{
    QFile file(uri);
    return frame->getImage().save(file.fileName(), "PNG");
}

bool ProjectManager::saveAsStripImage(Canvas *canvas, QString uri)
{
    QFile file(uri);

    int width = canvas->getWidth() * canvas->sizeFrame();
    int height = canvas->getHeight();

    QImage stripImage(width, height, QImage::Format_ARGB32);

    for(int index = 0; index < canvas->sizeFrame(); index++)
    {
        for(int x = 0; x < canvas->getWidth(); x++)
        {
            for(int y =0; y < canvas->getWidth(); y++)
            {
                QColor color = canvas->getFrame(index)->getPixel(x, y);
                int pointX = x + (index * canvas->getWidth());
                int pointY = y;

                stripImage.setPixel(pointX, pointY, color.rgba());
            }
        }
    }

    return stripImage.save(file.fileName(), "PNG");
}

bool ProjectManager::saveAsGif(Canvas *canvas, QString uri, int fps)
{
    GifAnim gifAnim;
    GifWriter g;

    uint32_t w = canvas->getWidth();
    uint32_t h = canvas->getHeight();
    uint32_t delay = 100/fps;
    uint32_t bitDepth = 8;
    int loopCount = 0;

    gifAnim.GifBegin(&g, uri.toStdString().c_str(), w, h, delay, loopCount, bitDepth);

    for(int i = 0; i < canvas->sizeFrame(); i++)
    {
        Frame* frame = canvas->getFrame(i);
        std::vector<uint8_t> image;

        for(int y = 0; y < h; y++)
        {
            for(int x = 0; x < w; x++)
            {
                QColor color = frame->getPixel(x, y);

                image.push_back(color.red());
                image.push_back(color.green());
                image.push_back(color.blue());
                image.push_back(color.alpha());
            }
        }

        gifAnim.GifWriteFrame(&g, image.data(), w, h, delay, bitDepth);
        image.clear();
    }

    gifAnim.GifEnd(&g);
}

bool ProjectManager::saveProject(Canvas *canvas, QString uri)
{
    QFile file(uri);
    QString jsonString = QString::fromStdString(canvas->toJson());

    qDebug() << "Save file to : " << uri << endl << jsonString << endl;

    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        stream << jsonString << endl;

        return true;
    }

    return false;
}

Canvas ProjectManager::openProject(QString uri)
{
    QFile file(uri);

    qDebug() << "Load project from : " << uri << endl;

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        QString line = in.readLine();
        file.close();

         qDebug() << "JSOM :: " << line << endl;

        return Canvas::fromJson(line);
    }

    qDebug() << "Load Project Failed" << endl;
    throw false;
}
