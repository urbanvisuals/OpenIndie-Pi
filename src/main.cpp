#include "ofMain.h"
#include "testApp.h"
//#include "ofGLProgrammableRenderer.h"

int main()
{
	//ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	ofSetupOpenGL(1920, 1080, OF_WINDOW);
	ofRunApp( new testApp());
}
