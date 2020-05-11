#include <iostream>
#include <filesystem>

#include "EnvironmentRepositoryImpl.h"
#include "ProcessServiceImpl.h"

using std::cout;
using std::endl;
using Shared::Infrastructure::EnvironmentRepository;
using Shared::Service::ProcessService;

int main() {
    try {
        EnvironmentRepository const environment{};
        ProcessService const processService{};

        if (!environment.SetVariable("Hello", "World"))
            cout << "Failed to set environment variable" << endl;

        auto const process = processService.StartProcess(R"(c:\windows\system32\cmd.exe)", "/c sleep 5");
        if (!process.HasValue())
            return -1;
        process->WaitForExit();

        auto const envVar = environment.GetVariable("Hello");
        cout << envVar.value_or("no value found") << endl;

        auto processes = processService.GetProcessesByName("svchost.exe");
        for (auto const& p : processes)
            cout << "Process ID: " << p->GetId() << endl;
    }
    catch (std::exception const& e) {
        cout << "Unexpected error occured: " << e.what() << endl;
    }
}
