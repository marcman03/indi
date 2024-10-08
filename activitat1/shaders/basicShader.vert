#version 330 core
in vec3 color;
in vec3 vertex;
uniform mat4 TG;
uniform mat4 RM;
out vec3 fs_color;
uniform int boolsamarreta;
out vec4 coord;
void main()  {

    gl_Position = RM*TG * vec4 (vertex, 1.0);
    fs_color=color;
    coord=gl_Position;
}
