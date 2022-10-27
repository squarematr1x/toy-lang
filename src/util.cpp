#include "util.h"

#if defined _WIN32
	#include <windows.h>
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
	#include <stdio.h>
#endif

namespace util {

void clear() {
#if defined _WIN32
	COORD top_left = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, top_left, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, top_left, &written
	);
	SetConsoleCursorPosition(console, top_left);
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
	printf("\033c");
#endif
}

} // util
