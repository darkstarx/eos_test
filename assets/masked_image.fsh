#ifdef GL_ES
precision mediump float;
precision mediump sampler2D;
#endif
uniform vec4 u_color;
varying vec2 v_texcoord;
varying vec2 v_masktexcoord;
uniform sampler2D u_texture;
uniform sampler2D u_mask;
void main()
{
	gl_FragColor = (u_color * texture2D(u_texture, v_texcoord)) * texture2D(u_mask, v_masktexcoord).a;
}
