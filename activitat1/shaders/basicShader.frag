#version 330 core
in vec3 fs_color;
out vec4 FragColor;
uniform int boolsamarreta;
in vec4 coord;
void main() {
    if (boolsamarreta==1){
        if (fs_color ==vec3(0.0,0.0,0.5)){
            if(mod(coord.x+coord.y,0.20)<0.10){
                FragColor=vec4(1,0,0,0);
                }
            else{
                FragColor=vec4(1,1,1,0);
                }
            }
        else{
            FragColor=vec4(fs_color,1);
        }
     }
    else{
    FragColor = vec4(fs_color, 1);
    }
}
