#ifndef __MAIN_WINDOW__
#define __MAIN_WINDOW__

#include <nana/gui.hpp>
//#include <nana/gui/wvl.hpp> 
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp> 
#include <widgetBuilder.hpp>
#include <string>


class MainWindow
{
    private:
    nana::form mainForm;
    int sizeX;
    int sizeY;
    WidgetBuilder* menuBuilder;

    //GUI Object declaration
    nana::label* SelectModeLabel;

    public: 
    MainWindow(int sizeX = 1080, int sizeY = 720);

    void run();


};


#endif