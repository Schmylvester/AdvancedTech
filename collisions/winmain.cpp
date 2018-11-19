#include "DXApp.h"
#include <thread>

int function()
{
	return 1;
}

int WINAPI WinMain(__in HINSTANCE h_instance, __in_opt HINSTANCE h_prev_instance, __in LPSTR lp_cmd_line, __in int n_show_cmd)
{
	std::thread thread(function);
	srand(time(NULL));
	DXApp app;

	if (!app.init(h_instance, n_show_cmd, &function))
	{
		return 1;
	}
	app.run(n_show_cmd);
	return 0;
}
