#version 330
#extension GL_ARB_separate_shader_objects : enable
// default130.frag: a simple fragment shader


layout(location = 0)uniform sampler2D texture;
layout(location = 1)uniform vec3 ka;           // ambient

layout(location = 0)in vec4 col;
layout(location = 1)in vec4 texC;

layout(location = 0)out vec4 fragColor ;

void main()
{

    fragColor = vec4(ka,1.0) * texture2D(texture,vec2(texC.x,texC.y));

}


