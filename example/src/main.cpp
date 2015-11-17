#include "ofMain.h"
#include "ofxShaderSelect.h"

class ofApp : public ofBaseApp{
	public:

	ofxShaderSelect shader;

	void setup(){
		vector<string> shaders = shader.load("shader.glsl");
		shader.active(shaders);                
	}

	void update(){

	}

	void draw(){

	}
};

int main( ){
	ofSetupOpenGL(1024,768, OF_WINDOW);
	ofRunApp( new ofApp());
}
