#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>

class Canvas;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *event) override;

private slots:
    void open();
    void save();
    void brushColor();
    void brushSize();
    void about();

private:
    void createActions();
    void createMenus();
    bool promptSave();
    bool saveFile(const QByteArray &fileFormat);
    Canvas *canvas;
    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;
    QAction *openAction;
    QList<QAction *> saveAsActions;
    QAction *brushColorAction;
    QAction *brushSizeAction;
    QAction *clearScreenAction;
    QAction *aboutAction;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
