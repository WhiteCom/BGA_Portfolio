#version 150

#ifdef GL_ES
precision mediump float; 
#else
precision highp float; // double, float64
#endif

uniform vec2 center;
uniform float radius;
uniform float lineWidth;

//from vert
in vec4 colorV;

out vec4 fragColor;

void main()
{
	float w = lineWidth / 2;

	float d = abs( length(center - gl_FragCoord.xy) - (radius - w));

	if(d > w)
		fragColor = vec4(colorV.rgb, 0.0f);
	else
		fragColor = vec4(colorV.rgb, colorV.a);

	//fragColor = vec4(colorV.xyz, clamp(radius - length(center - gl_FragCoord.xy), 0.0f, 1.0f));
}



