attribute vec2 a_position;
attribute vec4 a_color;
attribute vec2 a_texCoord;

uniform mat3 u_projTrans;

varying vec4 v_color;
varying vec2 v_texCoord;

void main()
{
	v_color = a_color;
	v_texCoord = a_texCoord;
	gl_Position = vec4(u_projTrans * vec3(a_position, 1.0), 1.0) - vec4(0.0, 0.0, 1.0, 0.0);
	//gl_Position = vec4(a_position, 0.0, 1.0);
}