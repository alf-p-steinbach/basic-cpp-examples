#include <windows.h>
extern auto startup() -> int;
auto WINAPI WinMain( HINSTANCE, HINSTANCE, char*, int ) -> int { return startup(); }
