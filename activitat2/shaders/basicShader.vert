#version 330 core

in vec3 vertex;
in vec3 color;
uniform bool pinta2;
uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

out vec3 fcolor;

void main() {
    if (pinta2){
        //pintem el morty2 de vermell
        fcolor=color*vec3(1,0,0);
    }
    else{
    fcolor = color;
    }
    gl_Position = Proj * View * TG * vec4 (vertex, 1.0);
}
