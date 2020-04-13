#include "exerciseview.h"
#include <iostream>
#include <QMouseEvent>
#include <QWidget>
#include <cmath>
#include <QDebug>

ExerciseView::ExerciseView(QWidget* parent) : QGraphicsView(parent), boxColors(16, Qt::yellow)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    this->setInteractive(false);

    for (int i = 0; i < 16; i++)
        barBoxesValues[i] = 50;

    construct_boxes(barBoxesValues, false);

    // For celebrate.
    colorVector.push_back(Qt::red);
    colorVector.push_back(Qt::blue);
    colorVector.push_back(Qt::yellow);
}
void ExerciseView::changeBoxColor(int boxIndexBegin, int boxIndexEnd )
{
    barBoxes[boxIndexBegin]->setBrush(Qt::magenta);
    barBoxes[boxIndexEnd]->setBrush(Qt::cyan);
    return;
}
void ExerciseView::celebrate()
{
    static int count = 0;

    if (count++>=6)
    {
        count = 0;
        for(int i = 0; i < 16; i++)
            barBoxes[i]->setBrush(boxColors[i]);
        return;
    }

    for(int j = 0; j < 16; j++)
    {
        int var = (count + j) % colorVector.size();
        barBoxes[j]->setBrush(colorVector[var]);
    }

    QTimer::singleShot(200, this, &ExerciseView::celebrate);
}

void ExerciseView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        if (dragging == true)
        {
            // Moves text number around with box.
            int boxIndex = getGrooveIndexFromBox(activeRect);
            barNumTexts[boxIndex]->setPos(activeRect->pos().rx(), activeRect->pos().ry()-20);

            // For sorting selection sort, moves are swaps.
            if (mode == Mode::SWAP)
            {
                // itemList is the list of Rect and num items at the position of the mouse.
                QList<QGraphicsItem *>	itemList = items(event->pos());

                 /* We sort through num and rect items at the position of the mouse to only get the rect that we
                 are passing over. We do not want to set the num item to our highlightRect variable, or the
                 activeRect.*/
                for (int i = 0; i < itemList.size(); i++)
                {
                    if (pointerInBarBoxes(itemList.at(i)) && itemList.at(i) != activeRect)
                    {
                        QGraphicsRectItem* highlightRect = dynamic_cast<QGraphicsRectItem*>(itemList.at(i));
                        highlightRect->setBrush(Qt::blue);
                        dragging = true;
                        break;
                    }
                    else
                    {
                        /* This loop makes sure that all the boxes are green, if we are not hovering over
                        another box. Since previously highlighted boxes has left scope of the mouse.*/
                        for (int i = 0; i < 16; i++)
                            barBoxes[i]->setBrush(boxColors[i]);
                    }
                }
            }

            /* The Insertion sort algorithm uses shift, instead of swap. So, while we are moving, we
             * switch the activeRect and passingBoxs' assigned groove, and we only change the position
             * of the passingBox at this point. We do not want to move the activeRect just because
             * it is passing over another box. */
            else if (mode == Mode::SHIFT)
            {
                int passingGroove = getGrooveIndexAt(activeRect->pos().rx());
                int activeGroove = getGrooveIndexFromBox(activeRect);

                if (passingGroove != activeGroove)
                    shiftSwap(barBoxes[passingGroove], activeGroove);
            }
        }
    }

    QGraphicsView::mouseMoveEvent(event);
}

void ExerciseView::mousePressEvent(QMouseEvent *event)
{
    // It is necessary to check all items in the itemList to find a rect, because, if a user clicks on the bar number,
    // itemAt(), and items().at(0) return the number item's address, which we do not want to set as our activeRect.
    QList<QGraphicsItem *>	itemList = items(event->pos());
    for (int i = 0; i < itemList.size(); i++)
    {
        if (pointerInBarBoxes(itemList.at(i)))
        {
            activeRect = dynamic_cast<QGraphicsRectItem*>(itemList.at(i));
            dragging = true;
            break;
        }
    }

    QGraphicsView::mousePressEvent(event);
}

