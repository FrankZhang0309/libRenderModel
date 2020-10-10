#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec4 aId;

out vec4 Id;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float pointSize;

uniform int width;
uniform int height;

void main()
{
    gl_PointSize = pointSize;

    gl_Position = projection*view*model*vec4(aPos, 1.0);
    Id = aId;

}
