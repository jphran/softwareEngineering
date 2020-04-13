#include "model.h"
#include <iostream>
#include <fstream>
#include <QDebug>
#include "toolTypesEnum.h"
#include <QMessageBox>

#define DEFAULT_PIXEL_BLOCK_SIZE 16

// 3rd party lib stuff
using json = nlohmann::json;
using namespace gifNamespace;

// TODO merge working branch back to master
// TODO: The open button on the start up screen doesnt work
// TODO: the close button on the startup screen crashes the program. I guess thats one way to close a program


model::model(QObject *parent) : QObject(parent)
{
    pixelBlockSize = DEFAULT_PIXEL_BLOCK_SIZE;

    // Declare a transparent pixel for ease of assignment later
    QColor transparent(0, 0, 0, 0);
    this->transparentPixel = transparent;
}

/**
 * initialize the sprite editor (called upon creating a new spriteSheet)
 */
void model::createFirstFrameAndInitalizeValues()
{
    playStatus = false;
    penMode = brush;
    currentWorkingFrameIdx = 0; // This is the index of the frame being drawn on
    fps = 1;

    // Create first frame empty
    QImage firstFrame(static_cast<int>(this->frameSize_x), static_cast<int>(this->frameSize_y), QImage::Format_ARGB32_Premultiplied);
    firstFrame.fill(this->transparentPixel);
    this->frames.push_back(firstFrame);

    //Put first frame on undoStack
    undoStack.push(firstFrame);

    QColor blackPixel(0, 0, 0, 255);
    this->selectedColor = blackPixel;
    emit updateSpriteDisplay();

    this->brushOrEraserRadius = 10;
    //brushAndEraserRadius();
}


/**
 * takes in the name of the file to be saved and uses the Json library we included to save the current work as an .ssp
 */
void model::saveAsJsonSlot(std::string saveFileName)
{

    // Check if user already enterned file extension (".ssp"). If not, add it
    saveFileName = addFileExtensionIfNeeded(saveFileName);

    json j;
    j["height"] = this->frameSize_y/DEFAULT_PIXEL_BLOCK_SIZE;
    j["width"] = this->frameSize_x/DEFAULT_PIXEL_BLOCK_SIZE;
    j["numberOfFrames"] = this->frames.size();

    // Loop over frames
    for (unsigned int f = 0; f < this->frames.size(); f++)
    {
        QImage currentFrame = this->frames[f];

        unsigned int saveRowIdx = 0;
        // Loop over rows
        for (unsigned int row = 0; row < frameSize_y; row = row + DEFAULT_PIXEL_BLOCK_SIZE)
        {
            unsigned int saveColIdx = 0;
            // Loop over column
            for (unsigned int col = 0; col < frameSize_x; col = col + DEFAULT_PIXEL_BLOCK_SIZE)
            {

                QColor currentPixel = currentFrame.pixelColor(static_cast<int>(col), static_cast<int>(row));
                int R = currentPixel.red();
                int G = currentPixel.green();
                int B = currentPixel.blue();
                int A = currentPixel.alpha();

                int pixelArray[4] = {R, G, B, A};

                j["frames"]["frame" + std::to_string(f)][saveRowIdx][saveColIdx] = pixelArray;
                saveColIdx++;
            }
            saveRowIdx++;
        }
    }

    std::ofstream outputFile;
    outputFile.open(saveFileName);
    outputFile << j; // Write to file
    outputFile.close();
}

/**
 * loads an .ssp file from the computer to the sprite editor using the Json lib
 */
