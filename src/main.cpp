#include <iostream>

#include <Pipeline.h>
#include "Config.h"

using namespace std;

#include "Application.h"
#include "MainFrame.h"
wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame( "Hello World", wxPoint(50, 50), wxSize(800, 600) );
    frame->Show( true );
    Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MyApp::OnIdle));
    return true;
}

void MyApp::OnIdle(wxIdleEvent& evt) {
        evt.RequestMore(); // render continuously, not only once on idle
}

//int main(int argc, char **argv) {
//    Config *config = new Config();
//    Pipeline *pipeline = nullptr;
//    if (argc > 1) {
//        cout << "Reading config: " << argv[1] << endl;
//        pipeline = config->loadConfig(argv[1]);
//    } else {
//        cout << "[ERROR] No valid argument" << endl;
//        exit(1);
//    }
//    if (!pipeline) {
//        cout << "[ERROR] Create pipeline failed" << endl;
//        exit(1);
//    }
//
//    // Environment preparation
//    pipeline->setupEnvironment();
//    pipeline->setupPipeline();
//
//    pipeline->pipelineLoop();
//
//    return 0;
//}
