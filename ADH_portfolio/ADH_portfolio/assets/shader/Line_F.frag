#version 150

#ifdef GL_ES
precision mediump float; 
#else
precision highp float; // double, float64
#endif

uniform vec2 sp;
uniform vec2 ep;
//uniform float radius;

//from vert
in vec4 colorV;

out vec4 fragColor;

//#issue! see ishadertoy -> iResolution
//see more shadertoy drawLine
//float segment(vec2 P, vec2 A, vec2 B, float r) 
//{
//    vec2 AB = B - A;
//    vec2 AP = P - A;
//    float d = length(h - g * clamp(dot(g, h) / dot(g,g), 0.0, 1.0));
//	return smoothstep(r, 0.5*r, d);
//}

void main()
{
	//float intensity = segment(gl_FragCoord.xy, sp, ep, 0.1f);
	//
	//fragColor = vec4(colorV.xyz, intensity);
	fragColor = vec4(colorV.xyz, clamp(length(sp-ep), 1.0f, 1.0f));
}



