#include "ofMain.h"
#include "ofxShaderSelect.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
	public:

	ofxShaderSelect shader;
    ofShader pass;
    ofEasyCam cam;
    ofIcoSpherePrimitive ico;
    ofSpherePrimitive sphere;
    ofBoxPrimitive box;

    ofVbo vbo;
    ofVec3f normals;
    ofxPanel gui;
    ofParameter<int> levelInner;
    ofParameter<int> levelOuter;
    ofParameter<ofVec3f> lightpos;
    ofParameter<ofVec3f> diffMat;
    ofParameter<ofVec3f> ambMat;

	void setup(){
		vector<string> shaders = shader.load("shader.glsl");
        pass = shader.active(shaders,440);

        gui.setup();
        gui.add(levelInner.set("level inner", 1, 1, 8));
        gui.add(levelOuter.set("level outer", 1, 1, 8));
        gui.add(lightpos.set("light pos",      ofVec3f(0.),                 ofVec3f(0.), ofVec3f(50.)));
        gui.add(diffMat.set("diffuse material",ofVec3f(0.04f, 0.04f, 0.04f),ofVec3f(0.), ofVec3f(1.)));
        gui.add(ambMat.set("ambient material", ofVec3f(0, 0.75, 0.75),      ofVec3f(0.), ofVec3f(1.)));

        cam.setNearClip(.1);
        cam.setFarClip(100000);

        ico.set(100,1);
        ofMesh m = ico.getMesh();
        vbo.setMesh(m,GL_DYNAMIC_DRAW);
	}

	void update(){

	}

	void draw(){
        ofBackground(0,0,95);
        cam.begin();
        ofEnableDepthTest();
        pass.begin();

        pass.setUniform3f("LightPosition",lightpos->x,lightpos->y,lightpos->z);
        pass.setUniform3f("DiffuseMaterial",diffMat->x,diffMat->y,diffMat->z);
        pass.setUniform3f("AmbientMaterial",ambMat->x,ambMat->y,ambMat->z);
        pass.setUniformMatrix4f("Modelview",cam.getModelViewMatrix());
        pass.setUniformMatrix3f("NormalMatrix",ofMatrix3x3(0.1,0.5,1.0,0.1,0.,0.,0.,0.,0.));
        pass.setUniform1f("TessLevelInner",levelInner);
        pass.setUniform1f("TessLevelOuter",levelOuter);
        pass.setUniformMatrix4f("Projection",cam.getProjectionMatrix());

            vbo.drawElements(GL_PATCHES, vbo.getNumVertices()*vbo.getNumVertices());

        pass.end();
        ofDisableDepthTest();
        cam.end();

        gui.draw();
	}

    void keyPressed(int key){
        if(key == ' '){
            pass.printActiveAttributes();
            pass.printActiveUniforms();
        }
    }
};

int main( ){
    ofSetupOpenGL(1280,700,OF_WINDOW);
    ofRunApp( new ofApp());
}
