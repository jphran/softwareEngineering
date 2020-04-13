#include "preview.h"
#include "ui_preview.h"
#include "model.h"

Preview::Preview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preview)
{
    ui->setupUi(this);
    this->ui->previewArea->setStyleSheet("background-color: rgba(255, 255, 255, 85);");
}

Preview::~Preview()
{
    delete ui;
}

/**
 * method allows for the preview area to be changed in size
 */
void Preview::resizePreviewArea(unsigned int x, unsigned int y)
{
    QSize size(x, y);
    ui->previewArea->setFixedSize(size);
}

/**
 * this method is made to set the sprite model as having certain properties
 */
void Preview::setSpriteModel(model* _spriteModel)
{
    spriteModel = _spriteModel;

    this->animationPlayBackCurrentFrame = 0;
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, QOverload<>::of(&Preview::loopOverFramesForAnimation));

    int interval = spriteModel->convertFramesPerSecondToMillisecondsBetweenFrames(this->spriteModel->fps);
    animationTimer->start(interval);
}

/**
 * This method goes over each frame in the display frame and plays it back
 */
void Preview::loopOverFramesForAnimation()
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


    this->ui->previewArea->setPixmap(QPixmap::fromImage(displayFrame));
}