void ExerciseView::mouseReleaseEvent(QMouseEvent *event)
{   
    if (dragging == true && activeRect != nullptr)
    {
        // This makes sure that the number label for the box moves with the box.
        dragging = false;
        int boxIndex = getGrooveIndexFromBox(activeRect);
        barNumTexts[boxIndex]->setPos(activeRect->pos().rx(), activeRect->pos().ry()-20);

        // For mode SWAP, we are doing Selection sort.
        if (mode == Mode::SWAP)
        {
            QList<QGraphicsItem *>	itemList = items(event->pos());

            QGraphicsRectItem* swapRect = nullptr;

            // We sift through the itemsList to find a box that isn't activeRect. (The box to swap!)
            for (int i = 0; i < itemList.size(); i++)
            {
                if (pointerInBarBoxes(itemList.at(i)) && itemList.at(i) != activeRect)
                {
                    swapRect = dynamic_cast<QGraphicsRectItem*>(itemList.at(i));
                    break;
                }
            }

            // If swapRect is defined, we swap.
            if (swapRect != nullptr)
                swap(activeRect, swapRect);

            // Otherwise the user let go of the box in random space, and we want to return it to its native groove.
            else
            {
                QPointF resetPoint(getXPosFromGroove(getGrooveIndexFromBox(activeRect)), getBoxYPos(activeRect));
                activeRect->setPos(resetPoint);
                barNumTexts[getGrooveIndexFromBox(activeRect)]->setPos(resetPoint.rx(), resetPoint.ry()-20);
            }
        }

        // Mode SHIFT means we are doing Insertion sort.
        else if (mode == Mode::SHIFT)
        {
            // Just like Mouse Move event. Except, now we move activeRect into position.
            int passingGroove = getGrooveIndexAt(activeRect->pos().rx()+22);
            int activeGroove = getGrooveIndexFromBox(activeRect);

            if (passingGroove != activeGroove)
                shiftSwap(barBoxes[passingGroove], activeGroove);
            QPointF resetPoint(getXPosFromGroove(getGrooveIndexFromBox(activeRect)), getBoxYPos(activeRect));
            activeRect->setPos(resetPoint);
            barNumTexts[getGrooveIndexFromBox(activeRect)]->setPos(resetPoint.rx(), resetPoint.ry()-20);
        }

        // Notify AlgoTrainer that the user has made a new array.
        int* array = getCurrentArrayState();
        emit arrayUpdated(array);

        // Make sure that activeRect becomes nullptr once we release the box.
        activeRect = nullptr;
    }

    QGraphicsView::mouseReleaseEvent(event);
}

// Returns all of the bar boxes within the scene
QList<QGraphicsRectItem *> ExerciseView::getBarBoxes()
{
    QList<QGraphicsRectItem *> barBoxesList;
    foreach(QGraphicsItem *item, scene->items())
    {
        QGraphicsRectItem *boxes = dynamic_cast<QGraphicsRectItem *>(item);

        if(boxes && boxes->brush() == boxColors[0])
            barBoxesList.append(boxes);
    }

    return barBoxesList;
}

void ExerciseView::construct_boxes(int box_values[], bool random)
{
    scene = new QGraphicsScene(this);
    // These constants are what is necessary to set the scene's origin in conjunction with the view.
    scene->setSceneRect(QRect(502, 356, this->width(), this->height()));
//    scene->setSceneRect(QRect(0, 0, this->width(), this->height()));
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));

    if (random)
    {
        for (int i = 0; i < 16; i++)
        {
            xpos[i] = 50+62.5*i;
            ypos[i] = 0;
            xSize[i] = 55;
            ySize[i] = (rand() % 100)*2;
            barBoxesValues[i] = ySize[i];
            originalBoxesValues[i] = ySize[i];
            originalBoxesXPos[i] = 15;
        }
    }
    else
    {
        for (int i = 0; i < 16; i++)
        {
            xpos[i] = 50 + 62.5*i;
            ypos[i] = 0;
            xSize[i] = 55;
            ySize[i] = box_values[i];
            barBoxesValues[i] = box_values[i];
            originalBoxesValues[i] = ySize[i];
            originalBoxesXPos[i] = 15;
        }
    }

    this->setScene(scene);

    QBrush whiteBrush(Qt::white);
