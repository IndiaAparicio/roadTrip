#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	//SHADER
	//Pierre why do I need this? 
	// just writing this is not workung: 
	//shader.load("shader"); //is workiung now because I said version in the beginning
	//shader.load("shaderGL3/shader");
	//but it is gone

	//why do I need version 120 in frag?
	//how to do it with the mesh?
	//how can I use a mesh insteag of polygon mesh?
	//GUI
	//how good workflow for shader?
	//how add color in shader (prettier) I only find for VScode
	//check sun position?



	shader.load("shader"); //circles
	shaderBg.load("shaderBg"); //bg
	shaderPlanets.load("shaderPlanets"); //moon and planets
	shaderSun.load("shaderSun"); //sun
	shaderTerrain.load("shaderTerrain");//terrain

	//SOUND
	//sound.load("Polycycler.mp3");
	//sound.play();
	//sound.setLoop(true);




	fft = new float[512];
	for (int i = 0; i < 512; i++) {
		fft[i] = 0;
	}
	bands = 512;

	

	//GUI
	gui.setup();
	gui.add(gui_perlin.setup("Start/Stop", false));
	gui.add(gui_meshType.setup("Where are we?", false));
	gui.add(gui_circleResolution.setup("The are we going?", 35, 3, 50));
	gui.add(gui_perlinAmount.setup("Where are we at?", 1.0, 5.0, 10.0));
	gui.add(vec3Slider.setup("stars", ofVec3f(100, 500, 800), ofVec3f(0, 300, 700), ofVec3f(300, 700, 1000)));


	//MESHES AND SHAPES

	//making shapes smoother
	ofEnableSmoothing();
	ofSetCircleResolution(35);

	//set mode of mesh to traingle polygons
	mainMesh.setMode(OF_PRIMITIVE_TRIANGLES);

	

	//planets
	rotatePlanets = 1;
	ofVec3f sunPosition(0, -30, 30);

	//stars
	for (int i = 0; i < 1000; i++) {
		stars[i] = ofRandom(-150,150);
	}

	//mesh size
	width = 150;
	height = 150;

	//rendering Styles
	b_messyMesh = false;
	b_perlinMesh = false;
	b_drawWireFrame = true;
	b_marchMesh = false;

	//noise measures
	perlinHeight = 5.0;
	perlinRange = 1.0;

	//Background, color, camera
	//ofBackground(0);
	ofSetColor(255);
	mainCam.setPosition(0, 0, 80);

	

	//creating vertices for the mesh
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			mainMesh.addVertex(ofPoint(x - width / 2, y - height / 2, 0));
			mainMesh.addColor(ofFloatColor(50, 50, 100));
		}
	}

	//connecting vertices to triangles for the actual mesh 
	for (int y = 0; y < height - 1; y++) {
		for (int x = 0; x < width - 1; x++) {
			mainMesh.addIndex(x + y * width);			// 0, 0, 0
			mainMesh.addIndex((x + 1) + y * width);		// 1, 0, 0
			mainMesh.addIndex(x + (y + 1)*width);		// 0, 1, 0

			mainMesh.addIndex((x + 1) + y * width);		// 1, 0, 0
			mainMesh.addIndex((x + 1) + (y + 1)*width);	// 1, 1, 0
			mainMesh.addIndex(x + (y + 1)*width);		// 0, 1, 0
		}
	}

	
}

