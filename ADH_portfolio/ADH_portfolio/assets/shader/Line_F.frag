#version 150

#ifdef GL_ES
precision mediump float; 
#else
precision highp float; // double, float64
#endif

uniform vec2 sp;
uniform vec2 ep;
uniform float radius;

in vec4 colorV;

out vec4 fragColor;

float LineSegment(vec2 U, vec2 A, vec2 B)
{
	vec2 UA = U - A;
	vec2 BA = B - A;

	float s = dot(UA, BA) / length(BA);
	s = s / length(BA);

	s = clamp(s, 0.0, 1.0);
	return step(length(UA - s * BA), radius);
}

void main()
{
	vec2 U = gl_FragCoord.xy;
	float l = LineSegment(U, sp, ep);
	
	fragColor = vec4(colorV.xyz, l);
}



