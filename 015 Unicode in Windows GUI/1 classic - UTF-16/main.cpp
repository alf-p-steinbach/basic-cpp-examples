#include "windows_h.for-utf16.hpp"
using Utf16_c_string = const wchar_t*;

auto startup() -> int
{
    const UINT as_infobox = MB_ICONINFORMATION | MB_SETFOREGROUND;
    const Utf16_c_string text = L"Every 日本国 кошка likes Norwegian blåbærsyltetøy.";

    MessageBox( 0, text, L"Did you know?", as_infobox );
    return 0;
}
