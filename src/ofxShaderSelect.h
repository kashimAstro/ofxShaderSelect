#include "ofMain.h"

class ofxShaderSelect{
	public:
	ofShader shader;
	ofFile file;
	string buffer;
	vector<string> bufferShader;
	int counter;

	string VERTEX      = "VERTEX";
	string GEOMETRY    = "GEOMETRY";
	string FRAGMENT    = "FRAGMENT";
	string TESSCONTROL = "TESSCONTROL";
	string TESSEVAL    = "TESSEVAL";

	vector<string> load(string path){
		bufferShader.clear();
        file.open(path, ofFile::ReadWrite, false);
        ofBuffer buff = file.readToBuffer();
        vector<string> source = ofSplitString(buff.getText(),"--");
        for(int i = 0; i < source.size(); i++) {
			if(source[i] != "") {
                vector<string> split = ofSplitString(source[i],"\n");
				for(int j = 0; j < split.size(); j++) {
					if( i == counter ) {
                        buffer+=split[j]+"\n";
					}
                }
			}
			ofStringReplace(buffer, VERTEX,     "");
			ofStringReplace(buffer, GEOMETRY,   "");
			ofStringReplace(buffer, FRAGMENT,   "");
			ofStringReplace(buffer, TESSCONTROL,"");
			ofStringReplace(buffer, TESSEVAL,   "");
			bufferShader.push_back(buffer);
			counter++;
			buffer="";
        }
		return bufferShader;
	}

    ofShader active(vector<string> source, int version, GLenum input=GL_POINTS, GLenum output=GL_TRIANGLE_STRIP, int outputCount=60) {
        if(source.size()>=4) {
            shader.setGeometryInputType(input);
            shader.setGeometryOutputType(output);
            shader.setGeometryOutputCount(outputCount);
            ofLog()<<"geometry headers!";
        }
        ofLog()<<source.size();
        if(source.size()>=2) { shader.setupShaderFromSource(GL_VERTEX_SHADER,          "\n#version "+ofToString(version)+"\n"+source[1]); ofLog()<<"vertex shader!"; }
        if(source.size()>=3) { shader.setupShaderFromSource(GL_FRAGMENT_SHADER,        "\n#version "+ofToString(version)+"\n"+source[2]); ofLog()<<"fragment shader!"; }
        if(source.size()>=4) { shader.setupShaderFromSource(GL_GEOMETRY_SHADER_EXT,    "\n#version "+ofToString(version)+"\n"+source[3]); ofLog()<<"geometry shader!"; }
        if(source.size()>=5) { shader.setupShaderFromSource(GL_TESS_CONTROL_SHADER,    "\n#version "+ofToString(version)+"\n"+source[4]); ofLog()<<"tess control shader!"; }
        if(source.size()>=6) { shader.setupShaderFromSource(GL_TESS_EVALUATION_SHADER, "\n#version "+ofToString(version)+"\n"+source[5]); ofLog()<<"tess evaluation shader!"; }
        if(source.size()>=2) { shader.linkProgram(); ofLog()<<"link program start!"; }
        return shader;
    }
};
