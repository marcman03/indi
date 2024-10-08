#version 330 core

in vec3 fColor;
in vec3 matamb_interp;
in vec3 matdiff_interp;
in vec3 matspec_interp;
in float matshin_interp;
in vec3 normal_sco;
in vec3 llum_sco;
in vec3 llumSnitch0;
in vec3 llumSnitch1;
in vec4 vertex_sco;
uniform vec3 colorFocus;
uniform vec3 llumAmbient;
uniform vec3 colorFocusSnitch;
uniform float  translucid;
uniform mat3 NormalMatrix;
uniform float activallumSnitch0;
uniform float activallumSnitch1;
in vec3 vertex_sca;
//--------------------------------------
out vec4 FragColor;
//--------------------------------------

//-------------------------------------------------------
//   Funció de soroll per fer efectes especials amb el shader
//	<https://www.shadertoy.com/view/4dS3Wd>
//	By Morgan McGuire @morgan3d, http://graphicscodex.com
//-------------------------------------------------------
float hash(float n) { return fract(sin(n) * 1e4); }
float hash(vec2 p) { return fract(1e4 * sin(17.0 * p.x + p.y * 0.1) * (0.1 + abs(sin(p.y * 13.0 + p.x)))); }

float noise(vec2 x) {
        vec2 i = floor(x);
        vec2 f = fract(x);

        // Four corners in 2D of a tile
        float a = hash(i);
        float b = hash(i + vec2(1.0, 0.0));
        float c = hash(i + vec2(0.0, 1.0));
        float d = hash(i + vec2(1.0, 1.0));

        // Same code, with the clamps in smoothstep and common subexpressions
        // optimized away.
        vec2 u = f * f * (3.0 - 2.0 * f);
        return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y-0.5;
}

//--------------------------------------
//    Funcions del model de Phong
//--------------------------------------

vec3 Ambient() {
    return llumAmbient * matamb_interp;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus)
{
    // Tant sols retorna el terme difús
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
    vec3 colRes = vec3(0);
    // Càlcul component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colFocus * matdiff_interp * dot (L, NormSCO);
    return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec4 vertSCO, vec3 colFocus)
{
    // Tant sols retorna el terme especular!
    // Els vectors rebuts com a paràmetres (NormSCO i L) estan normalitzats
    vec3 colRes = vec3 (0);
    // Si la llum ve de darrera o el material és mate no fem res
    if ((dot(NormSCO,L) < 0) || (matshin_interp == 0))
        return colRes;  // no hi ha component especular

      // Calculem R i V
      vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
      vec3 V = normalize(-vertSCO.xyz); // perquè la càmera està a (0,0,0) en SCO

      if (dot(R, V) < 0)
        return colRes;  // no hi ha component especular

      // Calculem i retornem la component especular
      float shine = pow(max(0.0, dot(R, V)), matshin_interp);
      return (matspec_interp * colFocus * shine);
  }


//--------------------------------------
//    Mètode main
//--------------------------------------
void main()
{
    vec3 normal_scon=normal_sco;
    if (vertex_sca.y<0.1){
        float w=2.0;
        float d=0.2;
        float residueX = mod(vertex_sca.x, w);
        float residueZ=mod(vertex_sca.z, w);

        if (residueX < d ) {
            normal_scon+=vec3(-1.0, 1.0, 0.0);


            }

        else if (residueX >w- d ) {
            normal_scon+=vec3(1.0, 1.0, 0.0);


        }

        else if(residueX < d && residueX >w- d ){
            normal_scon+=vec3(0.0, 0.1, 0.0);

        }
        else if (residueZ < d ) {
            normal_scon+=vec3(0.0, 1.0, -1.0);
            }
        else if (residueZ >w- d ) {
            normal_scon+=vec3(0.0, 1.0, +1.0);
            }
        else if (residueZ < d && residueZ >w- d ) {
            normal_scon+=vec3(0.0, 1.0, +0.0);
            }
        float r= noise(vec2(vertex_sca.x,vertex_sca.y));
        vec3 soroll=r*vec3(1, 0, 0);
        normal_scon+=soroll;
        normal_scon=NormalMatrix*normal_scon;


    }
    normal_scon=normalize(normal_scon);

    vec3 llum_scon=normalize(llum_sco);
    vec3 llumSnitch0n=normalize(llumSnitch0);
    vec3 llumSnitch1n=normalize(llumSnitch1);
    vec4 Snitch0=vec4(0,0,0,0);
    vec4 Snitch1=vec4(0,0,0,0);
    vec4 llum_inicial=vec4(Ambient(),1)+vec4(Difus(normal_scon,llum_scon,colorFocus),1)+vec4(Especular(normal_scon,llum_scon,vertex_sco,colorFocus),1);
    if(activallumSnitch0==1.0f){
        Snitch0=vec4(Difus(normal_scon,llumSnitch0n,colorFocusSnitch),1)+vec4(Especular(normal_scon,llumSnitch0n,vertex_sco,colorFocusSnitch),1);
    }
    if(activallumSnitch1==1.0f){
       Snitch1=vec4(Difus(normal_scon,llumSnitch1n,colorFocusSnitch),1)+vec4(Especular(normal_scon,llumSnitch1n,vertex_sco,colorFocusSnitch),1);
    }
    FragColor = llum_inicial+Snitch0+Snitch1;

    if (translucid==1.0f){
        FragColor.a=0.5;
    }
    else if (translucid==0.0f){
        FragColor.a=1.0;
    }
}
