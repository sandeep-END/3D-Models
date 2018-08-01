#version 330

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
in vec2 texCoord;

out vec3 normal;
out vec4 eye;
out vec2 tex;
out vec4 COLOR;
out vec4 pos;

uniform vec4 Mdiffuse;
uniform vec4 Mspecular;
uniform vec4 Mambient;
uniform float Mshine;
uniform float Light1;
uniform float Light2;
uniform float SpotLight;
uniform mat4 uModelViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 normalMatrix;
uniform mat4 viewMatrix;
void main (void) 
{
  tex = texCoord;
  gl_Position = uModelViewMatrix * vPosition;
  pos = ModelMatrix*vPosition;
  normal = (normalMatrix * normalize(vNormal)); 
  eye = - (uModelViewMatrix * gl_Position); 
  COLOR = vColor; 
}
