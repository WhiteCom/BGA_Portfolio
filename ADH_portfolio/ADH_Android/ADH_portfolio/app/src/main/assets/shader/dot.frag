#ifdef GL_ES
precision mediump float;
#define VARYING varying

#define FRAGCOLOR gl_FragColor

#else// Window
#version 150
#define VARYING in

out vec4 fragColor;
#define FRAGCOLOR fragColor

#endif

uniform vec2 center;
uniform float radius;

VARYING vec4 colorV;

void main()
{
	FRAGCOLOR = vec4(colorV.xyz, clamp(radius - length(center - gl_FragCoord.xy), 0.0, 1.0));
}