//--------------------------------------------------------------
void ofApp::update() {
	ofColor newColor, mountainColor;
	ofVec3f newPosition, oldPosition;


	//SOUND
	ofSoundUpdate();
	soundSpectrum = ofSoundGetSpectrum(bands);
	for (int i = 0; i < bands; i++) {
		fft[i] *= 0.9;
		if (fft[i] < soundSpectrum[i]) {
			fft[i] = soundSpectrum[i];
		}
	}


	//SHADER
	float time = ofGetElapsedTimef();
	float resolution_x = ofGetWidth();
	float resolution_y = ofGetHeight();
	shaderPlanets.begin();
	shaderPlanets.setUniform1f("u_time", time);
	shaderPlanets.setUniform1f("u_resolution_x", resolution_x);
	shaderPlanets.setUniform1f("u_resolution_y", resolution_y);
	shaderPlanets.end();


	float red = sin(ofGetElapsedTimef() * 0.5) * 2.0;
	float blue = sin(ofGetElapsedTimef()* 0.2) * 2.0;
	float alpha = fft[0] * 0.3;

	//setting uniform can only been done between shader.begin and shader.end
	shader.begin();
	shader.setUniform1f("u_red", red); //accessing shader and making a uniform
	shader.setUniform1f("u_blue", blue); //accessing shader and making a uniform //giving the uniform variable u_color from the shader the local variable color
	shader.setUniform1f("u_alpha", alpha);
	shader.setUniform1f("u_time", time);
	shader.setUniform1f("u_resolution_x", resolution_x);
	shader.setUniform1f("u_resolution_y", resolution_y);
	shader.end();

	shaderTerrain.begin();
	shaderTerrain.setUniform1f("u_time", time);
	shaderTerrain.setUniform1f("u_resolution_x", resolution_x);
	shaderTerrain.setUniform1f("u_resolution_y", resolution_y);
	shaderTerrain.end();

	shaderSun.begin();
	shaderSun.setUniform1f("u_time", time);
	shaderSun.setUniform1f("u_resolution_x", resolution_x);
	shaderSun.setUniform1f("u_resolution_y", resolution_y);
	shaderSun.end();



	
	float bgBeat = sin(ofGetElapsedTimef() * 0.5) * 0.2;
	//setting uniform can only been done between shader.begin and shader.end
	shaderBg.begin();
	shaderBg.setUniform1f("u_bgBeat", bgBeat); //accessing shader and making a uniform
	shaderBg.end();

	
	//SHAPES
	rotatePlanets++;
	//sunPosition.z -= 0.01;


	//MESH



	if (gui_perlin) {
		float time = ofGetSystemTimeMillis() / 100.0;
		int mountainHeight;
		int newMountainHeight;
		mountainColor.setHsb(230,255,255);
		int i = 0;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				//loop through every position/every vertex with index i and give it a new position in z-axis 
				//with a noise value in perlinRange and set color based on z-position
				newPosition = mainMesh.getVertex(i);
				newColor.setHsb(230 - ofMap(newPosition.z, 0, perlinHeight, 0, 65), 255 - ofMap(newPosition.z, 0, perlinHeight , 0, 95), 255);
				
				//giving z axis different values based on on street or mountains
				if (newPosition.x < 5 && newPosition.x > -5) {
					mainMesh.setColor(i, mountainColor);
					//newPosition.z = 1;
					//mainMesh.setVertex(i, newPosition);
				}
				else {
					mainMesh.setColor(i, newColor);
					//newPosition Range ist von -width/2 bis width/2
					
					mountainHeight = (abs(newPosition.x)); //Alle Werte auf positiv also Kurve 75 - 0 - 75
					newMountainHeight = ofMap(mountainHeight, 0, width/2, width/2, 0); // umkehren von 0 - 75 - 0 -> damit tiefe frequenzen am Rand sind

					//making the mountains far away from street higher with greater x-values at both edges 
					// + the new Range reacting to sound
					perlinHeight = (abs(newPosition.x))/3 + (fft[newMountainHeight]) * 10;

					//adding noise
					newPosition.z = ofNoise(ofMap(x, 0, width, 0, perlinRange), ofMap(y + time, 0, height, 0, perlinRange)) * perlinHeight;
					mainMesh.setVertex(i, newPosition);
				}
				i++;
			}
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw() {

	//how to map to sun
	ofBackground(0);
	ofEnableDepthTest();


	
	
	

	//CAMERA
	mainCam.begin();

	//SHAPES
	ofRotateXDeg(-90); //weil der beginn anders ist, sind achsen falsch

	//circle-shadows
	shader.begin();
	ofFill();
	ofSetColor(200, 0, 50, 60);
	for (int i = 0; i < 30; i += 5) {
		ofDrawCircle(0, sunPosition.y, width/1.9 - (i / 5), (i*3) + fft[i] * 15);
	}
	shader.end();

	//sun
	ofFill();
	ofSetColor(255, 255, 250);
	ofPushMatrix();
		ofTranslate(sunPosition.x, sunPosition.y, 30);
		ofRotateYDeg(90);
		ofRotateXDeg(rotatePlanets /10); //how fast planets are moving
		//sun
		shaderSun.begin();
		ofDrawIcoSphere(0, width/3, 0, 10); //y and z value is amount of distance of rotation
		shaderSun.end();
		//moon
		shaderPlanets.begin();
		ofDrawIcoSphere(0, -width/3,0, 8);
		
		//little planets
		for (int i = 0; i < 6; i++) {
			ofRotateYDeg((rotatePlanets*i) / 20);
			ofDrawIcoSphere(i * 3, width/3, 10, i / 2);
		}
		shaderPlanets.end();

	ofPopMatrix();

	shaderBg.begin();
	ofFill();
	ofDrawRectangle(-ofGetWidth()/2, -ofGetHeight()/2, 100, ofGetWidth(),ofGetHeight());
	shaderBg.end();

	ofFill();

	
	//STARS
	for (int i = 0; i < vec3Slider->x; i++) {
		ofSetColor(255, 255, 255, ofRandom(120, 250));
		ofDrawCircle(stars[i]*1.5, -1 * abs(stars[i-10]), 90, 0.45);
	}
	for (int i = 300; i < vec3Slider->y; i++) {
		ofSetColor(255, 255, 255, ofRandom(120, 250));
		ofDrawCircle(stars[i] * 1.5, -1 * abs(stars[i - 10]), 90, 0.15);
	}
	for (int i = 700; i < vec3Slider->z; i++) {
		ofSetColor(255, 255, 255, ofRandom(120, 250));
		ofDrawCircle(stars[i] * 1.5, -1 * abs(stars[i - 10]), 90, 0.3);
	}
	
	ofRotateXDeg(90);
	//end rotation
	
	


	//MESH-TERREAIN
	glPointSize(15); //giving Points a size -> hpw many pixels
	ofPushMatrix();
	ofTranslate(0, 0, -15);
	if (gui_meshType) {
		ofFill();
		mainMesh.draw();	// drawing the polygons
	}
	else {
		ofFill();
		shaderTerrain.begin();
		mainMesh.draw();
		shaderTerrain.end();
		//mainMesh.drawVertices();	//drawing only the vertices
	}
	ofPopMatrix();



	mainCam.end();

	ofDisableDepthTest();



	//GUI
	ofSetCircleResolution(gui_circleResolution);
	//ofSetColor(vec3Slider->x, vec3Slider->y, vec3Slider->z);
	perlinRange = gui_perlinAmount;
	
	gui.draw();


	//SOUND Visualisation //CAR
	
	ofFill();
	ofSetColor(0, 0, 0);
	ofDrawRectangle(0, ofGetHeight()-100, ofGetWidth(), 100);

	ofPushMatrix();
	ofTranslate((ofGetWidth() / 2) - (bands/2), ofGetHeight()-10);
	ofSetColor(50, 50,50+ fft[1]*90.0);
	ofDrawRectangle(0, 0, bands, -70);
	ofSetColor(255, 255, 255);
	for (int i = 0; i < 1; i++) {
		ofPolyline polyline;
		for (int j = 0; j < bands; j++) {
			polyline.addVertex(j, i - fft[j] * 70.0);
		}
		polyline.draw();
	}
	ofPopMatrix();

	//Writing a message 
	string msg = "This is my super cool road trip! Just drag and drop a song into the screen and enjoy your road trip!";
	ofDrawBitmapString(msg, 10, ofGetHeight()-80);


	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	switch (key) {
	case 'f':
		ofToggleFullscreen();
		break;
	case ' ':
		b_messyMesh = !b_messyMesh;
		break;
	case 'p':
		b_perlinMesh = !b_perlinMesh;
		break;
	case 'w':
		b_drawWireFrame = !b_drawWireFrame;
		break;
	case OF_KEY_UP:
		perlinRange += 0.1;
		break;
	case OF_KEY_DOWN:
		if (perlinRange > 0.1) {
			perlinRange -= 0.1;
		}
		break;
	case OF_KEY_RIGHT:
		perlinHeight += 0.1;
		break;
	case OF_KEY_LEFT:
		if (perlinHeight > 0.1) {
			perlinHeight -= 0.1;
		}
		break;
	case 'b':
		b_perlinMesh = !b_perlinMesh;
		break;
	case 'm':
		b_marchMesh = !b_marchMesh;
		break;
	}


}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

	//https://www.youtube.com/watch?v=h7jxTyiDBaM&list=PL4neAtv21WOlqpDzGqbGM_WN2hc5ZaVv7&index=24
	//MAKING SOUND DRAG IN DROP 

	if (dragInfo.files.size() > 0) {
		track.assign(dragInfo.files.size(), ofSoundPlayer());
		for (int s = 0; s < dragInfo.files.size(); s++) {
			track[s].load(dragInfo.files[s]);
			track[s].play();
			track[s].setLoop(true);
		}
	}

}
