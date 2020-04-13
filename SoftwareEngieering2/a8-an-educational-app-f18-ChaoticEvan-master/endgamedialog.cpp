/**
  * This class ends the game.
  * Created by Serena Aeschilman, Spencer Elkington, Andrew Stender, Evan Voordeckers, Ryan Williamson, Keegan Spencer, and Theaux Masqulier.
  */

#include "endgamedialog.h"
#include "ui_endgamedialog.h"

/// Constructor
/// \brief endgamedialog::endgamedialog
/// \param parent
///
endgamedialog::endgamedialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::endgamedialog)
{
    ui->setupUi(this);
}

/// ~Destructor
/// \brief endgamedialog::~endgamedialog
///
endgamedialog::~endgamedialog()
{
    delete ui;
}
