#include <iostream>
#include <string>
using   std::cout, std::endl,
        std::string;

auto main() -> int
{
    const   string  escape  ="\x1B";
    const   string  csi     = escape + "["; // CSI = “Control Sequence Introducer”
    const   string  reset   = csi + "0m";
    const   string  red     = csi + "31m";

    cout << "This is some " << red << "colored text" << reset << "." << endl;
}
