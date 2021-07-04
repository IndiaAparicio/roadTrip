#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	//ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context


	//making shaders visible
#ifdef OF_TARGET_OPENGLES
	ofGLESWindowSettings settings;
	settings.glesVersion = 2;
#else
	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
#endif
	ofCreateWindow(settings);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
