#version 330
#extension GL_ARB_separate_shader_objects : enable
// default130.frag: a simple fragment shader


layout(location = 0)uniform sampler2D texture;
layout(location = 0)uniform vec3 ka;           // ambient
layout(location = 0)uniform vec4 lightPosition;


layout(location = 0)in vec4 col;
layout(location = 1)in vec4 texC;
layout(location = 0)in vec3 normal;
layout(location = 2)in vec3 position;

layout(location = 0)out vec4 fragColor ;

void main()
{

    vec3 n = normalize(normal);
    vec3 s = normalize(vec3(lightPosition)-position);


    vec3 diffuse = vec3(0.5,0.5,0.5) * max(dot(s,n),0.0) ;

    vec3 outputColor = ka + diffuse ;

    //fragColor = vec4(1.0,1.0,1.0,1.0);
    fragColor = vec4(outputColor,1.0) * texture2D(texture,vec2(texC.x,texC.y));
    //fragColor = vec4(ka,1.0) * texture2D(texture,vec2(texC.x,texC.y));

}


