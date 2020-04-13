/*
* S.I.M.P
* Team 2
* https://github.com/University-of-Utah-CS3505/a8-sprite-editor-f19-Mattmathics
*/
#include "frame.h"

#include <QDebug>

using namespace std;
using json = nlohmann::json;

#define MAX_HISTORY 50

Frame::Frame(int sizeX, int sizeY, QImage *copyPixels)
{
    this->sizeX = sizeX;
    this->sizeY = sizeY;

    // If copy obejct is null, make new image object.
    if (copyPixels == nullptr)
    {
        image = QImage(sizeX, sizeY, QImage::Format_ARGB32);

        //Set all pixels to inital color
        QColor initalColor(0, 0, 0, 0);

        for(int x = 0; x < sizeX; x++)
            for(int y = 0; y < sizeY; y++)
                setPixel(x, y, initalColor);
    }
    else
       image = copyPixels->copy();
}

QColor Frame::getPixel(int x, int y)
{
    return QColor::fromRgba(image.pixel(x,y));
}

void Frame::setPixel(int x, int y, QColor color)
{
    image.setPixel(x, y, color.rgba());
}

QImage Frame::getImage()
{
    return image;
}

void Frame::captureHistory()
{
    // When historyIndex is not 0, user did undo last time.
    // So delete all undone datas.
    if(historyIndex != 0) {
        for(int i = 0; i < historyIndex; i++)
            histories.removeFirst();

        historyIndex = 0;
    }

    // Capture current image
    histories.push_front(image);

    // If vector is over maximum history size, delete old data.
    while(histories.size() > MAX_HISTORY)
        histories.removeLast();
}

void Frame::undo()
{
    image.swap(histories[historyIndex++]);
}

void Frame::redo()
{
    image.swap(histories[--historyIndex]);
}

bool Frame::isRedoable()
{
    return historyIndex != 0;
}

bool Frame::isUndoable()
{
    return histories.size() != historyIndex;
}

std::string Frame::toJson()
{
    json j;
    vector<vector<vector<int>>> pixels;

    // get the colors of each pixel and convert them to JSON
    for(int x = 0; x < sizeX; x++)
    {
        vector<vector<int>> temp;

        for(int y = 0; y < sizeY; y++)
        {
            vector<int> colors;

            QColor color = this->getPixel(x, y);
            colors.push_back(color.red());
            colors.push_back(color.green());
            colors.push_back(color.blue());
            colors.push_back(color.alpha());

            temp.push_back(colors);
        }

        pixels.push_back(temp);
    }

    j["pixels"] = pixels;

    return j.dump();
}

Frame Frame::fromJson(QString jsonString, int sizeX, int sizeY)
{
    auto j = json::parse(jsonString.toStdString());

    vector<vector<vector<int>>> pixels = j["pixels"].get<vector<vector<vector<int>>> >();

    // get the colors of each pixel from JSON and set the colors of pixels in frame
    Frame frame(sizeX, sizeY);
    for (int x = 0; x < sizeX; x++)
    {
        for(int y = 0; y < sizeY; y++)
        {
            vector<int> rgba = pixels.at(x).at(y);
            QColor color(rgba[0], rgba[1], rgba[2], rgba[3]);

            frame.setPixel(x, y, color);
        }
    }

    return frame;
}

int Frame::getSizeX()
{
	return sizeX;
}

int Frame::getSizeY()
{
	return sizeY;
}
