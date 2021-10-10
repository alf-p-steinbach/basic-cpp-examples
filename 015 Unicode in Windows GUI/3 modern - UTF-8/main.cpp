#include "windows_h.for-utf8.hpp"
using Utf8_c_string = const char*;

auto startup() -> int
{
    const UINT as_infobox = MB_ICONINFORMATION | MB_SETFOREGROUND;
    const Utf8_c_string text = "Every 日本国 кошка likes Norwegian blåbærsyltetøy.";

    MessageBox( 0, text, "Did you know?", as_infobox );
    return 0;
}
