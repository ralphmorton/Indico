
precision mediump float;

uniform sampler2D tex;

varying vec2 frag_texcoord;

void main() 
{ 
	gl_FragColor = texture2D(tex, frag_texcoord);
}
