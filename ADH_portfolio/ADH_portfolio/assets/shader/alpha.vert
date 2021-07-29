#version 150

#ifdef GL_ES
precision mediump float; //exe 파일같은거에서 읽어서 거기서 같이 컴파일 됨. 따로 컴파일 x
#endif

uniform mat4 projMatrix, viewMatrix;

in vec4 position;
in vec4 color;
in vec2 texcoord;

//to frag
out vec4 colorV; 
out vec2 texcoordV;

void main()
{
	gl_Position = projMatrix * viewMatrix * position;
	colorV = color;
	texcoordV = texcoord;
}



