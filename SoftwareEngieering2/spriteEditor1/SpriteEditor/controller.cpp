#include "controller.h"
#include "ui_controller.h"
#include "clickablespritedisplay.h"
#include "model.h"
#include "toolTypesEnum.h"
#include <iostream>

#define RESIZE_FACTOR 16


Controller::Controller(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Controller)
{
     this->ui->setupUi(this);
     this->toolActivatedViaUserClickInDrawingArea = false;

     startPage.show();

     this->ui->drawLabel->setStyleSheet("background-color: rgba(255, 255, 255, 85);");
     this->ui->previewLabel->setStyleSheet("background-color: rgba(255, 255, 255, 85);");
}


Controller::~Controller()
{
    delete ui;
}

/**
 * This method updates the current width and height and scales the frames
 */
void Controller::updateDrawingLabel()
{
    int w = this->spriteModel->frames[this->spriteModel->currentWorkingFrameIdx].size().width();
    int h = this->spriteModel->frames[this->spriteModel->currentWorkingFrameIdx].size().height();
    this->ui->drawLabel->setPixmap(QPixmap::fromImage((spriteModel->frames[this->spriteModel->currentWorkingFrameIdx]).scaled(w,h,Qt::KeepAspectRatio)));
}

/**
 * recognizes when the drawing area has been clicked due to the position of the click
 */
void Controller::drawingAreaClicked(QPointF position)
{
    this->toolActivatedViaUserClickInDrawingArea = true;
    spriteModel->useTool(position.x(), position.y());

    this->previousDrawPosition = position;
}

/**
 * recognizes when the drawing area is no longer clicked
 */
void Controller::drawingAreaUnclicked()
{
    this->toolActivatedViaUserClickInDrawingArea = false;
    // Push the drawing change(s) onto the undo stack
    this->spriteModel->undoStack.push(this->spriteModel->frames[this->spriteModel->currentWorkingFrameIdx]);
}

/**
 * Keeps track of the mouse position in the drawing area
 */
void Controller::mouseMovedInDrawingArea(QPointF position)
{
    // If user has clicked in the drawing area, use the tool, and makes sure tool is in drawing area bounds
    if(this->toolActivatedViaUserClickInDrawingArea)
    {
        // Interpolate to draw between last position and current position
        double midX = position.x() + (position.x() - this->previousDrawPosition.x())/2;
        double midY = position.y() + (position.y() - this->previousDrawPosition.y())/2;

        double firstQuarterX = position.x() + (position.x() - midX)/2;
        double firstQuarterY = position.y() + (position.y() - midY)/2;

        double lastQuarterX = midX + (midX - this->previousDrawPosition.x())/2;
        double lastQuarterY = midY + (midY - this->previousDrawPosition.y())/2;

        spriteModel->useTool(firstQuarterX, firstQuarterY);
        spriteModel->useTool(midX, midY);
        spriteModel->useTool(lastQuarterX, lastQuarterY);
        spriteModel->useTool(position.x(), position.y());

        this->previousDrawPosition = position;
    }
}

/**
 * unhighlights the menu buttons when they are no longer in use
 */
void Controller::unhighlightMenuButtons()
{
    ui->penButton->setStyleSheet("background-color:rgb(44, 44, 44);");
    ui->eraseButton->setStyleSheet("background-color:rgb(44, 44, 44);");
    ui->eyeDropperButton->setStyleSheet("background-color:rgb(44, 44, 44);");
}


/**
 * resizes the sprite drawing frame
 */
void Controller::resizeFrame(unsigned int x, unsigned int y)
{
    unsigned int resize_factor = 16;
    QSize size(static_cast<int>(x*resize_factor), static_cast<int>(y*resize_factor));
    preview.resizePreviewArea(static_cast<int>(x*resize_factor), static_cast<int>(y*resize_factor));
    ui->drawLabel->setFixedSize(size);
    emit spriteSize(x, y);
}


/**
 * Animation driver.
 */
