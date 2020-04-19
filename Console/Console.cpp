#include <iostream>
#include <filesystem>
#include "Process.h"

using std::cout;
using std::endl;
using std::unique_ptr;
using Shared::Domain::Process;
using Shared::Domain::IProcess;

int main()
{
    try
    {
        
        const unique_ptr<IProcess> process = Process::Start(R"(c:\windows\system32\cmd.exe)", "/c sleep 5");
        process->WaitForExit();
    }
    catch (std::exception &e)
    {
        cout << "Unexpected error occured: " << e.what() << endl;
    }
}