//    QBrush greenBrush(Qt::yellow);
    QPen blackpen(Qt::black);

    blackpen.setWidth(1);

    for (int i = 0; i < 16; i++)
    {
        barBoxes[i] = scene->addRect(0, 0, xSize[i], ySize[i], blackpen, boxColors[i]);
        barBoxes[i]->setFlag(QGraphicsRectItem::ItemIsSelectable);
        barBoxes[i]->setFlag(QGraphicsRectItem::ItemIsMovable);
        barBoxes[i]->setFlag(QGraphicsRectItem::ItemSendsGeometryChanges);
        barBoxes[i]->setPos(xpos[i], ypos[i]);
//        QString qs = QString::number(ySize[i]);
        barNumTexts[i] = scene->addText("");//qs);
        barNumTexts[i]->setPos(xpos[i], ypos[i]-20);

    }
    ground = scene->addRect(50, 660,1000,10,blackpen,whiteBrush);

    // Define the gravity vector.
    gravity = new b2Vec2(0.0f,20.0f);

    // Construct a world object, which will hold and simulate the rigid bodies.
    world = new b2World(*gravity);

    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(50.0f, 660.0f);

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world->CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2PolygonShape groundBox;

    // The extents are the half-widths of the box.
    groundBox.SetAsBox(10000.0f, 10.0f);

    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 10.0f);

    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef[16];
    for (int i = 0; i < 16; i++)
    {
        bodyDef[i].type = b2_dynamicBody;

        bodyDef[i].position.Set(xpos[i], 1.0f);
        body[i] = world->CreateBody(&bodyDef[i]);

        dynamicBox[i].SetAsBox(55, ySize[i]);

        fixtureDef[i].shape = &dynamicBox[i];

        // Set the box density to be non-zero, so it will be dynamic.
        fixtureDef[i].density = 1000.0f;

        // Override the default friction.
        fixtureDef[i].friction = 0.0f;

        fixtureDef[i].restitution = 0.0f;

        // Add the shape to the body.
        body[i]->CreateFixture(&fixtureDef[i]);
    }

    for (int i = 0; i < 16; i++)
    {
        barBoxes[i]->setFlag(QGraphicsRectItem::ItemIsSelectable);
        barBoxes[i]->setFlag(QGraphicsRectItem::ItemIsMovable);
        barBoxes[i]->setFlag(QGraphicsRectItem::ItemSendsGeometryChanges);
    }

    // Define another box shape for our dynamic body.
    // Define the dynamic body fixture.
    // Prepare for simulation. Typically we use a time step of 1/60 of a
    // second (60Hz) and 10 iterations. This provides a high quality simulation
    // in most game scenarios.
    timeStep = 0.4f;
    velocityIterations = 1;
    positionIterations = 1;
}

// Returns the array pointer that is displayed on the screen, extrapolated from the text number labels.
int* ExerciseView::getCurrentArrayState()
{
    int* array;
    array = new int[16];
    for (int i = 0; i < 16; i++)
        array[i] = barNumTexts[i]->toPlainText().toInt();

    return array;
}

// Returns the groove index at the x coordinate position of the scene.
int ExerciseView::getGrooveIndexAt(int x)
{
    if (x < 50)
        return 0;
    else if (x > 1050)
        return 15;
    else
        return static_cast<int>(ceil((16*(x-50))/1000));
}

// Returns the groove index of a box.
int ExerciseView::getGrooveIndexFromBox(QGraphicsRectItem *box)
{
    for (int i = 0; i < 16; i++)
    {
        if (box == barBoxes[i])
            return i;
    }

    return 0;
}

// Returns that Y coordinate position for any box. It is an offset of the box's height.
int ExerciseView::getBoxYPos(const QGraphicsRectItem* box)
{
    return static_cast<int>(floor(650 - box->rect().height()));
}

// Returns the X coordinate position for any groove index.
int ExerciseView::getXPosFromGroove(int x)
{
    if (x < 0)
        return 50;
    else if (x > 15)
        return 1000;
    else
    {
        return (50 + static_cast<int>(floor(62.5*x)));
    }
}

// Checks whether pointer is a box in our barBoxes array.
// We use this to distinguish between numText items and boxItems.
bool ExerciseView::pointerInBarBoxes(void *pointer)
{
    for (int i = 0; i < 16; i++)
        if (pointer == barBoxes[i])
            return true;
    return false;
}

void ExerciseView::setBoxColor(int index, QColor color)
{
    if (index <= 15 && index >= 0)
    {
        boxColors[index] = color;
        barBoxes[index]->setBrush(color);
    }
}

