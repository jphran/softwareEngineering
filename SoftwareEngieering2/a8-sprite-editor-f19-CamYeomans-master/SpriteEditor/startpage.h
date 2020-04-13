#ifndef STARTPAGE_H
#define STARTPAGE_H

#include <QDialog>
#include <QFileDialog>
#include "string"

namespace Ui {
class StartPage;
}

class StartPage : public QDialog
{
    Q_OBJECT

public:
    explicit StartPage(QWidget *parent = nullptr);
    ~StartPage();

signals:
    void createNewClicked();
    void learnClicked();
    void cancelClicked();
    void loadDudeSignal(std::string loadableDude);

private slots:

    void on_LearnButton_released();

    void on_CreateNewButton_released();

    void on_OpenFileButton_released();

    void on_closeButton_released();

signals:
    void loadSpriteSheetSignal(std::string fileName);

private:
    Ui::StartPage *ui;
};

#endif // STARTPAGE_H
