#include <iostream>
#include <filesystem>

#include "shared/environment_repository.h"
#include "shared/process_service.h"

using std::cout;
using std::endl;

using shared::infrastructure::make_shared_const_environment_repository;
using shared::service::make_process_service;

int main()
{
    try {
        auto const environment = make_shared_const_environment_repository();
        auto const processService = make_process_service();

        if (!environment->set_variable("Hello", "World"))
            cout << "Failed to set environment variable" << endl;

        auto const process = processService->start_process(R"(c:\windows\system32\cmd.exe)", "/c sleep 5");
        if (process == nullptr)
            return -1;
        process->wait_for_exit();

        auto const envVar = environment->get_variable("Hello");
        cout << envVar.value_or("no value found") << endl;

        auto processes = processService->get_processes_by_name("svchost.exe");
        for (auto const& p : processes)
            cout << "Process ID: " << p->get_id() << endl;
    }
    catch (std::exception const& e) {
        cout << "Unexpected error occured: " << e.what() << endl;
    }
}
