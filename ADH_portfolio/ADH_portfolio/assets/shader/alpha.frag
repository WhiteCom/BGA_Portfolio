#version 150

#ifdef GL_ES
precision mediump float; 
#else
precision highp float; // double, float64
#endif

uniform vec2 center;

uniform sampler2D tex;

//from vert
in vec4 colorV;
in vec2 texcoordV;

out vec4 fragColor;

void main()
{
	fragColor = texture(tex, texcoordV) * colorV;

}