void ExerciseView::shiftSwap(QGraphicsRectItem* shiftingBox, int activeGroove)
{
    // We get the new groove to assign to the activeRect.
    int newActiveGroove = getGrooveIndexFromBox(shiftingBox);

    // We assign the shifting box to activeRect's groove,
    // and get the new X coordinates for the shifting box.
    int newShiftedGroove = activeGroove;
    int newShiftedXPos= getXPosFromGroove(newShiftedGroove);

    // We must swap the values in our model arrays, to make sure we are always consistent.
    std::swap(barBoxes[newActiveGroove], barBoxes[newShiftedGroove]);
    std::swap(barNumTexts[newActiveGroove], barNumTexts[newShiftedGroove]);

    // Note that we want to move the shifting box's X position, but we get its native Y position.
    QPointF newShiftingBoxPoint(newShiftedXPos, getBoxYPos(shiftingBox));

    // Only move the shifting box, not activeRect.
    shiftingBox->setPos(newShiftingBoxPoint);
    barNumTexts[newShiftedGroove]->setPos(newShiftingBoxPoint.rx(), newShiftingBoxPoint.ry()-20);
}

void ExerciseView::startAlgorithm(ExerciseView::Mode algoMode)
{
    mode = algoMode;
    if (timer)
        timer->start(4);
    else
        emit motionComplete();
}

void ExerciseView::swap(QGraphicsRectItem *box1, QGraphicsRectItem *box2)
{
    // We first get box1's coordinates.
    int box1Groove = getGrooveIndexFromBox(box1);
    int box1XPos = getXPosFromGroove(box1Groove);

    // And box2's coordinates.
    int box2Groove = getGrooveIndexFromBox(box2);
    int box2XPos= getXPosFromGroove(box2Groove);

    // We then define the new points for each.
    // Box1 will now have box2's X position, but keep its own Y position.
    // Box2 will now have box1's X position, but keep its own Y position.
    QPointF box1NewPoint(box2XPos, getBoxYPos(box1));
    QPointF box2NewPoint(box1XPos, getBoxYPos(box2));

    box1->setPos(box1NewPoint);
    box2->setPos(box2NewPoint);

    // We also want to move the text numbers with the boxes, after they shift.
    barNumTexts[box1Groove]->setPos(box1NewPoint.rx(), box1NewPoint.ry()-20);
    barNumTexts[box2Groove]->setPos(box2NewPoint.rx(), box2NewPoint.ry()-20);

    // We must swap the values in our model arrays, to make sure we are always consistent.
    std::swap(barBoxes[box1Groove], barBoxes[box2Groove]);
    std::swap(barNumTexts[box1Groove], barNumTexts[box2Groove]);

    // Finally, repaint the boxes to the normal color after they've been moved.
    barBoxes[box1Groove]->setBrush(boxColors[box1Groove]);
    barBoxes[box2Groove]->setBrush(boxColors[box2Groove]);
}

// Here we keep all the boxes in existence, but we just reassign their values, and reset their positions.
void ExerciseView::setupBoxesPosition(int array[16])
{
    int newx;
    int newy;
    for(int i = 0; i < 16; i++)
    {
        barBoxes[i]->setRect(0,0,55,array[i]*10);
        newx = getXPosFromGroove(i);
        newy = getBoxYPos(barBoxes[i]);
        barBoxes[i]->setPos(newx,newy);
        barNumTexts[i]->setPos(newx,newy-20);
        barNumTexts[i]->setPlainText(QString::number(array[i]));
        barNumTexts[i]->ensureVisible();
    }
}

// This is how we drop the boxes at the beginning of the app.
// We use Box2D to keep track of the falling blocks, until they are near their groove positions.
// We then stop the Box2D updates, kill the timer, and reset the view.
void ExerciseView::update()
{
    world->Step(timeStep, velocityIterations, positionIterations);

    b2Vec2 position[16];

    bool finished = true;

    for (int i = 0; i < 16; i++)
    {
        position[i] = body[i]->GetPosition();
        ypos[i] = (int)position[i].y;
        barBoxes[i]->setPos(xpos[i], ypos[i]);
        barNumTexts[i]->setPos(xpos[i], ypos[i]-20);
        if (barBoxes[i]->pos().ry() < getBoxYPos(barBoxes[i])-4)
            finished = false;
    }

    if (finished)
    {
        timer->stop();
        delete timer;

        // We set timer to nullptr so subsequent calls to startAlgorithm do not recreate the Box2D world, etc.
        timer = nullptr;
        this->setInteractive(true);

        // motionComplete notifies AlgoTrainer that it can start displaying arrays.
        emit motionComplete();
    }
}

