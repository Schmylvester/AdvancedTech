#include "Application.h"
#include "Debug.h"

int WINAPI WinMain(__in HINSTANCE h_instance, __in_opt HINSTANCE h_prev_instance, __in LPSTR lp_cmd_line, __in int n_show_cmd)
{
	Debug debugger;
	Application app(h_instance, &debugger);

	if (!app.init())
	{
		return 1;
	}
	return app.run();
}
