#version 130
// default130.frag: a simple fragment shader

varying vec4 texC;
varying vec4 normalen;
uniform sampler2D texture;
uniform sampler2D textureSphere;
void main()
{

    gl_FragColor = texture2D(texture,vec2(texC.x,texC.y));
    gl_FragColor = texture2D(textureSphere,vec2(texC.x,texC.y));

}

