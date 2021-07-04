#version 120

precision highp float;

uniform float u_time;
uniform float u_resolution_x;
uniform float u_resolution_y;
#define TWO_PI 6.2831

void main()
{
    // gl_FragCoord contains the window relative coordinate for the fragment.
    // we use gl_FragCoord.x position to control the red color value.
    // we use gl_FragCoord.y position to control the green color value.
    // please note that all r, g, b, a values are between 0 and 1.
    
	vec2 resolution = vec2(u_resolution_x, u_resolution_y);

	float coord_x = gl_FragCoord.x / resolution.x;
    float coord_y = gl_FragCoord.y / resolution.y;
    vec2 coord = 25.0 * vec2(coord_x, coord_y); //make more "tiles"
    float repeat = abs(cos(u_time));


    for(int n = 1; n < 12; n++){ //make it more detailed
        float i = float(n);
        coord += vec2(0.7 /i* cos(i*coord.y + u_time + 0.3 * i) + 0.8, 0.4 / i * sin(coord.x + u_time + 0.3 * i) + 1.6);
            
    }

    
    vec3 color = vec3(0.3 * sin(coord.x) + 0.7 * (repeat * 0.4 + 0.6), 0.1 * cos(coord.y) + 0.0, sin(coord.x + coord.y) + 0.9);

    
    
    
	float r = abs(cos(u_time));
	float g = gl_FragCoord.y / resolution.y;
	float b = 1.0;
	float a = 1.0;



	gl_FragColor = vec4(color, a);
}
