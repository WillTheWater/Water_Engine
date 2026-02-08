#version 130

uniform sampler2D Source;
uniform float Time;

void main()
{
	 vec2 uv = gl_TexCoord[0].xy;
    
    float x = uv.x * 20.0 + Time;
    float y = uv.y * 20.0 + Time * 0.5;
    
    float wave = sin(x) * cos(y) * 0.03;
    wave += sin(x * 0.5 + Time) * 0.02;
    
    vec2 distorted = uv + vec2(wave, sin(Time) * 0.02);
    
    distorted = fract(distorted);
    
    vec4 col = texture2D(Source, distorted);
    
    col.rgb = mix(col.rgb, vec3(0.0, 0.3, 0.6), 0.2);
    
    gl_FragColor = col;
}