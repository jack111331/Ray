//
// Created by Edge on 2021/7/9.
//
#include "glad/glad.h"
#include "GLPanel.h"
#include <wx/wx.h>
#include <Config.h>
#include <Context.h>

wxBEGIN_EVENT_TABLE(GLPanel, wxGLCanvas)
                EVT_SIZE(GLPanel::OnSize)
                EVT_PAINT(GLPanel::OnPaint)
                EVT_MOTION(GLPanel::OnMouseMove)
                EVT_LEFT_DOWN(GLPanel::OnLeftMouseDown)
                EVT_LEFT_UP(GLPanel::OnLeftMouseUp)
wxEND_EVENT_TABLE()

GLPanel::GLPanel(wxWindow *parent, const wxGLAttributes &canvasAttrs) : wxGLCanvas(parent, canvasAttrs),
                                                                        m_winHeight(0) {
    // GUI setup
    SetWindowStyle(wxBORDER_SUNKEN);

    //SetColour("gray");
    wxGLContextAttrs ctxAttrs;

    ctxAttrs.PlatformDefaults().CoreProfile().OGLVersion(4, 5).EndList();
    m_GLContext = new wxGLContext(this, NULL, &ctxAttrs);
    if (!m_GLContext->IsOK()) {
        wxLogFatalError("This sample needs an OpenGL 3.2 capable driver.\nThe app will end now.");
        delete m_GLContext;
        m_GLContext = NULL;
    } else {
        wxLogDebug("OpenGL Core Profile 3.2 successfully set.");
    }

    SetCurrent(*m_GLContext);

    gladLoadGL();


}

GLPanel::~GLPanel() {
    if (m_GLContext)
        SetCurrent(*m_GLContext);

    if (m_GLContext) {
        delete m_GLContext;
        m_GLContext = NULL;
    }
}

void GLPanel::OnSize(wxSizeEvent &event) {
    event.Skip();

    const wxSize size = event.GetSize() * GetContentScaleFactor();
    m_winHeight = size.y;

    //OpenGLContext::Instance().resize(size.x, size.y);
    glViewport(0, 0, size.x, size.y);
    Pipeline *pipeline = Context::getInstance()->getPipeline();
    if (pipeline) {
        pipeline->m_camera->m_width = size.x;
        pipeline->m_camera->m_height = size.y;
    }

    //if (!IsShownOnScreen())
    //	return;

//    Refresh(false);
}

void GLPanel::OnPaint(wxPaintEvent & WXUNUSED(event)) {
    // This is a dummy, to avoid an endless succession of paint messages.
    // OnPaint handlers must always create a wxPaintDC.
    wxPaintDC dc(this);

    // Avoid painting when we have not yet a size
    if (m_winHeight < 1)
        return;

    // This should not be needed, while we have only one canvas
    SetCurrent(*m_GLContext);

    Pipeline *pipeline = Context::getInstance()->getPipeline();
    wxLogMessage("Get pipeline.");
    if (pipeline) {
        pipeline->pipelineRender();
        wxLogMessage("Rendered.");
    }

// Refresh and update will invoke this

//    OpenGLContext::Instance().render(m_model->get_world());

    SwapBuffers();
    //wxLogDebug("buffers swapped");
}

void GLPanel::OnMouseMove(wxMouseEvent &event) {
    wxPoint pos = event.GetPosition();
    wxClientDC dc(this);

    int x = dc.DeviceToLogicalX(pos.x);
    int y = dc.DeviceToLogicalY(pos.y);
}

void GLPanel::OnLeftMouseDown(wxMouseEvent &event) {
    wxPoint pos = event.GetPosition();
    wxClientDC dc(this);

    if (!this->HasCapture())
        CaptureMouse();

    long x = dc.DeviceToLogicalX(pos.x);
    long y = dc.DeviceToLogicalY(pos.y);
    double ux, uy, uz;

//    wxLogDebug("xy: %d, %d. uxy: %f, %f, %f", x, y, ux, uy, uz);
}

void GLPanel::OnLeftMouseUp(wxMouseEvent &event) {
    wxPoint pos = event.GetPosition();

    if (this->HasCapture())
        ReleaseMouse();

    {
        wxClientDC dc(this);

        long x = dc.DeviceToLogicalX(pos.x);
        long y = dc.DeviceToLogicalY(pos.y);

    } // use xcurly braces or reset will fail assert
}

void GLPanel::updated() {
    Refresh();
    Update();
}