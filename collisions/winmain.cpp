#include "DXApp.h"

int WINAPI WinMain(__in HINSTANCE h_instance, __in_opt HINSTANCE h_prev_instance, __in LPSTR lp_cmd_line, __in int n_show_cmd)
{
	srand(time(NULL));
	DXApp app;

	if (!app.init(h_instance, n_show_cmd))
	{
		return 1;
	}
	return app.run(n_show_cmd);
}
