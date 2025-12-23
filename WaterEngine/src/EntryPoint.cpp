#include <Windows.h>
#include "EntryPoint.h"
#include "Framework/Application.h"

int main()
{
#ifdef NDEBUG
	FreeConsole();
#endif
	we::Application* App = GetApplication();
	App->Run();

	delete App;
}