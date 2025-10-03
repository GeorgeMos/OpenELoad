#include <widgetBuilder.hpp>

WidgetBuilder::WidgetBuilder(nana::form* fm, std::string filename)
{
    this->file = new std::ifstream(filename);
}

void WidgetBuilder::build()
{
    this->jData = nlohmann::json::parse(*(this->file));
    
}