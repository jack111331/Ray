//
// Created by Edge on 2021/7/8.
//

#ifndef RAY_APPLICATION_H
#define RAY_APPLICATION_H

#include <wx/wxprec.h>
#include <wx/string.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class MyApp: public wxApp
{
public:
    virtual bool OnInit();
    void OnIdle(wxIdleEvent& evt);
};

#endif //RAY_APPLICATION_H
