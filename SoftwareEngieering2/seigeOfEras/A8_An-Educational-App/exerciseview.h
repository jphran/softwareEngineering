#ifndef EXERCISEVIEW_H
#define EXERCISEVIEW_H

#include "Box2D/Box2D.h"
#include <QColor>
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QWidget>
#include <deque>

class ExerciseView : public QGraphicsView
{

    Q_OBJECT

public:
    enum Mode {SHIFT, SWAP, MERGE};
    Mode mode;

private:
    std::vector<QColor> boxColors;
    std::vector<QColor> colorVector;
    QGraphicsScene *scene;
    QGraphicsRectItem *barBoxes[16];
    QGraphicsTextItem *barNumTexts[16];
    int barBoxesValues[16];
    int originalBoxesValues[16];
    int originalBoxesXPos[16];
    QGraphicsRectItem* activeRect;
    QGraphicsRectItem *ground;
    QGraphicsRectItem *higherGround;
    QGraphicsRectItem *dividingLine;
    int xpos[16];
    int ypos[16];
    int ySize[16];
    int xSize[16];
    float32 timeStep;
    int32 velocityIterations;
    int32 positionIterations;
    b2FixtureDef fixtureDef[16];
    b2PolygonShape dynamicBox[16];
    b2Body* body[16];
    b2World* world;
    b2Vec2* gravity;
    int mergeStep;
    int mergeLocation;
    std::deque<int> emptyGrooveLocations;
    std::vector<QGraphicsRectItem *> subArray;

    bool dragging = false;
    void resetBoxesFromView();
    QRect *threshold;

    int activeBoxIndex = -1;

public:
    QTimer* timer;
    QTimer* celebrateTimer;

    ExerciseView(QWidget* parent=nullptr);
    inline int* get_box_values(){ return barBoxesValues; }
    void celebrate();

    void construct_boxes(int box_values[], bool random);
    int* getCurrentArrayState();
    int getBoxYPos(const QGraphicsRectItem* box);
    int getGrooveIndexAt(int x);
    int getGrooveIndexFromBox(QGraphicsRectItem* box);
    int getXPosFromGroove(int x);
    bool pointerInBarBoxes(void* pointer);
    bool pointerInSubArray(void* pointer);
    void setBoxColor(int index, QColor color);
    void shiftSwap(QGraphicsRectItem* shiftBox, int activeGroove);
    void swap(QGraphicsRectItem* box1, QGraphicsRectItem* box2);
    void setupBoxesPosition(int array[16]);
    void startAlgorithm(Mode algoMode);
    void setupMergeSortPositions();

    int getBoxYPosMerge(const QGraphicsRectItem *box);
    void getNextMergeBoxes();
    void incrementMergeLocation();
    void resetMergeVars();
    void resetBoxColors();

signals:
    // When inital motion is done, we want to start doing other things.
    void arrayUpdated(int array[]);
    void motionComplete();
    void disableButtons();

public slots:
    void update();
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
};

#endif // EXERCISEVIEW_H
