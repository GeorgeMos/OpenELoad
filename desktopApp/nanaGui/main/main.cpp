#include <nana/gui.hpp>
#include <../../cpp_libs/inc/usbCom.hpp>
int main()
{
	//Brings all nana names into main scope.
	//using namespace nana;

	//Create a form.
	nana::form fm;
    
	//Draws text "Hello World!" on the form.
	nana::drawing{fm}.draw([](nana::paint::graphics& graph){
		graph.string({ 10, 20 }, "Hello World!", nana::colors::black);
	});

	//Shows the form
	fm.show();

	//where main() passes the control to Nana, and exec() will return
	//if the form is closed.
	nana::exec();
    
   return 0;
}