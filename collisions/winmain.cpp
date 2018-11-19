#include "DXApp.h"
#include <thread>
#include <string>

int threadedLoader(LoadType load, std::string file_name)
{
	return 1;
}

int WINAPI WinMain(__in HINSTANCE h_instance, __in_opt HINSTANCE h_prev_instance, __in LPSTR lp_cmd_line, __in int n_show_cmd)
{
	std::thread thread(threadedLoader, LoadType(), std::string());
	srand(time(NULL));
	DXApp app;

	if (!app.init(h_instance, n_show_cmd, &threadedLoader))
	{
		return 1;
	}
	int return_int = app.run(n_show_cmd);
	thread.detach();
	return return_int;
}
