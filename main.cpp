#include "app_singleton.h"  
int main()
{
	app_singleton& app = app_singleton::get_instance();
	app.run();
}