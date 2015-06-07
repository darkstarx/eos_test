#include <Windows.h>
#include <MainWindow.hpp>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	return MainWindow::create_window(hInstance);
}
