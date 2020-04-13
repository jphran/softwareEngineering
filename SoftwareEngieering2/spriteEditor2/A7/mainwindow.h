/*
* S.I.M.P
* Team 2
* https://github.com/University-of-Utah-CS3505/a8-sprite-editor-f19-Mattmathics
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <QColorDialog>
#include <QLabel>
#include <QFileDialog>
#include <QScrollArea>
#include <QGridLayout>
#include <QWidget>

#include "canvas.h"
#include "tool.h"
#include "eraser.h"
#include "pencil.h"
#include "bucket.h"
#include "colorpicker.h"
#include "projectmanager.h"
#include "qimagebutton.h"
#include "json.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Canvas *copyCanvas = nullptr, QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    /* VAR */
    Ui::MainWindow *ui;

    QWidget previewWindow;
    QLabel animationPreview;
    QLabel animationCheckerboard;
    Canvas *canvas = nullptr;
    Tool *tool = nullptr;

    QVector<QImageButton*> framePreviews;
    QGridLayout *frameGridLayout;

    int horizontalOffset = 150;
    int pixelSize = 10;

    int animationFrame = 0;
    bool playAnimation = true;

    int lastPointX = 0;
    int lastPointY = 0;
    bool canvasClicked = false;

    QColor brushColor = QColor(0,0,0,255);
    QColor brushSubColor = QColor(255,255,255,255);

    QString projectLocation = "";

    /* METHOD */

    /**
     * Constructs or copies a new canvas, and sets the framePreview and tool to pencil. 
     *
     * @param canvas Should be not nullptr.
     */
    void initialize(Canvas *canvas);

    /**
     * Deinitalize canvas and clears the preview frames.
     */
    void deinitalize();

    /**
     * When canvas is cliked, update pixel with current tool.
     *
     * @param posX Clicked position X
     * @param posY Clicked posision Y
     */
    void canvasPressed(int posX, int posY);

    /**
     * When mouse is moving on canvas after clicked, update pixels with current tool
     * .
     * @param posX Clicked position X
     * @param posY Clicked posision Y
     */
    void canvasMoved(int posX, int posY);

    /**
     * When mouse is released after clicked, update pixel with current tool.
     *
     */
    void canvasReleased();

    /**
     * After performing tool, do some after job.
     *
     * @param pointX Clicked position X
     * @param pointY Clicked posision Y
     */
    void aftrerToolPerform(int pointX, int pointY);

    /**
     * Get background CSS from color.
     *
     * @param color
     * @return
     */
    QString getColorString(QColor color);

    /**
     * @brief updateColorPickerButton
     * @param button
     */
    void updateColorPickerButton(QImageButton* button);

    /**
     * Update primary brush color.
     *
     * @param color
     */
    void primaryBrushColorUpdate(QColor color);

    /**
     * Update secondary brush color.
     *
     * @param color
     */
    void secondaryBrushColorUpdate(QColor color);

    /**
     * Add frame preview widget to preview section.
     */
    void addFramePreview();

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *e);
    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_pencilButton_clicked();
    void on_eraserButton_clicked();
    void on_swapBrushesButton_clicked();
    void on_primaryBrushButton_clicked();
    void on_resetBrushesButton_clicked();
    void on_secondaryBrushButton_clicked();
    void on_findAndReplaceButton_clicked();
    void on_addFrameButton_clicked();
    void on_bucketButton_clicked();
    void on_colorPicker_clicked();
    void on_saveButton_clicked();
    void on_framePreview_clicked();
    void on_openButton_clicked();
    void on_redoButton_clicked();
    void on_undoButton_clicked();
    void on_newProjectButton_clicked();
    void update_animation();
    void on_deleteFrameButton_clicked();
    void on_actualSizeButton_clicked();
    void on_playButton_clicked();
    void on_pauseButton_clicked();
    void on_duplicateFrameButton_clicked();
};
#endif // MAINWINDOW_H
