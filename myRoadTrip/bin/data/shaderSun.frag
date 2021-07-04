#version 120

precision highp float;



uniform float u_time;
uniform float u_resolution_x;
uniform float u_resolution_y;

const int AMOUNT = 10;


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


    vec2 new_coord = 20.0 * (gl_FragCoord.xy - resolution / 2.0) / min(resolution.y, resolution.x);

    float len;

    for (int i = 0; i < AMOUNT; i++){
        len = length(vec2(new_coord.x, new_coord.y));
        new_coord.x = new_coord.x - cos(new_coord.y + sin(len)) + cos(u_time / 9.0);
        new_coord.y = new_coord.y + sin(new_coord.x + cos(len)) + sin(u_time / 12.0);
    }
    

	gl_FragColor = vec4(cos(len * .0), cos(len * .0), cos(len * 0.1), 1.0);
}