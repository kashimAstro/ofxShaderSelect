-- VERTEX

void main(){
	gl_Position = ftransform();
}

-- FRAGMENT

void main(){
	gl_FragColor = vec4(1.0,0.0,0.0,1.0);
}

-- GEOMETRY

void main(){
	gl_Position = gl_in[i].gl_Position;
}


-- TESSCONTROL

void main(){}

-- TESSEVAL

void main(){}
