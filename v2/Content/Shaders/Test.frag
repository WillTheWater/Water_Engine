#version 130

uniform sampler2D Source;

void main()
{
	vec2 uv = gl_TexCoord[0].xy;
	vec4 col = texture2D(Source, uv);
	gl_FragColor = vec4(1.0 - col.rgb, col.a);
}