#include "pch.h"

int main(int argc, char* argv[])
{
    return 0;
}

#if (!defined(_DEBUG) && defined(_WIN32))
extern int __argc;
extern char** __argv;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdLine, int showCmd)
{
    return main(__argc, __argv);
}
#endif // !defined(_DEBUG) && defined(_WIN32)
