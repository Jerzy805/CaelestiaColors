
#include "main_window.hpp"

#include <QLabel>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("CaelestiaColours");
    resize(1100, 750);

    auto* placeholder = new QLabel("CaelestiaColours", this);
    placeholder->setAlignment(Qt::AlignCenter);

    setCentralWidget(placeholder);
}
