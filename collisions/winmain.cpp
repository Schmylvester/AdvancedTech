#include "Application.h"

int WINAPI WinMain(__in HINSTANCE h_instance, __in_opt HINSTANCE h_prev_instance, __in LPSTR lp_cmd_line, __in int n_show_cmd)
{
	Application app(h_instance);

	if (!app.init())
	{
		return 1;
	}
	return app.run();
}
