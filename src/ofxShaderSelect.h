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
						buffer+=split[j];
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

	void active(vector<string> source, GLenum input=GL_POINTS, GLenum output=GL_TRIANGLE_STRIP, int outputCount=60) {
	       	shader.setGeometryInputType(input);
		shader.setGeometryOutputType(output);
		shader.setGeometryOutputCount(outputCount);
		for(int i = 0; i < source.size(); i++){
			ofLog()<<source[i];
		}
        //	shader.setupShaderFromSource(GL_VERTEX_SHADER,       source[0]);
	//      shader.setupShaderFromSource(GL_FRAGMENT_SHADER,     source[1]);
	//      shader.setupShaderFromSource(GL_GEOMETRY_SHADER_EXT, source[2]);
        //	shader.linkProgram();
        }
};
