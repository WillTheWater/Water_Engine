#include "EntryPoint.h"
#include "Framework/Application.h"

int main()
{
	we::Application* App = GetApplication();
	App->Run();

	delete App;
}