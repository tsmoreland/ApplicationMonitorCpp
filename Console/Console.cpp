#include <iostream>
#include <filesystem>
#include "Process.h"

using std::cout;
using std::endl;

int main()
{
    try
    {
        const auto process = System::Process::Start("gflags.exe", "/i Console.exe +ust");
        process->WaitForExit();
    }
    catch (std::exception &e)
    {
        cout << "Unexpected error occured: " << e.what() << endl;
    }
}

