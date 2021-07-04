#version 120

precision highp float;


uniform float u_red;
uniform float u_blue;
uniform float u_alpha;
uniform float u_time;
uniform float u_resolution_x;
uniform float u_resolution_y;

void main()
{
    // gl_FragCoord contains the window relative coordinate for the fragment.
    // we use gl_FragCoord.x position to control the red color value.
    // we use gl_FragCoord.y position to control the green color value.
    // please note that all r, g, b, a values are between 0 and 1.
    
    vec2 resolution = vec2(u_resolution_x, u_resolution_y);

	float coord_x = gl_FragCoord.x / resolution.x;
    float coord_y = gl_FragCoord.y / resolution.y;

    vec2 coord = 4.0 * vec2(coord_x, coord_y); //make more "tiles"

    float repeat = abs(cos(u_time));
    
	float r = gl_FragCoord.x / resolution.x + (u_red * 0.2);
	float g =  gl_FragCoord.y / resolution.y / 5;
	float b = u_blue;
	float a = u_alpha + 0.4;

    for(int n = 1; n < 8; n++){
        float i = float(n);
        coord += vec2(0.7 /i* cos(i*coord.y + u_time * 0.3 + 0.7 * i) + 0.8, 0.4 / i * sin(coord.x + u_time * 0.3 + 0.3 * i) + 1.6);
            
    }

    

    vec3 color = vec3(0.2 * sin(coord.x) + 0.7 + u_alpha * (repeat * 0.4 + 0.6), 0.1 * sin(coord.y) + 0.1, sin(coord.x + coord.y) + 0.9);

    



	gl_FragColor = vec4(color, a);
}
