//
// Created by Edge on 2021/7/9.
//
#include <wx/splitter.h>
#include <glad/glad.h>
#include <Context.h>
#include "wx/wx.h"
#include "MainFrame.h"

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
                EVT_MENU(ID_Hello, MyFrame::OnHello)
                EVT_MENU(MENU_Exit, MyFrame::OnExit)
                EVT_MENU(MENU_About, MyFrame::OnAbout)
                EVT_MENU(MENU_Open, MyFrame::OpenFile)
                EVT_BUTTON ( BUTTON_Render, MyFrame::OnRender )
wxEND_EVENT_TABLE()

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
        : wxFrame(NULL, wxID_ANY, title, pos, size) {
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(MENU_Open, "&Open",
                     "Open config file");
    menuFile->AppendSeparator();
    menuFile->Append(MENU_Exit, "&Exit", "Exit program");

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(MENU_About, "About");
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

    m_render = new wxButton(this, BUTTON_Render, _T("Render"),
            // shows a button on this window
                              wxDefaultPosition, wxDefaultSize, 0);

    wxGLAttributes vAttrs;
    vAttrs.PlatformDefaults().Defaults().EndList();
    wxSplitterWindow *splitter = new wxSplitterWindow(this);
    m_3DView = new GLPanel(splitter, vAttrs);

    m_log = new wxTextCtrl(splitter, wxID_ANY, wxString(), wxDefaultPosition,
                           wxDefaultSize, wxTE_MULTILINE);
    m_log->SetMinSize(wxSize(-1, 100));
    m_logOld = wxLog::SetActiveTarget(new wxLogTextCtrl(m_log));
    wxLogMessage("This is the log window");


    splitter->SetSize(GetClientSize());
    splitter->SetSashGravity(0.5);
    splitter->SplitVertically(m_log, m_3DView);

}

MyFrame::~MyFrame() {
    delete wxLog::SetActiveTarget(m_logOld);
}

void MyFrame::OnExit(wxCommandEvent &event) {
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent &event) {
    wxMessageBox("This is a wxWidgets' Hello world sample",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnHello(wxCommandEvent &event) {
    wxLogMessage("Hello world from wxWidgets!");
}

void MyFrame::OnRender(wxCommandEvent &event) {
    const wxSize ClientSize = m_3DView->GetClientSize();
    glViewport(0, 0, ClientSize.x, ClientSize.y);
    Pipeline *pipeline = Context::getInstance()->getPipeline();
    if (pipeline) {
        pipeline->m_camera->m_width = ClientSize.x;
        pipeline->m_camera->m_height = ClientSize.y;
    }
    wxLogMessage("Get pipeline.");
    if (pipeline) {
        pipeline->pipelineRender();
        wxLogMessage("Rendered.");
    }
    m_3DView->updated();
}

void MyFrame::OpenFile(wxCommandEvent & WXUNUSED(event)) {
    wxFileDialog *OpenDialog = new wxFileDialog(
            this, _("Choose a file to open"), wxEmptyString, wxEmptyString,
            _("YAML files (*.yml)|*.yml"),
            wxFD_OPEN, wxDefaultPosition);

    // Creates a "open file" dialog with 4 file types
    if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "cancel"
    {
        Context *context = Context::getInstance();
        std::string filepath(OpenDialog->GetPath());
        context->loadConfig(filepath.c_str());
        // Sets our current document to the file the user selected
        // Set the Title to reflect the  file open
//        SetTitle(wxString("Edit - ") << OpenDialog->GetFilename());
    }
}
