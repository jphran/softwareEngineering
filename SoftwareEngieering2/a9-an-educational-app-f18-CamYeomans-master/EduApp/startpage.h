#ifndef STARTPAGE_H
#define STARTPAGE_H

#include <QWidget>
#include <QPixmap>
#include <QtMultimedia/QSound>
#include <QtMultimedia/QMediaPlayer>
#include <view.h>
#include "Box2D/Box2D.h"

QT_BEGIN_NAMESPACE
namespace Ui { class startPage; }
QT_END_NAMESPACE

class startPage : public QWidget
{
    Q_OBJECT

public:
    explicit startPage(QWidget *parent = nullptr);
    ~startPage();

private:
    Ui::startPage *ui;
    QMediaPlayer *player;
    void playMusic();
    void stopMusic();
    void setupWorld();
    b2World world;
    b2Body* body;

protected:
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_newGameButton_clicked();
    void updateWorld();

    void on_helpButton_clicked();

signals:
    void openView();
    void newHeight(int);
};

#endif // STARTPAGE_H
