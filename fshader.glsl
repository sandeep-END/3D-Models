#version 330

in vec3 normal;
in vec4 eye;
in vec4 COLOR;
in vec4 pos;
in vec2 tex;

uniform vec4 Mdiffuse;
uniform vec4 Mspecular;
uniform vec4 Mambient;
uniform float Mshine;
uniform float Light1;
uniform float Light2;
uniform float SpotLight;
uniform mat4 viewMatrix;
uniform sampler2D texture;

out vec4 frag_color;

void main () 
{
  // Defining Materials
  vec4 diffuse = Mdiffuse; 
  vec4 specular = Mspecular;
  float shininess = Mshine;
  float spotInt = SpotLight;
  vec4 ambient = Mambient;
  vec4 spec1= vec4(0.0);
  vec4 spec2 = vec4(0.0);
  vec4 Sspec = vec4(0.0);

  // Defining Light 
  vec4 lightPos1 = vec4(1.0, 1.0, 1.0, 0.0);
  vec3 lightDir1 = vec3(viewMatrix * lightPos1);  // Transforms with camera
  lightDir1 = normalize( vec3(lightDir1));

  vec4 lightPos2 = vec4(-1.0, 1.0, 0.0, 0.0);
  vec3 lightDir2 = vec3(viewMatrix * lightPos2);  // Transforms with camera
  lightDir2 = normalize( vec3(lightDir2));

  vec4 SlightPos = vec4(3.0, 11.0, 6.0, 0.0);
  vec4 pointdir = SlightPos-pos;
  float coneangle = dot(normalize( vec3(pointdir)),normalize(vec3(SlightPos)));
  vec3 SlightDir = vec3(viewMatrix * SlightPos);  // Transforms with camera
  SlightDir = normalize( vec3(SlightDir));


  //Diffuse
  vec3 n = normalize(vec3(normal));
  float dotProduct1 = dot(n, lightDir1);
  float intensity1 =  max( dotProduct1, 0.0);

  float dotProduct2 = dot(n, lightDir2);
  float intensity2 =  max( dotProduct2, 0.0);


  float SdotProduct = dot(n, SlightDir);
  float Sintensity =  max( SdotProduct, 0.0);
  // Compute specular component only if light falls on vertex

  if(Sintensity > 0)
  {
	vec3 e = normalize(vec3(eye));
	vec3 h = normalize(lightDir1 + e );
   	float intSpec = max(dot(h,n), 0.0);	
    Sspec = specular * pow(intSpec, shininess);
  }  

  if(intensity1 > 0.0)
  {
	vec3 e = normalize(vec3(eye));
	vec3 h = normalize(lightDir1 + e );
   	float intSpec = max(dot(h,n), 0.0);	
    spec1 = specular * pow(intSpec, shininess);
  }  

  if(intensity2 > 0.0)
  {
  	vec3 e = normalize(vec3(eye));
  	vec3 h = normalize(lightDir2 + e );
    float intSpec = max(dot(h,n), 0.0);	
    spec2 = specular * pow(intSpec, shininess);
  }  

  if(coneangle<0.80602540378){
  	spotInt = 0;
  }

  vec4 color1 = Light1*max((intensity1 * diffuse  + spec1)*COLOR, ambient); // All
  vec4 color2 = Light2*max((intensity2 * diffuse  + spec2)*COLOR, ambient); // All
  vec4 Scolor = spotInt*max((Sintensity * diffuse  + Sspec)*COLOR, ambient); // All


  frag_color = (color1+color2+Scolor)*texture2D(texture, tex);

}