void model::loadFromJsonSlot(std::string loadFileName)
{
    // Check if user already enterned file extension (".ssp"). If not, add it
    loadFileName = addFileExtensionIfNeeded(loadFileName);

    char period = '.';
    std::size_t found = loadFileName.find(period);

    if(found != std::string::npos)
    {
        //then we know it has been found and nothing needs to be added to the end
        return;
    }
    else
    {
        std::ifstream file(loadFileName + ".ssp"); // TODO check if user added ".spp" so we dont have it twice
    }


    // Open file, read contents
    std::ifstream file(loadFileName);
    std::string currentLine;
    std::string fileText = "";

    if (file.is_open())
    {
        while (getline(file, currentLine))
        {
            fileText += currentLine;
        }
        file.close();
    }

    // Save file contents as json and create a sprite
    json j = json::parse(fileText);
    unsigned int inputSize_x = j["width"];
    unsigned int inputSize_y = j["height"];

    //we are testing to make sure that the size of the file to be loaded is a square, otherwise it will tell you can't
    //and it will return from the method, as we don't want to keep going.
    //this theoretically should not happen, however, someone might try to load a non square file from another computer.
    // Also make sure within bounds
    if (inputSize_x != inputSize_y)
    {
        QMessageBox errorMessage;
        errorMessage.setText("You do not have a square frame! Please make a sprite in a square frame.");
        errorMessage.exec();
        return;
    }
    else if(inputSize_x > DEFAULT_PIXEL_BLOCK_SIZE * DEFAULT_PIXEL_BLOCK_SIZE)
    {
        QMessageBox errorMessage;
        errorMessage.setText("Your frame size is too large! Please make a sprite no bigger than 32x32.");
        errorMessage.exec();
        return;
    }

    emit resizeSpriteFrame(inputSize_x, inputSize_y);
    this->frameSize_x = inputSize_x * DEFAULT_PIXEL_BLOCK_SIZE;
    this->frameSize_y = inputSize_y * DEFAULT_PIXEL_BLOCK_SIZE;
    this->frames.clear(); // Due to the initalization, the first frame is empty. We need to remove it
    unsigned long numOfFrames = j["numberOfFrames"];

    for (unsigned int i = 0; i < numOfFrames; i++)
    {
        QImage currentFrame(static_cast<int>(frameSize_x), static_cast<int>(frameSize_y), QImage::Format_ARGB32_Premultiplied);

        for (unsigned int row = 0; row < inputSize_x; row++)
        {
            for (unsigned int col = 0; col < inputSize_y; col++)
            {
                // note: currentFrame = j["frames"]["frame" + std::to_string(i)]
                int R = j["frames"]["frame" + std::to_string(i)][row][col][0];
                int G = j["frames"]["frame" + std::to_string(i)][row][col][1];
                int B = j["frames"]["frame" + std::to_string(i)][row][col][2];
                int A = j["frames"]["frame" + std::to_string(i)][row][col][3];
                QColor currentColor(R, G, B, A);

                // For each pixel loaded in, create a big chunk of same color for displaying
                for (unsigned int a = 0; a < DEFAULT_PIXEL_BLOCK_SIZE; a++)
                {
                    for (unsigned int b = 0; b < DEFAULT_PIXEL_BLOCK_SIZE; b++)
                    {
                        unsigned int offsetIdxA = row * DEFAULT_PIXEL_BLOCK_SIZE + a;
                        unsigned int offsetIdxB = col * DEFAULT_PIXEL_BLOCK_SIZE + b;
                        currentFrame.setPixelColor(static_cast<int>(offsetIdxB), static_cast<int>(offsetIdxA), currentColor);
                    }
                }
            }
        }
        this->frames.push_back(currentFrame);
    }

    this->currentWorkingFrameIdx = 0;
    emit updateSpriteDisplay();
    emit changeColorBox(this->selectedColor);
    emit updateSpinMaxSignal(frames.size()-1);
}

int model::convertFramesPerSecondToMillisecondsBetweenFrames(int spriteFps)
{
    return (1/(double)spriteFps)*1000;
}

/**
 * Uses a .gif library we found to add each frame to the gif file and then save it
 */
