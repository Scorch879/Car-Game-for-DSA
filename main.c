#include "menu.h"

void setFullScreen() 
{
    HWND consoleWindow = GetConsoleWindow();
    ShowWindow(consoleWindow, SW_SHOWMAXIMIZED);

    // Send ALT + ENTER key combo to force fullscreen (old-style)
    keybd_event(VK_MENU, 0x38, 0, 0);              // Press ALT
    keybd_event(VK_RETURN, 0x1C, 0, 0);            // Press ENTER
    keybd_event(VK_RETURN, 0x1C, KEYEVENTF_KEYUP, 0); // Release ENTER
    keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);   // Release ALT
}

int main() 
{
	setFullScreen();
    mainMenu();
    return 0;
}
