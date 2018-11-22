#include "Scene.h"
#include <string>

int threadedLoader(LoadType load, std::string file_name)
{
	return 1;
}

int WINAPI WinMain(__in HINSTANCE h_instance, __in_opt HINSTANCE h_prev_instance, __in LPSTR lp_cmd_line, __in int n_show_cmd)
{
	srand(time(NULL));
	Scene app;

	if (!app.init(h_instance, n_show_cmd, &threadedLoader))
	{
		return 1;
	}
	int return_int = app.run(n_show_cmd);
	return return_int;
}
