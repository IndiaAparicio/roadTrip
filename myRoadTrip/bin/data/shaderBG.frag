#version 120

precision highp float;

uniform float u_bgBeat;

void main()
{
    // gl_FragCoord contains the window relative coordinate for the fragment.
    // we use gl_FragCoord.x position to control the red color value.
    // we use gl_FragCoord.y position to control the green color value.
    // please note that all r, g, b, a values are between 0 and 1.
    
    float windowWidth = 1024.0;
    float windowHeight = 768.0;
    
	float r = gl_FragCoord.x / windowHeight * u_bgBeat;
	float g = 0.0;
	float b = gl_FragCoord.y / windowHeight * u_bgBeat;
	// float b = gl_FragCoord.y / windowHeight * u_bgBeat;
	float a = 1.0;



	gl_FragColor = vec4(r, g, b, a);
}
