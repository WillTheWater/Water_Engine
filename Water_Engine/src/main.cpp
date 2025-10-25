#include <memory>
#include "Framework/Application.h"

int main()
{
    std::unique_ptr<we::Application> App{ std::make_unique<we::Application>() };
    App->Run();
}