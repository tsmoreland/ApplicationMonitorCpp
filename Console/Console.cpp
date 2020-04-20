#include <iostream>
#include <filesystem>

#include "EnvironmentService.h"

using std::cout;
using std::endl;
using Shared::Services::EnvironmentService;

int main()
{
    try
    {
        EnvironmentService const environment{};

        auto const process = environment.StartProcess(R"(c:\windows\system32\cmd.exe)", "/c sleep 5");
        if (!process.has_value())
            return -1;
        process.value()->WaitForExit();

        auto processes = environment.GetProcessesByName("svchost.exe");
        for (auto const& p : processes)
            cout << "Process ID: " << p->GetId() << endl;
    }
    catch (std::exception &e)
    {
        cout << "Unexpected error occured: " << e.what() << endl;
    }
}
