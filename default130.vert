#version 130
// default130.vert: a simple vertex shader


uniform mat4 modelMatrix;
uniform mat4 viewlMatrix;
uniform mat4 perspectiveMatrix;
attribute vec4 vert;
attribute vec4 normale;
attribute vec4 texCoord;
varying vec4 texC;
varying vec4 col;
varying vec4 normalen;

void main(void)
{
    normalen = normale;
    texC = texCoord;
    gl_Position = perspectiveMatrix * viewlMatrix * modelMatrix * vert;
}



