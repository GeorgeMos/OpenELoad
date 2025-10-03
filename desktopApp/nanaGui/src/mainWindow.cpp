#include "mainWindow.hpp"

MainWindow::MainWindow(int sizeX, int sizeY)
{
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    this->mainForm.size(nana::size(this->sizeX, this->sizeY));

    //GUI Objects setup
    this->SelectModeLabel = new nana::label(this->mainForm);
    this->SelectModeLabel->caption("Select Operation Mode");

}

void MainWindow::run()
{
    this->mainForm.show();
    nana::exec();
}