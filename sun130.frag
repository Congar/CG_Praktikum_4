#version 130
// default130.frag: a simple fragment shader

varying vec4 texC;
varying vec4 normalen;
uniform sampler2D texture;
void main()
{

    //gl_FragColor = normalen ;
    gl_FragColor = texture2D(texture,vec2(texC.x,texC.y));
    //gl_FragColor = ve4(1.0f, 1.0f, 0.0f, 1.0f);
}

