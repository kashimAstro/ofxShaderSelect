#include "ofMain.h"
#include "ofxShaderSelect.h"
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"

class ofApp : public ofBaseApp{
    public:

        ofxShaderSelect shader;
        ofShader pass;
        ofEasyCam cam;
        ofxAssimpModelLoader assimp;

        ofVbo vbo;
        ofxPanel gui;
        ofParameter<int> levelInner;
        ofParameter<int> levelOuter;
        ofParameter<ofVec3f> lightpos;
        ofParameter<ofVec3f> diffMat;
        ofParameter<ofVec3f> ambMat;
        ofParameter<ofVec3f> speMat;
        ofParameter<ofVec3f> translate;
        ofParameter<ofVec4f> rotation;
        ofParameter<float> speedanim;

        bool stopRotate;
        ofImage colorTex,normalTex;

        void setup(){
            ofSetVerticalSync(false);
            vector<string> shaders = shader.load("quads.glsl");
            pass = shader.active(shaders,440);

            gui.setup();
            gui.add(levelInner.set("level inner", 1, 1, 8));
            gui.add(levelOuter.set("level outer", 1, 1, 8));
            gui.add(lightpos.set("light pos",      ofVec3f(30.,24.,0.),         ofVec3f(0.), ofVec3f(50.)));
            gui.add(diffMat.set("diffuse material",ofVec3f(0.04f, 0.04f, 0.04f),ofVec3f(0.), ofVec3f(1.)));
            gui.add(ambMat.set("ambient material", ofVec3f(0, 0.75, 0.75),      ofVec3f(0.), ofVec3f(1.)));
            gui.add(speMat.set("specular material", ofVec3f(0, 0.5, 1.0),      ofVec3f(0.), ofVec3f(1.)));
            gui.add(translate.set("translate", ofVec3f(122.,-265.,-642.), ofVec3f(-1000.), ofVec3f(1000.)));
            gui.add(rotation.set("rotation", ofVec4f(0.8,-0.1,0.8,0.),ofVec4f(-1.,-1.,-1.,-360.),ofVec4f(1.,1.,1.,360.) ));
            gui.add(speedanim.set("speedAnim", 0, 0, 2500));

            cam.setNearClip(.1);
            cam.setFarClip(100000);

            //assimp.loadModel("model.obj",true);
            assimp.loadModel("monster-animated-character-X.X",true);
            vbo.setMesh(assimp.getMesh(0),GL_DYNAMIC_DRAW);

            shader.setUniform();//Test
            ofDisableArbTex();
            colorTex.load("c.png");
            normalTex.load("n.png");
        }

        int counter;
        float animationPosition;

        void update() {
            //ofSetWindowTitle(ofToString(ofGetFrameRate()));
            assimp.update();
            animationPosition = ofMap(counter,0,speedanim,0.0,1.0);
            counter++;
            if(counter>speedanim) counter=0;
            assimp.setPositionForAllAnimations(animationPosition);
            vbo.setMesh(assimp.getCurrentAnimatedMesh(0),GL_DYNAMIC_DRAW);
        }

        void draw(){
            ofBackgroundGradient(ofColor(100),ofColor(0));
            //ofBackground(0);

            cam.begin();
            ofEnableDepthTest();
            pass.begin();
            ofMatrix4x4 camdist;
            camdist.preMultTranslate(translate);
            camdist.preMultRotate(ofQuaternion(rotation));

            pass.setUniform3f("vLightPosition",lightpos->x,lightpos->y,lightpos->z);
            pass.setUniform4f("diffuseColor",diffMat->x,diffMat->y,diffMat->z,1.);
            pass.setUniform4f("ambientColor",ambMat->x,ambMat->y,ambMat->z,1.);
            pass.setUniform4f("specularColor",speMat->x,speMat->y,speMat->z,1.);
            pass.setUniformMatrix4f("mvMatrix",assimp.getModelMatrix()*camdist);
            pass.setUniformMatrix3f("normalMatrix",shader.mat4ToMat3(ofGetCurrentNormalMatrix()));
            pass.setUniformMatrix4f("pMatrix",cam.getProjectionMatrix());

            pass.setUniform1f("TessLevelInner",levelInner);
            pass.setUniform1f("TessLevelOuter",levelOuter);

                    // vbo patches
                    glPatchParameteri(GL_PATCH_VERTICES, 3);
                    vbo.drawElements(GL_PATCHES, vbo.getNumVertices()*vbo.getNumVertices());

            pass.end();
            ofDisableDepthTest();
            cam.end();

            gui.draw();
        }

        void mouseMoved(int x, int y){

        }

        void keyPressed(int key){
            //stopRotate=!stopRotate;
            if(key == ' ') ofToggleFullscreen();
            if(key == 'r')
            {
                vector<string> shaders = shader.load("quads.glsl");
                pass = shader.active(shaders,440);
            }
        }
};

int main( ){
    ofSetupOpenGL(1280,700,OF_WINDOW);
    ofRunApp( new ofApp());
}
