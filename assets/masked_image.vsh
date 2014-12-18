uniform mat4 u_mvpmatrix;
attribute vec2 a_position;
attribute vec2 a_texcoord;
attribute vec2 a_masktexcoord;
varying vec2 v_texcoord;
varying vec2 v_masktexcoord;
void main()
{
	gl_Position = u_mvpmatrix * vec4(a_position, 0.0, 1.0);
	v_texcoord = a_texcoord;
	v_masktexcoord = a_masktexcoord;
}
