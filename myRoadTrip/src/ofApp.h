#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);


		//shader
		ofShader shader;
		ofShader shaderBg;
		ofShader shaderPlanets;
		ofShader shaderSun;
		ofShader shaderTerrain;


		// mesh object (terrain)
		ofMesh mainMesh;
		ofMesh secondMesh;
		ofEasyCam mainCam;
		float rotatePlanets;
		ofVec3f sunPosition;
		float stars[1000];
		

		int width, height;
		bool b_messyMesh, b_perlinMesh, b_drawWireFrame, b_marchMesh;
		float perlinRange, perlinHeight, perlinHeight_2;

		//gui
		ofxPanel gui; //panel where all elements are inside
		ofxIntSlider gui_circleResolution; 
		ofxFloatSlider gui_perlinAmount;
		ofxToggle gui_meshType;
		ofxToggle gui_perlin;

		ofxTextField textField;

		ofxVec3Slider vec3Slider;
		

		//sound
		ofSoundPlayer sound;
		float *fft;
		float *soundSpectrum;
		int bands;

		vector<ofSoundPlayer> track;


};
