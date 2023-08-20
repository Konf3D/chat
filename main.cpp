#include <wx/wx.h>
#include "declarewindows.h"

class MyApp : public wxApp
{
public:
    virtual bool OnInit()
    {
        InitialWindow* frame = new InitialWindow("Welcome", wxSize(250, 150));
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
