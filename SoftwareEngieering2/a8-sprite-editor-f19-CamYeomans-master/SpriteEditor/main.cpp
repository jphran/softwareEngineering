#include "controller.h"
#include "model.h"
#include "ui_controller.h"
#include "clickablespritedisplay.h"
#include <QApplication>
#include <QPushButton>
#include <iostream>
#include <QIcon>
#include <QMouseEvent>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setWindowIcon(QIcon("../../../../../images/icon.png")); // Mac path
    a.setWindowIcon(QIcon("../images/icon.png")); // Windows and Linux path
    Controller w;
    model* spriteModel;
    spriteModel = new model();
    w.spriteModel = spriteModel;
    w.show();

    // Events for drawing on the drawLabel/display
    QObject::connect(*(&w.ui->drawLabel), SIGNAL(clicked(QPointF)), &w, SLOT(drawingAreaClicked(QPointF)));
    QObject::connect(*(&w.ui->drawLabel), SIGNAL(unclicked(QPointF)), &w, SLOT(drawingAreaUnclicked()));
    QObject::connect(*(&w.ui->drawLabel), SIGNAL(mouseMoved(QPointF)), &w, SLOT(mouseMovedInDrawingArea(QPointF)));

    // Events from the model to update the display
    QObject::connect(*(&w.spriteModel), SIGNAL(updateSpriteDisplay()), &w, SLOT(updateDrawingLabel()));
    QObject::connect(*(&w.spriteModel), SIGNAL(updateSpinSignal()), &w, SLOT(updateSpinSlot()));

    // Connections from View to Model for simple buttons and actions
    QObject::connect(&w, SIGNAL(rotateLeftButtonPressed()), spriteModel, SLOT(rotateFrameLeft()));
    QObject::connect(&w, SIGNAL(undoButtonClicked()), spriteModel, SLOT(undoEdit()));
    QObject::connect(&w, SIGNAL(rotateRightButtonPressed()), spriteModel, SLOT(rotateFrameRight()));
    QObject::connect(*(&w.spriteModel), SIGNAL(changeColorBox(QColor)), &w, SLOT(changeColorBoxSlot(QColor)));
    QObject::connect(&w, SIGNAL(updateFPSSignal(int)), spriteModel, SLOT(setFPSSlot(int)));
    // Add, delete, or change frames
    QObject::connect(&w, SIGNAL(createNewFrameSignal()), spriteModel, SLOT(addFrame()));
    QObject::connect(&w, SIGNAL(deleteFrame(int)), spriteModel, SLOT(removeFrameSlot(int)));
    QObject::connect(&w, SIGNAL(changeFrameSignal(int)), spriteModel, SLOT(changeFrameSlot(int)));
    QObject::connect(*(&w.spriteModel), SIGNAL(updateSpinMaxSignal(int)), &w, SLOT(setSpinMaxSlot(int)));

    // hook model update to sprite display bounds
    QObject::connect(*(&w.spriteModel), SIGNAL(resizeSpriteFrame(unsigned int, unsigned int)), &w, SLOT(resizeFrame(unsigned int, unsigned int)));

    //Saving, Loading, Exporting, and new
    QObject::connect(&w, SIGNAL(exportToGifSignal(std::string)), spriteModel, SLOT(exportAsGifSlot(std::string)));
    QObject::connect(&w, SIGNAL(saveSpriteSheetSignal(std::string)), spriteModel, SLOT(saveAsJsonSlot(std::string)));
    QObject::connect(&w, SIGNAL(loadSpriteSheetSignal(std::string)), spriteModel, SLOT(loadFromJsonSlot(std::string)));
    //open up createnewpage when "Create New" is clicked in startpage
    QObject::connect(&w.startPage, SIGNAL(createNewClicked()), &w.createNewPage, SLOT(openCreateNewPage()));
    //redisplay start page if user hits cancel when defining dimensions for a new page
    QObject::connect(&w.createNewPage, SIGNAL(openStartPage()), &w, SLOT(displayStartPage()));
    QObject::connect(&w, SIGNAL(newSpriteSheetSignal()), spriteModel, SLOT(newSpriteSheetSlot()));

    //Connections from StartPage to other PopUps
    //Load
    QObject::connect(&w.startPage, SIGNAL(loadDudeSignal(std::string)), spriteModel, SLOT(loadFromJsonSlot(std::string)));
    //open up help guide if learn button is clicked on startpage
    QObject::connect(&w.startPage, SIGNAL(learnClicked()), &w, SLOT(on_actionGuide_triggered()));
    QObject::connect(&w.startPage, SIGNAL(cancelClicked()), &w.startPage, SLOT(on_closeButton_released()));

    //Sizing and resizing
    //Sprite Dimensions from CreateNew to SpriteModel
    QObject::connect(&w.createNewPage, SIGNAL(createNewAccepted(unsigned, unsigned)), &w, SLOT(resizeFrame(unsigned,unsigned)));
    // hook model update to sprite display bounds
    QObject::connect(*(&w.spriteModel), SIGNAL(resizeSpriteFrame(unsigned int, unsigned int)), &w, SLOT(resizeFrame(unsigned int, unsigned int)));
     // connects inputed sprite size to the model
    QObject::connect(&w, SIGNAL(spriteSize(unsigned, unsigned)), spriteModel, SLOT(getSize(unsigned, unsigned)));

    return a.exec();
}
