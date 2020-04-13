/*
* S.I.M.P
* Team 2
* https://github.com/University-of-Utah-CS3505/a8-sprite-editor-f19-Mattmathics
*/
#ifndef CANVAS_H
#define CANVAS_H

#include <QObject>
#include <QVector>
#include <QLinkedList>

#include "frame.h"
#include "json.hpp"

/**
 * The Canvas class
 *
 * @author Kyungyoon Kim
 */
class Canvas
{
public:

    /**
     * Create Canvas Object
     *
     * @param sizeX x size of canvas
     * @param sizeY y size of canvas
     * @param frames all frames
     */
    Canvas(int sizeX, int sizeY, QVector<Frame> *frames = nullptr);

    Canvas(const Canvas &canvas);

    /**
     * Create canvas object from JSON string.
     *
     * This method is static method.
     *
     * @param jsonString JSON string
     * @return Canvas object
     */
    static Canvas fromJson(QString jsonString);

    std::string toJson();

    void update();

    Frame* moveFrame(int index);
    Frame* getFrame(int index);
    Frame* getCurrentFrame();
    void addFrame(int index = -1);
    void addFrame(Frame frame, int index = -1);
    void deleteFrame(int index);
    int sizeFrame();
    int currentIndex();
    bool getSaved();
    void setSaved(bool status);

    int getWidth();
    int getHeight();

private:
    QVector<Frame> frames;
    int sizeX;
    int sizeY;
    int index;
    bool saved = false;

};

#endif // CANVAS_H
