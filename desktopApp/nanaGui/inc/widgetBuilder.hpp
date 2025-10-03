#ifndef __WIDGET_BUILDER__
#define __WIDGET_BUILDER__

#include <nlohmann/json.hpp>
#include <nana/gui.hpp>
#include <nana/gui/widgets/widget.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

class WidgetBuilder
{
    private:
    nana::form* fm;
    std::ifstream* file;
    std::string filename;
    nlohmann::json jData;
    //std::vector<nana::widget_object, 

    public:
    WidgetBuilder(nana::form* fm, std::string filename);
    void build();

};

#endif