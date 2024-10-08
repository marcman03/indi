#version 330 core

//-------------------------------
in vec3 vertex;
in vec3 normal;
in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;


out vec3 matamb_interp;
out vec3 matdiff_interp;
out vec3 matspec_interp;
out float matshin_interp;
//-------------------------------
uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;
//-------------------------------
out vec3 fColor;
uniform vec3 posFocus;
out vec3 normal_sco;
out vec3 llum_sco;
out vec3 llumSnitch0;
out vec3 llumSnitch1;
out vec4 vertex_sco;
out vec3 vertex_sca;
uniform mat3 NormalMatrix;
uniform vec4 posFocus_sco;
uniform vec4 posFocusSnitch0;
uniform vec4 posFocusSnitch1;

void main()
{	
    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
    fColor = matdiff;
    vertex_sca=vertex;


    normal_sco = NormalMatrix * normal;
    vertex_sco = view * TG * vec4 (vertex, 1.0);
    llum_sco = posFocus_sco.xyz - vertex_sco.xyz;
    llumSnitch0=posFocusSnitch0.xyz-vertex_sco.xyz;
    llumSnitch1=posFocusSnitch1.xyz - vertex_sco.xyz;
    matamb_interp  = matamb;
    matdiff_interp = matdiff;
    matspec_interp = matspec;
    matshin_interp = matshin;
}
