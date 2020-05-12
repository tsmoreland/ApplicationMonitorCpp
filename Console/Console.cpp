#include <iostream>
#include <filesystem>

#include "environment_repository_impl.h"
#include "process_service_impl.h"

using std::cout;
using std::endl;
using shared::infrastructure::environment_repository_impl;
using shared::service::process_service_impl;

int main()
{
    try {
        environment_repository_impl const environment{};
        process_service_impl const processService{};

        if (!environment.set_variable("Hello", "World"))
            cout << "Failed to set environment variable" << endl;

        auto const process = processService.start_process(R"(c:\windows\system32\cmd.exe)", "/c sleep 5");
        if (process == nullptr)
            return -1;
        process->wait_for_exit();

        auto const envVar = environment.get_variable("Hello");
        cout << envVar.value_or("no value found") << endl;

        auto processes = processService.get_processes_by_name("svchost.exe");
        for (auto const& p : processes)
            cout << "Process ID: " << p->get_id() << endl;
    }
    catch (std::exception const& e) {
        cout << "Unexpected error occured: " << e.what() << endl;
    }
}
