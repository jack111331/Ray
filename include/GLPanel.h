//
// Created by Edge on 2021/7/9.
//

#ifndef RAY_GLPANEL_H
#define RAY_GLPANEL_H

#include <wx/glcanvas.h>
#include "Config.h"
#include "Pipeline.h"

class GLPanel : public wxGLCanvas {
public:
    GLPanel(wxWindow *parent, const wxGLAttributes &canvasAttrs);

    ~GLPanel();

public:
    void OnSize(wxSizeEvent &event);

    void OnPaint(wxPaintEvent &event);

    void OnMouseMove(wxMouseEvent &event);

    void OnLeftMouseDown(wxMouseEvent &event);

    void OnLeftMouseUp(wxMouseEvent &event);

    void updated();

private:
    wxGLContext *m_GLContext;

    int m_winHeight;

    bool m_isLoaded = false;

wxDECLARE_EVENT_TABLE();
};

#endif //RAY_GLPANEL_H