void Controller::startStopAnimation(bool runAnimation)
{
    if(runAnimation)
    {
        this->animationPlayBackCurrentFrame = 0;
        animationTimer = new QTimer(this);
        connect(animationTimer, &QTimer::timeout, this, QOverload<>::of(&Controller::loopOverFramesForAnimation));

        int interval = spriteModel->convertFramesPerSecondToMillisecondsBetweenFrames(this->spriteModel->fps);
        animationTimer->start(interval);
    }
    else
    {
        animationTimer->stop();
    }
}

/**
 * goes over each frame in the spritesheet and plays it over
 */
void Controller::loopOverFramesForAnimation()
{
    QImage& displayFrame = this->spriteModel->frames[this->animationPlayBackCurrentFrame];

    if(this->animationPlayBackCurrentFrame < this->spriteModel->frames.size() - 1)
    {
        this->animationPlayBackCurrentFrame++;
    }
    else
    {
        this->animationPlayBackCurrentFrame = 0;
    }

    int w = this->ui->previewLabel->width();
    int h = this->ui->previewLabel->height();
    this->ui->previewLabel->setPixmap(QPixmap::fromImage(displayFrame).scaled(w,h,Qt::KeepAspectRatio));
}

/**
 * triggers the help key
 */
void Controller::on_actionGuide_triggered()
{
    helpGuide.show();
}

/**
 * slot to display start page if user cancels on creating new page
 */
void Controller::displayStartPage()
{
    startPage.show();
}

/**
 * slot for clicking color button
 */
void Controller::on_colorButton_clicked()
{
    QColor newColor = QColorDialog::getColor(Qt::black,this,"Choose a Color");
    QString styleSheetString = "border-radius: 4px; \n border: 2px solid white; \n background-color: " + newColor.name();
    ui->selectedColorBox->setStyleSheet(styleSheetString);
    spriteModel->setColor(newColor);
}

/**
 * This slot takes in a QColor, and tells the style sheet to change it
 */
void Controller::changeColorBoxSlot(QColor newColor)
{
    QString styleSheetString = "border-radius: 4px; \n border: 2px solid white; \n background-color: " + newColor.name();
    ui->selectedColorBox->setStyleSheet(styleSheetString);
}


/**
 * slot for clicking color button
 */
void Controller::on_eraseButton_clicked()
{
    spriteModel->setPenMode(eraser);
    unhighlightMenuButtons();
    ui->eraseButton->setStyleSheet("background-color:rgb(189, 189, 189);");
}

/**
 * slot for clicking pen button
 */
void Controller::on_penButton_clicked()
{
    spriteModel->setPenMode(brush);
    unhighlightMenuButtons();
    ui->penButton->setStyleSheet("background-color:rgb(189, 189, 189);");
}


/**
 * slot for clicking eye dropper button
 */
void Controller::on_eyeDropperButton_clicked()
{
    spriteModel->setPenMode(eyedropper);
    unhighlightMenuButtons();
    ui->eyeDropperButton->setStyleSheet("background-color:rgb(189, 189, 189);");
}

/**
 * slot for clicking rotate left button
 */
void Controller::on_rotateLeftButton_clicked()
{
    emit rotateLeftButtonPressed();
}

/**
 * slot for clicking rotate right button
 */
void Controller::on_rotateRightButton_clicked()
{
    emit rotateRightButtonPressed();
}

/**
 * trigger for selecting pen in tools menu
 */
void Controller::on_actionPen_Tool_triggered()
{
    spriteModel->setPenMode(brush);
    unhighlightMenuButtons();
    ui->penButton->setStyleSheet("background-color:rgb(189, 189, 189);");
}

/**
 * trigger for selecting erase in tools menu
 */
void Controller::on_actionErase_Tool_triggered()
{
    spriteModel->setPenMode(eraser);
    unhighlightMenuButtons();
    ui->eraseButton->setStyleSheet("background-color:rgb(189, 189, 189);");
}

/**
 * trigger for selecting eyedropper in tools menu
 */
