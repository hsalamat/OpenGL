#version 430 core

layout(location = 0) in vec3 vPositionAttr;
layout(location = 1) in vec3 vColorAttr;
layout(location = 2) in vec4 vNormalAttr;

out vec4 vColor;

// Values that stay constant for the whole mesh.
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec4 lightPosition;
out vec4 normalAttr;
out vec3 L,N;

void main()
{
 // pos is vertex position in eye coordinates
  vec3 pos = (viewMatrix * modelMatrix * vec4(vPositionAttr,1.0)).xyz;

  //Transformed normal position
  //N = normalize(vec3(normalMatrix * vNormalAttr));

  N = normalize(mat3(transpose(inverse(modelMatrix))) * vNormalAttr.xyz);

  // check for directional light

  if(lightPosition.w == 0.0) L = normalize(lightPosition.xyz);
  else L = normalize( lightPosition.xyz - pos );


  gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vPositionAttr,1.0f);
}