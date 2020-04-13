#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QMainWindow>
#include <QColorDialog>
#include "model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class View; }
QT_END_NAMESPACE

class View : public QMainWindow
{
    Q_OBJECT

public:
    View(QWidget *parent = nullptr);
    ~View();

private:
    Ui::View *ui;
    model::model *spriteModel;
};
#endif // CONTROLLER_H
