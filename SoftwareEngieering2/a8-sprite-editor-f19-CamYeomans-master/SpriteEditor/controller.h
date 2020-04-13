#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFrame>
#include <QColor>
#include <QState>
#include "model.h"
#include "helpguide.h"
#include "startpage.h"
#include "createnewpage.h"
#include "preview.h"
#include <QDebug>
#include <QColorDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Controller; }
QT_END_NAMESPACE

class Controller : public QMainWindow
{
    Q_OBJECT

public:
    Controller(QWidget *parent = nullptr);
    ~Controller();
    Ui::Controller *ui;
    model* spriteModel;
    HelpGuide helpGuide;
    StartPage startPage;
    CreateNewPage createNewPage;
    Preview preview;


    void startStopAnimation(bool runAnimation);

public slots:
    void resizeFrame(unsigned int x, unsigned int y);
    void drawingAreaClicked(QPointF);
    void drawingAreaUnclicked();
    void mouseMovedInDrawingArea(QPointF);
    void updateDrawingLabel();
    void changeColorBoxSlot(QColor newColor);
    void displayStartPage();

private:
    void playAnimation(int frameRate);
    void playAnimationHelper();
    void loopOverFramesForAnimation();
    //void resizeAnimationPreviewFrame(unsigned x, unsigned y);
    QTimer* animationTimer;
    unsigned int animationPlayBackCurrentFrame;
    QColor transparentPixel;
    bool toolActivatedViaUserClickInDrawingArea;
    QPointF previousDrawPosition;
    void unhighlightMenuButtons();

signals:
    void spriteSize(unsigned x, unsigned y);
    void colorButtonPressed(QColor selectedColor);
    void penButtonPressed();
    void eraserButtonPressed();
    void eyeDropperButtonPressed(QColor selectedColor);
    void rotateLeftButtonPressed();
    void rotateRightButtonPressed();
    void exportToGifSignal(std::string fileName);
    void saveSpriteSheetSignal(std::string fileName);
    void loadSpriteSheetSignal(std::string fileName);
    void createNewFrameSignal();
    void updateFPSSignal(int newFps);
    void changeFrameSignal(int newFrame);
    void deleteFrame(int frame2Delete);
    void undoButtonClicked();
    void newSpriteSheetSignal();


private slots:
    void on_colorButton_clicked();
    void on_eraseButton_clicked();
    void on_penButton_clicked();
    void on_eyeDropperButton_clicked();
    void on_rotateLeftButton_clicked();
    void on_rotateRightButton_clicked();
    void on_actionPen_Tool_triggered();
    void on_animationButton_clicked();
    void on_actionErase_Tool_triggered();
    void on_actionEyedropper_Tool_triggered();
    void on_actionRotate_Left_triggered();
    void on_actionRotate_Right_triggered();
    void on_actionExport_to_Gif_triggered();
    void on_actionSave_SpriteSheet_triggered();
    void on_actionLoad_SpriteSheet_triggered();
    void on_actionNew_Frame_triggered();
    void on_actionPreview_GIF_triggered();
    void on_horizontalSlider_valueChanged(int value);
    void on_actionGuide_triggered();
    void on_currentFrameSpinBox_valueChanged(int arg1);
    void on_actionDelete_Current_Frame_triggered();
    void on_undo_clicked();
    void on_addFrame_pressed();
    void on_actionNew_SpriteSheet_triggered();
    void updateSpinSlot();
    void setSpinMaxSlot(int newMax);
};
#endif // CONTROLLER_H
