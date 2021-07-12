//
// Created by Edge on 2021/7/9.
//

#ifndef RAY_MAINFRAME_H
#define RAY_MAINFRAME_H

#include "GLPanel.h"

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

    virtual ~MyFrame();

    GLPanel *m_3DView;
    wxTextCtrl *m_log;
    wxLog *m_logOld;
    wxButton *m_render;
private:
    void OnHello(wxCommandEvent &event);

    void OnExit(wxCommandEvent &event);

    void OnAbout(wxCommandEvent &event);

    void OnRender(wxCommandEvent &event);

    void OpenFile(wxCommandEvent &event);

wxDECLARE_EVENT_TABLE();
};

enum {
    // declares an id which will be used to call our button
    ID_Hello = wxID_HIGHEST + 1,
    MENU_Exit,
    MENU_About,
    MENU_Open,
    BUTTON_Render
};


#endif //RAY_MAINFRAME_H
