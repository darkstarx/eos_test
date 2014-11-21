#ifdef GL_ES
precision mediump float;
precision mediump sampler2D;
#endif
uniform vec4 u_color;
varying vec2 v_texcoord;
uniform sampler2D u_texture;
void main()
{
	gl_FragColor = u_color/* * texture2D(u_texture, v_texcoord)*/;
}