void model::exportAsGifSlot(std::string exportFileName)
{
    char period = '.';
    std::size_t found = exportFileName.find(period);

    if(found != std::string::npos)
    {
        //then we know it has been found and nothing needs to be added to the end
        return;
    }
    else
    {
       exportFileName = exportFileName + ".gif"; // check if user already added .gif
    }

    int delay = convertFramesPerSecondToMillisecondsBetweenFrames(this->fps);
    GifWriter g;
    GifBegin(&g, exportFileName.c_str(), frameSize_x, frameSize_y, static_cast<uint32_t>(delay));

    // Add each frame to the gif
    for (unsigned long i = 0; i < frames.size(); i++)
    {
        // Heads up, this 3rd party lib only supports 1 byte pixel values
        // Our 8 byte values need to be converted
        // This is a bit of a pain, but pros of this lib outweigh the cons

        std::vector<uint8_t> vectorFrame(frameSize_x * frameSize_y*4, 255);
        unsigned long vectorIdx = 0;
        QImage currentFrame = frames[i];

        // Loop over rows
        for (unsigned int row = 0; row < frameSize_y; row++)
        {
            // Loop over column
            for (unsigned int col = 0; col < frameSize_x; col++)
            {
                QColor currentPixel = currentFrame.pixelColor(static_cast<int>(col), static_cast<int>(row));
                uint8_t R = static_cast<uint8_t>(currentPixel.red());
                uint8_t G = static_cast<uint8_t>(currentPixel.green());
                uint8_t B = static_cast<uint8_t>(currentPixel.blue());
                uint8_t A = static_cast<uint8_t>(currentPixel.alpha());

                if(A==0){
                    vectorFrame[vectorIdx*4+0] = 255;
                    vectorFrame[vectorIdx*4+1] = 255;
                    vectorFrame[vectorIdx*4+2] = 255;
                    vectorFrame[vectorIdx*4+3] = 0;

                }else{
                    vectorFrame[vectorIdx*4+0] = R;
                    vectorFrame[vectorIdx*4+1] = G;
                    vectorFrame[vectorIdx*4+2] = B;
                    vectorFrame[vectorIdx*4+3] = A;
                }
                vectorIdx++;
            }
        }

        GifWriteFrame(&g, vectorFrame.data(), frameSize_x, frameSize_y, static_cast<uint32_t>(delay));
    }
    GifEnd(&g);
}

void model::setColor(QColor newColor)
{
    this->selectedColor = newColor;
}


void model::setPenMode(int newMode)
{
    this->penMode = newMode;
}

void model::setSize(unsigned int x, unsigned int y)
{
    this->frameSize_x = x*pixelBlockSize;
    this->frameSize_y = y*pixelBlockSize;
}

/**
 * creates a duplicate of the frame and then inserts it after the current frame on the frame list
 */
void model::addFrame()
{
    QImage duplicateFrame = this->frames[this->currentWorkingFrameIdx].copy();
    // Add frame right after current frame
    this->frames.insert(this->frames.begin()+static_cast<long>(this->currentWorkingFrameIdx), duplicateFrame);
    //Clear the undoStack
    this->clearUndoStack(duplicateFrame);
}

void model::showHelp()
{
    // TODO?
}

void model::rotateFrameLeft()
{
    QTransform rotationTransform;
    rotationTransform.rotate(-90); // -90 degrees
    this->frames[currentWorkingFrameIdx] = this->frames[currentWorkingFrameIdx].transformed(rotationTransform);
    emit updateSpriteDisplay();
}

void model::rotateFrameRight()
{
    QTransform rotationTransform;
    rotationTransform.rotate(90); // 90 degrees
    this->frames[currentWorkingFrameIdx] = this->frames[currentWorkingFrameIdx].transformed(rotationTransform);
    emit updateSpriteDisplay();
}

void model::removeFrameSlot(int frameIdx)
{
    this->frames.erase(this->frames.begin()+frameIdx);
    emit updateSpriteDisplay();
}

void model::removeFrame(const int frameIdx)
{
    // erase takes in an iterator. Add offset to iterator to remove specific position
    this->frames.erase(this->frames.begin()+frameIdx);
}

/**
 * a helper method to add a file extension to a file name if it is needed
 */
std::string model::addFileExtensionIfNeeded(std::string inputFileName)
{
    unsigned long fileNameLength = inputFileName.length();
    std::string endOfFileName = inputFileName.substr(fileNameLength - 4, 4);
    if (endOfFileName != ".ssp")
    {
        inputFileName = inputFileName + ".ssp";
    }
    return inputFileName;
}

bool model::mouseNotInDrawingArea(double x, double y)
{
    return x < 0 || y < 0 || x >= this->frameSize_x || y >= this->frameSize_y;
}


/**
 * Driver method. Called whever any tool is used on the sprite
 */
