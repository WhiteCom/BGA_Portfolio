#version 150

#ifdef GL_ES
precision mediump float; 
#else
precision highp float; // double, float64
#endif

//from vert
in vec4 colorV;

out vec4 fragColor;

void main()
{
	fragColor = colorV;
}