void Controller::on_actionEyedropper_Tool_triggered()
{
    spriteModel->setPenMode(eyedropper);
    unhighlightMenuButtons();
    ui->eyeDropperButton->setStyleSheet("background-color:rgb(189, 189, 189);");
}

/**
 * trigger for selecting rotate left in tools menu
 */
void Controller::on_actionRotate_Left_triggered()
{
    emit rotateLeftButtonPressed();
}

/**
 * trigger for selecting rotate right in tools menu
 */
void Controller::on_actionRotate_Right_triggered()
{
    emit rotateRightButtonPressed();
}

/**
 * trigger for selecting export to gif in file menu
 */
void Controller::on_actionExport_to_Gif_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File as GIF"),
                               "./",
                               tr("GIF (*.gif)"));
    emit exportToGifSignal(fileName.toStdString());
}

/**
 * trigger for selecting save in file menu
 */
void Controller::on_actionSave_SpriteSheet_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File as Sprite"),
                               "./",
                               tr("Sprite (*.ssp)"));
    emit saveSpriteSheetSignal(fileName.toStdString());
}

/**
 * trigger for selecting load in file menu
 */
void Controller::on_actionLoad_SpriteSheet_triggered()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                               "./",
                               tr("Sprite (*.ssp)"));
    emit loadSpriteSheetSignal(fileName.toStdString());
}

/**
 * tells the animation to start or stop if the play/pause button is pressed
 */
void Controller::on_animationButton_clicked()
{
    if (spriteModel->getPlayStatus())
    {
        startStopAnimation(false);
        spriteModel->setPlayStatus(false);
        ui->animationButton->setText("Play");
    }
    else
    {
        startStopAnimation(true);
        spriteModel->setPlayStatus(true);
        ui->animationButton->setText("Stop");
    }
}

/**
 * trigger for selecting new frame in tools menu
 */
void Controller::on_actionNew_Frame_triggered()
{
    int newMax = ui->currentFrameSpinBox->maximum() + 1;
    ui->currentFrameSpinBox->setMaximum(newMax);
    emit createNewFrameSignal();
    ui->currentFrameSpinBox->setValue(newMax);
}

/**
 * slot for selecting fps size
 */
void Controller::on_horizontalSlider_valueChanged(int value)
{
    ui->fpsVal->setText(QString::fromStdString(std::to_string(value)));
    emit updateFPSSignal(value);
}

/**
 * slot for selecting current frame
 */
void Controller::on_currentFrameSpinBox_valueChanged(int arg1)
{
    emit changeFrameSignal(arg1);
}

/**
 * slot for deleting current frame
 */
void Controller::on_actionDelete_Current_Frame_triggered()
{
    int newMax = ui->currentFrameSpinBox->maximum() - 1;
    if(newMax>-1){
        emit deleteFrame(ui->currentFrameSpinBox->value());
        ui->currentFrameSpinBox->setMaximum(newMax);
        spriteModel->clearUndoStack(spriteModel->frames[ui->currentFrameSpinBox->value()]);
    }

}

/**
 * slot to open preview animation window
 */
void Controller::on_actionPreview_GIF_triggered()
{
    this->preview.setSpriteModel(spriteModel);
    preview.show();
}

/**
 * slot for clicking undo button
 */
void Controller::on_undo_clicked()
{
    emit undoButtonClicked();
}

/**
 * slot for adding frame
 */
void Controller::on_addFrame_pressed()
{
    int newMax = ui->currentFrameSpinBox->maximum() + 1;
    ui->currentFrameSpinBox->setMaximum(newMax);
    emit createNewFrameSignal();
    ui->currentFrameSpinBox->setValue(newMax);
}

void Controller::on_actionNew_SpriteSheet_triggered()
{
    emit newSpriteSheetSignal();
}

void Controller::updateSpinSlot(){
 ui->currentFrameSpinBox->setValue(0);
 ui->currentFrameSpinBox->setMaximum(0);
}

void Controller::setSpinMaxSlot(int newMax){
 ui->currentFrameSpinBox->setMaximum(newMax);
}