void model::useTool(double x, double y)
{
    // Dont do anything if mouse is outside of drawing area
    if (mouseNotInDrawingArea(x, y))
    {
        return;
    }

    switch(this->penMode)
    {
        case brush :
            this->paintPixels(x, y, this->selectedColor);
            break;

        case eraser :
            paintPixels(x, y, this->transparentPixel);
            break;

        case eyedropper :
            this->eyeDropperPickColor(x, y);
            break;

        default :
            qDebug() << "Error unknown brush selected. Setting tool to paint brush";
            this->paintPixels(x, y, this->selectedColor);
            break;
    }

}


/**
 * Given x and y coords of pixel selected by eyedropper,
 * return the pixel's color
 */
void model::eyeDropperPickColor(double x, double y)
{
    this->selectedColor = this->frames[currentWorkingFrameIdx].pixelColor(static_cast<int>(round(x)), static_cast<int>(round(y)));
    emit changeColorBox(selectedColor);
}


/**
 * This method can be used for the brush or the eraser.
 * For eraser, the paintColor should = transparent
 */
void model::paintPixels(double centerX, double centerY, QColor paintColor)
{
    toColor_x = static_cast<unsigned int>(floor(centerX/pixelBlockSize));
    toColor_y = static_cast<unsigned int>(floor(centerY/pixelBlockSize));

    for(unsigned int i = (toColor_x*pixelBlockSize); i < ((toColor_x+1)*pixelBlockSize); i++)
    {
        for(unsigned int j = (toColor_y*pixelBlockSize); j < ((toColor_y+1)*pixelBlockSize); j++)
        {
            this->frames[currentWorkingFrameIdx].setPixelColor(static_cast<int>(i), static_cast<int>(j), paintColor);
        }
    }
    emit updateSpriteDisplay();
}

void model::setFPS(int newFps)
{
    fps = newFps;
    // TODO: emit updateFPSSlider();
}

void model::changeFrameSlot(int newFrame)
{
    currentWorkingFrameIdx = (unsigned long)(newFrame);
    emit updateSpriteDisplay();
}

void model::changeFrame(unsigned long newFrameIdx)
{
    currentWorkingFrameIdx = newFrameIdx;
    // Clear UndoStack
    this->clearUndoStack(this->frames[currentWorkingFrameIdx]);
    emit updateSpriteDisplay();
}

void model::selectBrush()
{
    penMode = brush;
}

void model::selectErase()
{
    penMode = eraser;
}


void model::selectDropper()
{
    penMode = eyedropper;
}


void model::selectColorPicker()
{
    // TODO: what does this method do?
}

/**
 * slot to get the sprite size from user input
 */
void model::getSize(unsigned int x, unsigned int y)
{
    setSize(x, y);
    this->createFirstFrameAndInitalizeValues();
}

bool model::getPlayStatus()
{
    return playStatus;
}

void model::setPlayStatus(bool status)
{
    playStatus = status;
}


void model::setFPSSlot(int newFps)
{
    this->setFPS(newFps);
}

/**
 * Helper method to clear the undo stack
 */
void model::clearUndoStack(QImage frame){
    while(!this->undoStack.empty()){
        this->undoStack.pop();
    }
    this->undoStack.push(frame);
}

/**
 * small method to undo the last made edit
 */
void model::undoEdit()
{
    if(undoStack.size() > 1)
    {
        this->undoStack.pop();
        this->frames[this->currentWorkingFrameIdx] = this->undoStack.top();
        emit updateSpriteDisplay();
    }
}

void model::newSpriteSheetSlot()
{
    frames.clear();
    playStatus = false;
    penMode = brush;
    currentWorkingFrameIdx = 0; // This is the index of the frame being drawn on
    fps = 1;

    // Create first frame empty
    QImage firstFrame(static_cast<int>(this->frameSize_x), static_cast<int>(this->frameSize_y), QImage::Format_ARGB32_Premultiplied);
    firstFrame.fill(this->transparentPixel);
    this->frames.push_back(firstFrame);

    //Put first frame on undoStack
    undoStack.push(firstFrame);

    QColor blackPixel(0, 0, 0, 255);
    this->selectedColor = blackPixel;
    emit updateSpinSignal();
    emit updateSpriteDisplay();

    this->brushOrEraserRadius = 10;
}

