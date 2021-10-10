#include "windows_h.for-utf8.hpp"   // Essentially just includes <windows.h>
#include <stdlib.h>                 // EXIT_SUCCESS

namespace g {
    const HINSTANCE h_instance = ::GetModuleHandle( nullptr );
}  // namespace g

namespace dialog {
    auto spec()
        -> const DLGTEMPLATE&
    {
        struct alignas(DWORD) Spec
        {
            DLGTEMPLATE     window;
            WORD            menu        = 0;
            WORD            windowclass = 0;
            wchar_t         title[80]   = L"A dialog window!";
            WORD            padding;    // Manual padding avoids a silly-warning.

            Spec(): window()
            {
                window.style = DS_CENTER | WS_OVERLAPPEDWINDOW;
                window.cx = 400;  window.cy = 250;  // Width & height in dialog units.
            }
        };
        
        static const Spec the_spec;
        return reinterpret_cast<const DLGTEMPLATE&>( the_spec );    // E.g. C++20 §11.4/20.
    }
    
    auto WINAPI event_callback( const HWND window, const UINT msg, WPARAM, LPARAM )
        -> INT_PTR
    {
        switch( msg ) {
            case WM_CLOSE:  {
                EndDialog( window, IDOK );
                return true;
            }
        }
        return false;
    }
}  // namespace dialog

auto startup() -> int
{
    ::DialogBoxIndirect( g::h_instance, &dialog::spec(), HWND(), &dialog::event_callback );
    return EXIT_SUCCESS;
}
