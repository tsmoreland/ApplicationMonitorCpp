#include <iostream>
#include <filesystem>

#include "EnvironmentService.h"

using std::cout;
using std::endl;
using Shared::Services::EnvironmentService;

int main() {
    try {
        EnvironmentService const environment{};

        if (!environment.SetVariable("Hello", "World"))
            cout << "Failed to set environment variable" << endl;

        auto const process = environment.StartProcess(R"(c:\windows\system32\cmd.exe)", "/c sleep 5");
        if (!process.has_value())
            return -1;
        process.value()->WaitForExit();

        auto const envVar = environment.GetVariable("Hello");
        cout << envVar.value_or("no value found") << endl;

        auto processes = environment.GetProcessesByName("svchost.exe");
        for (auto const& p : processes)
            cout << "Process ID: " << p->GetId() << endl;
    }
    catch (std::exception const& e) {
        cout << "Unexpected error occured: " << e.what() << endl;
    }
}
