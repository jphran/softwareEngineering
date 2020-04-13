#ifndef MODEL_H
#define MODEL_H
#include <QImage>
#include <QTimer>
#include <QObject>
#include <QColor>
#include <vector>
#include <stack>
#include <QColorDialog>
#include <QObject>
#include <string>
#include <3rdPartyLibs/gif.h> // 3rd party lib to export as gif
#include <3rdPartyLibs/json.hpp> // 3rd party lib for json

class model : public QObject
{
    Q_OBJECT

public:
    model(QObject *parent = nullptr);
    std::vector<QImage> frames;
    void createFirstFrameAndInitalizeValues();
    void setSize(unsigned int x, unsigned int y);
    int fps;
    void removeFrame(int frameIdx);
    void setFPS(int newFps);
    void setColor(QColor newColor);
    void changeFrame(unsigned long newFrame);
    void setPenMode(int newMode);
    unsigned long currentWorkingFrameIdx; // This is the index of the frame being drawn on, starting from zero
    int brushOrEraserRadius;
    void useTool(double centerX, double centerY);
    void clearUndoStack(QImage frame);
    std::stack<QImage> undoStack;
    int convertFramesPerSecondToMillisecondsBetweenFrames(int spriteFps);


public slots:
    void addFrame();
    void changeFrameSlot(int newFrame);
    void saveAsJsonSlot(std::string saveFileName);
    void loadFromJsonSlot(std::string fileName);
    void exportAsGifSlot(std::string fileName);
    void removeFrameSlot(int frameIdx);
    void rotateFrameLeft();
    void rotateFrameRight();
    void showHelp();
    void selectBrush();
    void selectErase();
    void selectDropper();
    void selectColorPicker();
    void getSize(unsigned int x, unsigned int y);
    bool getPlayStatus();
    void setPlayStatus(bool status);
    void setFPSSlot(int newFps);
    void undoEdit();
    void newSpriteSheetSlot();


private:
    int penMode;
    bool playStatus;
    QColor selectedColor;
    unsigned int frameSize_x;
    unsigned int frameSize_y;
    unsigned int toColor_x;
    unsigned int toColor_y;
    unsigned int pixelBlockSize;
    QColor transparentPixel; // TODO make static?
    std::string addFileExtensionIfNeeded(std::string);
    void paintPixels(double centerX, double centerY, QColor paintColor);
    void eyeDropperPickColor(double centerX, double centerY);
    bool mouseNotInDrawingArea(double x, double y);

signals:
    void updateSpinSignal();
    void updateSpinMaxSignal(int newMax);
    void updateSpriteDisplay();
    void resizeSpriteFrame(unsigned int x, unsigned int y);
    void changeColorBox(QColor selectedColor);
};



#endif // MODEL_H
