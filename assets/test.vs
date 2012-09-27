
uniform mat4 MVP;
attribute vec3 pos;
attribute vec2 texcoord;

varying vec2 frag_texcoord;

void main() 
{ 
	gl_Position = MVP * vec4(pos, 1.0f); 
	frag_texcoord = texcoord;
}
