#version 430 core

 in vec4 vColor;

 out vec4 finalColor;

 uniform vec4 lightDiffuse;
 uniform vec4 materialDiffuse;
 uniform vec4 lightAmbient;
 uniform vec4 materialAmbient;
 uniform vec4 lightSpecular;
 uniform vec4 materialSpecular;
 uniform float shininess;

 in vec3 L;
 in vec3 N;
 in vec3 E;
 vec3 nL, nN, nE;

 void main(void) {

 vec4 color0 = materialDiffuse; // Material Color

 vec4 color1 = vec4(0.0, 0.0, 0.0, 1.0);    // Silhouette Color

 vec4 color2 = materialSpecular; // Specular Color

 nE = normalize(E);
 nN = normalize(N);
 nL = normalize(L);

 vec3 EyeLight = normalize(nL+nE);

 // Simple Silhouette
 float sil = max(dot(nN,nE), 0.0);
 if (sil < 0.0) {
     finalColor = color1;
 }
 else
 {
    finalColor = color0;

    // Specular part
    float specular = pow(max(dot(nN,nE),0.0), shininess);

    if (specular < 0.2) finalColor *= 0.8;
    else finalColor = color2;

    if (specular > 0.2 && specular < 0.8) finalColor = materialDiffuse * 0.6;

    // Diffuse part
    float diffuse = max(dot(nN,nL),0.0);
    if (diffuse < 0.3) finalColor *=0.8;
    }
}
 

