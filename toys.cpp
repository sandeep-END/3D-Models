#include "toys.hpp"
#include "texture.hpp"


GLuint shaderProgram;
GLuint Light1,Light2,SpotLight;
GLuint vPosition,vColor,vNormal,texCoord,uModelViewMatrix,ModelMatrix,viewMatrix,normalMatrix,Mdiffuse,Mspecular,Mambient,Mshine;

GLuint vbo[2], vao[2];

glm::mat4 projection_matrix;
glm::mat4 c_rotation_matrix;
glm::mat4 lookat_matrix;

glm::mat4 view_matrix;
glm::mat3 normal_matrix;

glm::vec4 white(1.0, 1.0, 1.0, 1.0);


// generates and adds a new sphere with centre at origin
csX75::Model* add_sphere(csX75::Model* parent, GLuint tex, double radius, int Lats, int Longs, GLfloat tx, GLfloat ty, GLfloat tz, GLfloat rx, GLfloat ry, GLfloat rz)
{
	float lats,longs;

	float slices=(180/(float(Lats)*10))/2;
	float sectors=(180/(float(Longs)*10))/2;

	float l;

	int num_vertices=0;
	// count final number of vertices
	for(lats=0.0;lats<=PI;lats+=sectors){
		for(longs=0.0;longs<=2.0*PI; longs+=slices){
			num_vertices+=2;
		}
	}
	glm::vec4 v_positions[num_vertices];
	glm::vec4 v_colors[num_vertices];
	glm::vec4 v_normals[num_vertices];
	glm::vec2 v_texture[num_vertices];
	int tri_idx=0;
	for (lats = 0.0; lats <= PI; lats+=sectors)  
	{
		for(longs = 0.0; longs <= 2.0*PI; longs+=slices)
		{
			float x = radius * sin(lats) * cos(longs);
			float y = radius * sin(lats) * sin(longs);
			float z = radius * cos(lats);
			glm::vec4 pt(x, y, z, 1.0);
			float u,v;
			u = longs/(2.0*PI);
			v = lats/PI;

			v_colors[tri_idx] = white; v_positions[tri_idx] = pt; 
			v_texture[tri_idx] = glm::vec2(u,v);
			v_normals[tri_idx] = pt; tri_idx++;


			if(lats+sectors>PI)
				l=PI;
			else
				l=lats+sectors;
			x = radius * sin(l) * cos(longs);
			y = radius * sin(l) * sin(longs);
			z = radius * cos(l);
			v = l/PI;
			pt =glm::vec4(x, y, z, 1.0);
			v_colors[tri_idx] = white; v_positions[tri_idx] = pt;
			v_texture[tri_idx] = glm::vec2(u,v);
			v_normals[tri_idx] = pt; tri_idx++;
		}
	}
	csX75::Model* sphere = new csX75::Model(parent,num_vertices,tex,GL_TRIANGLE_STRIP,v_positions,v_colors,v_normals,v_texture,sizeof(v_positions),sizeof(v_colors),sizeof(v_normals),sizeof(v_texture));
	sphere->change_parameters(tx,ty,tz,rx,ry,rz);
	return sphere;
}
// generates and adds a new cylinder with centre at origin, oriented along Z axis
csX75::Model* add_cylinder(csX75::Model* parent, GLuint tex,double radius, double height, int cparts, int hparts, GLfloat tx, GLfloat ty, GLfloat tz, GLfloat rx, GLfloat ry, GLfloat rz)
{
	float thetha = 2.0*PI/float(cparts);
	float delta_z = height/float(hparts);

	float l;
	int num_vertices=0;
	float angle,hgt;
	// count final number of vertices
	for(angle=0.0;angle<=2*PI;angle+=thetha){
		num_vertices+=4;  // top and bottom circles
	}

	for(hgt=(-0.5*height);hgt<=(0.5*height);hgt+=delta_z){
		for(angle=0.0;angle<=2*PI;angle+=thetha){
			num_vertices+=2;  // curved surface
		}
	}
	glm::vec4 v_positions[num_vertices];
	glm::vec4 v_colors[num_vertices];
	glm::vec4 v_normals[num_vertices];
	glm::vec2 v_texture[num_vertices];

	int tri_idx=0;
	// lower circle
	for(angle=0.0; angle<=2*PI; angle+=thetha){
		float x = radius*cos(angle);
		float y = radius*sin(angle);
		float z = 0;
		glm::vec4 pt(x, y, z, 1.0);

		v_colors[tri_idx] = white; v_positions[tri_idx] = pt; 
		v_texture[tri_idx] = glm::vec2(0,0);
		v_normals[tri_idx] = glm::vec4(0,0,-1,1.0); tri_idx++;
		pt = glm::vec4(0.0,0.0,z,1.0);

		v_colors[tri_idx] = white; v_positions[tri_idx] = pt; 
		v_texture[tri_idx] = glm::vec2(0,0);
		v_normals[tri_idx] = glm::vec4(0,0,-1,1.0); tri_idx++;
	}
	// upper circle 
	for(angle=0.0; angle<=2*PI; angle+=thetha){
		float x = radius*cos(angle);
		float y = radius*sin(angle);
		float z = height;
		glm::vec4 pt(x, y, z, 1.0);

		v_colors[tri_idx] = white; v_positions[tri_idx] = pt; 
		v_texture[tri_idx] = glm::vec2(0,0);
		v_normals[tri_idx] = glm::vec4(0,0,1,1.0); tri_idx++;

		pt = glm::vec4(0.0,0.0,z,1.0);
		
		v_colors[tri_idx] = white; v_positions[tri_idx] = pt; 
		v_texture[tri_idx] = glm::vec2(0,0);
		v_normals[tri_idx] = glm::vec4(0,0,1,1.0); tri_idx++;
	}
	// curved surface
	for(hgt=0;hgt<=height;hgt+=delta_z){
		for(angle=0.0;angle<=2*PI;angle+=thetha){
			float l=hgt+delta_z;
			if(l>height){
				l = height;
			}
			float temp1 = radius*cos(angle);
			float temp2 = radius*sin(angle);
			float temp3 = radius*cos(angle+thetha);
			float temp4 = radius*sin(angle+thetha);
			v_colors[tri_idx] = white; v_positions[tri_idx] = glm::vec4(temp1,temp2,hgt,1.0);
			v_texture[tri_idx] = glm::vec2(angle/(2.0*PI),hgt/height);
			v_normals[tri_idx] = glm::vec4(temp1,temp2,0.0,1.0); tri_idx++;
			v_colors[tri_idx] = white; v_positions[tri_idx] = glm::vec4(temp1,temp2,l,1.0);
			v_texture[tri_idx] = glm::vec2(0,0);
			v_normals[tri_idx] = glm::vec4(temp1,temp2,0.0,1.0); tri_idx++;
		}
	}
	csX75::Model* cylinder = new csX75::Model(parent,num_vertices,tex,GL_TRIANGLE_STRIP,v_positions,v_colors,v_normals,v_texture,sizeof(v_positions),sizeof(v_colors),sizeof(v_normals),sizeof(v_texture));
	cylinder->change_parameters(tx,ty,tz,rx,ry,rz);
	return cylinder;
}
// generates and adds a new cuboid with sides along axes and one vertex at origin 
csX75::Model* add_cuboid(csX75::Model* parent, GLuint tex,double length, double breadth, double width, GLfloat tx, GLfloat ty, GLfloat tz, GLfloat rx, GLfloat ry, GLfloat rz)
{
	glm::vec2 t_coords[4] = {
	  glm::vec2( 0.0, 0.0),
	  glm::vec2( 0.0, 1.0),
	  glm::vec2( 1.0, 0.0),
	  glm::vec2( 1.0, 1.0)
	};
	int num_vertices=36;
	glm::vec4 v_positions[num_vertices];
	glm::vec4 v_colors[num_vertices];
	glm::vec4 v_normals[num_vertices];
	glm::vec2 v_texture[num_vertices];
	int tri_idx=0;
	v_positions[tri_idx]=glm::vec4(0.0,0.0,0.0,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(0.0,0.0,-1.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(length,0.0,0.0,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(0.0,0.0,-1.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(0.0,breadth,0.0,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(0.0,0.0,-1.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(0.0,breadth,0.0,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(0.0,0.0,-1.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(length,0.0,0.0,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(0.0,0.0,-1.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(length,breadth,0.0,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(0.0,0.0,-1.0,1.0);tri_idx++;

	v_positions[tri_idx]=glm::vec4(0.0,0.0,width,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(0.0,0.0,1.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(length,0.0,width,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[2];
	v_normals[tri_idx]=glm::vec4(0.0,0.0,1.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(0.0,breadth,width,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[1];
	v_normals[tri_idx]=glm::vec4(0.0,0.0,1.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(0.0,breadth,width,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[1];
	v_normals[tri_idx]=glm::vec4(0.0,0.0,1.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(length,0.0,width,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[2];
	v_normals[tri_idx]=glm::vec4(0.0,0.0,1.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(length,breadth,width,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[3];
	v_normals[tri_idx]=glm::vec4(0.0,0.0,1.0,1.0);tri_idx++;

	v_positions[tri_idx]=glm::vec4(length,0.0,0.0,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(1.0,0.0,0.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(length,0.0,width,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(1.0,0.0,0.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(length,breadth,width,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(1.0,0.0,0.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(length,breadth,width,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(1.0,0.0,0.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(length,breadth,0.0,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(1.0,0.0,0.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(length,0.0,0.0,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(1.0,0.0,0.0,1.0);tri_idx++;

	v_positions[tri_idx]=glm::vec4(0.0,0.0,0.0,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(-1.0,0.0,0.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(0.0,0.0,width,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(-1.0,0.0,0.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(0.0,breadth,width,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(-1.0,0.0,0.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(0.0,breadth,width,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(-1.0,0.0,0.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(0.0,breadth,0.0,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(-1.0,0.0,0.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(0.0,0.0,0.0,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(-1.0,0.0,0.0,1.0);tri_idx++;

	v_positions[tri_idx]=glm::vec4(0.0,0.0,0.0,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(0.0,-1.0,0.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(0.0,0.0,width,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(0.0,-1.0,0.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(length,0.0,width,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(0.0,-1.0,0.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(length,0.0,width,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(0.0,-1.0,0.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(0.0,0.0,0.0,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(0.0,-1.0,0.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(length,0.0,0.0,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(0.0,-1.0,0.0,1.0);tri_idx++;

	v_positions[tri_idx]=glm::vec4(0.0,breadth,0.0,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(0.0,1.0,0.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(0.0,breadth,width,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(0.0,1.0,0.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(length,breadth,width,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(0.0,1.0,0.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(length,breadth,width,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(0.0,1.0,0.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(0.0,breadth,0.0,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(0.0,1.0,0.0,1.0);tri_idx++;
	v_positions[tri_idx]=glm::vec4(length,breadth,0.0,1.0);v_colors[tri_idx]=white;
	v_texture[tri_idx] = t_coords[0];
	v_normals[tri_idx]=glm::vec4(0.0,1.0,0.0,1.0);tri_idx++;
	
	csX75::Model* cuboid = new csX75::Model(parent,num_vertices,tex,GL_TRIANGLES,v_positions,v_colors,v_normals,v_texture,sizeof(v_positions),sizeof(v_colors),sizeof(v_normals),sizeof(v_texture));
	cuboid->change_parameters(tx,ty,tz,rx,ry,rz);
	return cuboid;
}
//generates the bounding environment which is a cuboid.
csX75::Model* add_boundaries(csX75::Model* parent, GLuint tex,double length, double breadth, double width, GLfloat tx, GLfloat ty, GLfloat tz, GLfloat rx, GLfloat ry, GLfloat rz)
{
	csX75::Model* boundary = add_sphere(NULL,-1,CENTRE_RADIUS,50,50,0,0,0,0,0,0);
	GLuint wall=LoadTexture("images/wall.bmp",512,512);
	GLuint ground=LoadTexture("images/ground.bmp",512,512);
	GLuint sky=LoadTexture("images/sky.bmp",512,512);
	// top plane(skymap)
	if(true){
		int num_vertices = 6;
		glm::vec4 v_positions[6] = {
			glm::vec4(0.0, 0.0, width, 1.0),
			glm::vec4(0.0, breadth, width, 1.0),
			glm::vec4(length, breadth, width, 1.0),
			glm::vec4(length, breadth, width, 1.0),
			glm::vec4(length, 0.0, width, 1.0),
			glm::vec4(0.0, 0.0, width, 1.0)
		};
		glm::vec2 v_texture[6] = {
			glm::vec2(0.0, 0.0),
			glm::vec2(0.0, 1.0),
			glm::vec2(1.0, 1.0),
			glm::vec2(1.0, 1.0),
			glm::vec2(1.0, 0.0),
			glm::vec2(0.0, 0.0)
		};
		glm::vec4 v_colors[6] = {
			white,
			white,
			white,
			white,
			white,
			white
		};
		glm::vec4 v_normals[6] = {
			glm::vec4(0.0, 0.0, 1.0, 1.0),
			glm::vec4(0.0, 0.0, 1.0, 1.0),
			glm::vec4(0.0, 0.0, 1.0, 1.0),
			glm::vec4(0.0, 0.0, 1.0, 1.0),
			glm::vec4(0.0, 0.0, 1.0, 1.0),
			glm::vec4(0.0, 0.0, 1.0, 1.0)
		};
		csX75::Model* cuboid = new csX75::Model(boundary,num_vertices,wall,GL_TRIANGLES,v_positions,v_colors,v_normals,v_texture,sizeof(v_positions),sizeof(v_colors),sizeof(v_normals),sizeof(v_texture));
		cuboid->change_parameters(tx,ty,tz,rx,ry,rz);
		cuboid->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	}
	// bottom plane(groundmap)
	if(true){
		int num_vertices = 6;
		glm::vec4 v_positions[6] = {
			glm::vec4(0.0, 0.0, 0, 1.0),
			glm::vec4(length, 0.0, 0, 1.0),
			glm::vec4(length, breadth, 0, 1.0),
			glm::vec4(length, breadth, 0, 1.0),
			glm::vec4(0.0, breadth, 0, 1.0),
			glm::vec4(0.0, 0.0, 0, 1.0)
		};
		glm::vec2 v_texture[6] = {
			glm::vec2(0.0, 0.0),
			glm::vec2(1.0, 0.0),
			glm::vec2(1.0, 1.0),
			glm::vec2(1.0, 1.0),
			glm::vec2(0.0, 1.0),
			glm::vec2(0.0, 0.0)
		};
		glm::vec4 v_colors[6] = {
			white,
			white,
			white,
			white,
			white,
			white
		};
		glm::vec4 v_normals[6] = {
			glm::vec4(0.0, 0.0, -1.0, 1.0),
			glm::vec4(0.0, 0.0, -1.0, 1.0),
			glm::vec4(0.0, 0.0, -1.0, 1.0),
			glm::vec4(0.0, 0.0, -1.0, 1.0),
			glm::vec4(0.0, 0.0, -1.0, 1.0),
			glm::vec4(0.0, 0.0, -1.0, 1.0)
		};
		csX75::Model* cuboid = new csX75::Model(boundary,num_vertices,wall,GL_TRIANGLES,v_positions,v_colors,v_normals,v_texture,sizeof(v_positions),sizeof(v_colors),sizeof(v_normals),sizeof(v_texture));
		cuboid->change_parameters(tx,ty,tz,rx,ry,rz);
		cuboid->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	}
	// x=0 plane(wallmap)
	if(true){
		int num_vertices = 6;
		glm::vec4 v_positions[6] = {
			glm::vec4(0.0, 0.0, 0, 1.0),
			glm::vec4(0.0, breadth, 0, 1.0),
			glm::vec4(0.0, breadth, width, 1.0),
			glm::vec4(0.0, breadth, width, 1.0),
			glm::vec4(0.0, 0.0, width, 1.0),
			glm::vec4(0.0, 0.0, 0.0, 1.0)
		};
		glm::vec2 v_texture[6] = {
			glm::vec2(0.0, 0.0),
			glm::vec2(1.0, 0.0),
			glm::vec2(1.0, 1.0),
			glm::vec2(1.0, 1.0),
			glm::vec2(0.0, 1.0),
			glm::vec2(0.0, 0.0)
		};
		glm::vec4 v_colors[6] = {
			white,
			white,
			white,
			white,
			white,
			white
		};
		glm::vec4 v_normals[6] = {
			glm::vec4(1.0, 0.0, 0.0, 1.0),
			glm::vec4(1.0, 0.0, 0.0, 1.0),
			glm::vec4(1.0, 0.0, 0.0, 1.0),
			glm::vec4(1.0, 0.0, 0.0, 1.0),
			glm::vec4(1.0, 0.0, 0.0, 1.0),
			glm::vec4(1.0, 0.0, 0.0, 1.0)
		};
		csX75::Model* cuboid = new csX75::Model(boundary,num_vertices,wall,GL_TRIANGLES,v_positions,v_colors,v_normals,v_texture,sizeof(v_positions),sizeof(v_colors),sizeof(v_normals),sizeof(v_texture));
		cuboid->change_parameters(tx,ty,tz,rx,ry,rz);
		cuboid->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	}
	// x=length plane(wallmap)
	if(true){
		int num_vertices = 6;
		glm::vec4 v_positions[6] = {
			glm::vec4(length, 0.0, 0, 1.0),
			glm::vec4(length, 0.0, width, 1.0),
			glm::vec4(length, breadth, width, 1.0),
			glm::vec4(length, breadth, width, 1.0),
			glm::vec4(length, breadth, 0.0, 1.0),
			glm::vec4(length, 0.0, 0.0, 1.0)
		};
		glm::vec2 v_texture[6] = {
			glm::vec2(0.0, 0.0),
			glm::vec2(0.0, 1.0),
			glm::vec2(1.0, 1.0),
			glm::vec2(1.0, 1.0),
			glm::vec2(1.0, 0.0),
			glm::vec2(0.0, 0.0)
		};
		glm::vec4 v_colors[6] = {
			white,
			white,
			white,
			white,
			white,
			white
		};
		glm::vec4 v_normals[6] = {
			glm::vec4(-1.0, 0.0, 0.0, 1.0),
			glm::vec4(-1.0, 0.0, 0.0, 1.0),
			glm::vec4(-1.0, 0.0, 0.0, 1.0),
			glm::vec4(-1.0, 0.0, 0.0, 1.0),
			glm::vec4(-1.0, 0.0, 0.0, 1.0),
			glm::vec4(-1.0, 0.0, 0.0, 1.0)
		};
		csX75::Model* cuboid = new csX75::Model(boundary,num_vertices,wall,GL_TRIANGLES,v_positions,v_colors,v_normals,v_texture,sizeof(v_positions),sizeof(v_colors),sizeof(v_normals),sizeof(v_texture));
		cuboid->change_parameters(tx,ty,tz,rx,ry,rz);
		cuboid->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	}
	// y=0 plane(wallmap)
	if(true){
		int num_vertices = 6;
		glm::vec4 v_positions[6] = {
			glm::vec4(0.0, 0.0, 0, 1.0),
			glm::vec4(0.0, 0.0, width, 1.0),
			glm::vec4(length, 0.0, width, 1.0),
			glm::vec4(length, 0.0, width, 1.0),
			glm::vec4(length, 0.0, 0.0, 1.0),
			glm::vec4(0.0, 0.0, 0.0, 1.0)
		};
		glm::vec2 v_texture[6] = {
			glm::vec2(0.0, 0.0),
			glm::vec2(1.0, 0.0),
			glm::vec2(1.0, 1.0),
			glm::vec2(1.0, 1.0),
			glm::vec2(0.0, 1.0),
			glm::vec2(0.0, 0.0)
		};
		glm::vec4 v_colors[6] = {
			white,
			white,
			white,
			white,
			white,
			white
		};
		glm::vec4 v_normals[6] = {
			glm::vec4(0.0, 1.0, 0.0, 1.0),
			glm::vec4(0.0, 1.0, 0.0, 1.0),
			glm::vec4(0.0, 1.0, 0.0, 1.0),
			glm::vec4(0.0, 1.0, 0.0, 1.0),
			glm::vec4(0.0, 1.0, 0.0, 1.0),
			glm::vec4(0.0, 1.0, 0.0, 1.0)
		};
		csX75::Model* cuboid = new csX75::Model(boundary,num_vertices,ground,GL_TRIANGLES,v_positions,v_colors,v_normals,v_texture,sizeof(v_positions),sizeof(v_colors),sizeof(v_normals),sizeof(v_texture));
		cuboid->change_parameters(tx,ty,tz,rx,ry,rz);
		cuboid->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	}
	// y=breadth plane(wallmap)
	if(true){
		int num_vertices = 6;
		glm::vec4 v_positions[6] = {
			glm::vec4(0.0, breadth, 0.0, 1.0),
			glm::vec4(length, breadth, 0.0, 1.0),
			glm::vec4(length, breadth, width, 1.0),
			glm::vec4(length, breadth, width, 1.0),
			glm::vec4(0.0, breadth, width, 1.0),
			glm::vec4(0.0, breadth, 0.0, 1.0)
		};
		glm::vec2 v_texture[6] = {
			glm::vec2(0.0, 0.0),
			glm::vec2(0.0, 1.0),
			glm::vec2(1.0, 1.0),
			glm::vec2(1.0, 1.0),
			glm::vec2(1.0, 0.0),
			glm::vec2(0.0, 0.0)
		};
		glm::vec4 v_colors[6] = {
			white,
			white,
			white,
			white,
			white,
			white
		};
		glm::vec4 v_normals[6] = {
			glm::vec4(0.0, -1.0, 0.0, 1.0),
			glm::vec4(0.0, -1.0, 0.0, 1.0),
			glm::vec4(0.0, -1.0, 0.0, 1.0),
			glm::vec4(0.0, -1.0, 0.0, 1.0),
			glm::vec4(0.0, -1.0, 0.0, 1.0),
			glm::vec4(0.0, -1.0, 0.0, 1.0)
		};
		csX75::Model* cuboid = new csX75::Model(boundary,num_vertices,sky,GL_TRIANGLES,v_positions,v_colors,v_normals,v_texture,sizeof(v_positions),sizeof(v_colors),sizeof(v_normals),sizeof(v_texture));
		cuboid->change_parameters(tx,ty,tz,rx,ry,rz);
		cuboid->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	}
	return boundary;
}
// generates and adds a new cone
csX75::Model* add_cone(csX75::Model* parent,GLuint tex,double height, double vangle, int cparts, int hparts, GLfloat tx, GLfloat ty, GLfloat tz, GLfloat rx, GLfloat ry, GLfloat rz)
{
	float thetha = 2.0*PI/float(cparts);
	float delta_z = height/float(hparts);

	float l;
	int num_vertices=0;
	float angle,hgt;
	// count final number of vertices
	for(hgt=0.0;hgt<=height;hgt+=delta_z){
		for(angle=0;angle<=2*PI;angle+=thetha){
			num_vertices+=6;
		}
	}
	for(angle=0.0; angle<=2*PI; angle+=thetha){
		num_vertices+=3;
	}
	glm::vec4 v_positions[num_vertices];
	glm::vec4 v_colors[num_vertices];
	glm::vec4 v_normals[num_vertices];
	glm::vec2 v_texture[num_vertices];
	double rscale = tan(vangle);
	double rsin = sin(vangle);
	double rcos = cos(vangle);
	int tri_idx=0;
	for(hgt=0.0;hgt<=height;hgt+=delta_z){
		for(angle=0;angle<=2*PI;angle+=thetha){
			float x = hgt*rscale*cos(angle);
			float y = hgt*rscale*sin(angle);
			float l=hgt+delta_z;
			if(l>(height)){
				l = height;
			}
			v_colors[tri_idx] = white; v_positions[tri_idx] = glm::vec4(hgt*rscale*cos(angle),hgt*rscale*sin(angle),hgt,1.0);
			v_normals[tri_idx] = glm::vec4(cos(angle)*rcos,sin(angle)*rcos,rsin,1.0); tri_idx++;
			v_colors[tri_idx] = white; v_positions[tri_idx] = glm::vec4(hgt*rscale*cos(angle+thetha),hgt*rscale*sin(angle+thetha),hgt,1.0);
			v_normals[tri_idx] = glm::vec4(cos(angle+thetha)*rcos,sin(angle+thetha)*rcos,rsin,1.0); tri_idx++;
			v_colors[tri_idx] = white; v_positions[tri_idx] = glm::vec4(l*rscale*cos(angle),l*rscale*sin(angle),l,1.0);
			v_normals[tri_idx] = glm::vec4(cos(angle)*rcos,sin(angle)*rcos,rsin,1.0); tri_idx++;
			v_colors[tri_idx] = white; v_positions[tri_idx] = glm::vec4(l*rscale*cos(angle),l*rscale*sin(angle),l,1.0);
			v_normals[tri_idx] = glm::vec4(cos(angle)*rcos,sin(angle)*rcos,rsin,1.0); tri_idx++;
			v_colors[tri_idx] = white; v_positions[tri_idx] = glm::vec4(l*rscale*cos(angle+thetha),l*rscale*sin(angle+thetha),l,1.0);
			v_normals[tri_idx] = glm::vec4(cos(angle+thetha)*rcos,sin(angle+thetha)*rcos,rsin,1.0); tri_idx++;
			v_colors[tri_idx] = white; v_positions[tri_idx] = glm::vec4(hgt*rscale*cos(angle+thetha),hgt*rscale*sin(angle+thetha),hgt,1.0);
			v_normals[tri_idx] = glm::vec4(cos(angle+thetha)*rcos,sin(angle+thetha)*rcos,rsin,1.0); tri_idx++;
		}
	}
	// upper circle 
	for(angle=0.0; angle<=2*PI; angle+=thetha){
		float x = height*rscale*cos(angle);
		float y = height*rscale*sin(angle);
		float z = height;
		glm::vec4 pt(x, y, z, 1.0);

		v_colors[tri_idx] = white; v_positions[tri_idx] = pt; 
		v_normals[tri_idx] = glm::vec4(0,0,1,1.0); tri_idx++;

		x = height*rscale*cos(angle+thetha);
		y = height*rscale*sin(angle+thetha);
		pt = glm::vec4(x, y, z, 1.0);

		v_colors[tri_idx] = white; v_positions[tri_idx] = pt; 
		v_normals[tri_idx] = glm::vec4(0,0,1,1.0); tri_idx++;

		pt = glm::vec4(0.0,0.0,z,1.0);
		
		v_colors[tri_idx] = white; v_positions[tri_idx] = pt; 
		v_normals[tri_idx] = glm::vec4(0,0,1,1.0); tri_idx++;
	}
	csX75::Model* cone = new csX75::Model(parent,num_vertices,tex,GL_TRIANGLE_STRIP,v_positions,v_colors,v_normals,v_texture,sizeof(v_positions),sizeof(v_colors),sizeof(v_normals),sizeof(v_texture));
	cone->change_parameters(tx,ty,tz,rx,ry,rz);
	return cone;
}


void initialize(void){
	// Load shaders and use the resulting shader program
	std::string vertex_shader_file("vshader.glsl");
	std::string fragment_shader_file("fshader.glsl");

	std::vector<GLuint> shaderList;
	shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
	shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

	shaderProgram = csX75::CreateProgramGL(shaderList);
	glUseProgram( shaderProgram );

	// getting the attributes from the shader program
	vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
	vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
	vNormal = glGetAttribLocation( shaderProgram, "vNormal" ); 
	texCoord = glGetAttribLocation( shaderProgram, "texCoord" ); 
	uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");
	ModelMatrix = glGetUniformLocation(shaderProgram,"ModelMatrix");
	normalMatrix =  glGetUniformLocation( shaderProgram, "normalMatrix");
	viewMatrix = glGetUniformLocation( shaderProgram, "viewMatrix");
	Mdiffuse = glGetUniformLocation( shaderProgram, "Mdiffuse");
	Mspecular = glGetUniformLocation( shaderProgram, "Mspecular");
	Mambient = glGetUniformLocation( shaderProgram, "Mambient");
	Mshine = glGetUniformLocation( shaderProgram, "Mshine");
	Light1 = glGetUniformLocation( shaderProgram, "Light1");
	Light2 = glGetUniformLocation( shaderProgram, "Light2");
	SpotLight = glGetUniformLocation( shaderProgram, "SpotLight");

	// loading textures
	GLuint face=LoadTexture("images/face.bmp",512,512);
	GLuint shirt=LoadTexture("images/body.bmp",512,512);
        GLuint skin=LoadTexture("images/skin.bmp",512,512);
	GLuint jeans=LoadTexture("images/jeans.bmp",512,512);
	GLuint evehead=LoadTexture("images/eve2.bmp",1082,375);
	GLuint evebody=LoadTexture("images/evebackhead.bmp",305,225);

// humanoid model creation.
	hmn_centre = add_sphere(NULL,-1,CENTRE_RADIUS,50,50,-5,-3,-5,0,0,0);
	hmn_centre->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	hmn_torso = add_cuboid(hmn_centre,shirt,BODY_WIDTH,BODY_HEIGHT,BODY_DEPTH,-(BODY_WIDTH)/2.0,0,-(BODY_DEPTH)/2.0,0,0,0);
	hmn_torso->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	hmn_ulegL = add_cylinder(hmn_centre,jeans,LEG_RADIUS,ULEG_LENGTH,60,1000,-(BODY_WIDTH/2.0)+(LEG_RADIUS),0,0,90,0,0);
	hmn_ulegL->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	hmn_ulegR = add_cylinder(hmn_centre,jeans,LEG_RADIUS,ULEG_LENGTH,60,1000,(BODY_WIDTH/2.0)-(LEG_RADIUS),0,0,90,0,0);
	hmn_ulegR->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	hmn_slegL = add_sphere(hmn_ulegL,skin,LEG_RADIUS,50,50,0,0,ULEG_LENGTH,0,0,0);
	hmn_slegL->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	hmn_slegR = add_sphere(hmn_ulegR,skin,LEG_RADIUS,50,50,0,0,ULEG_LENGTH,0,0,0);
	hmn_slegR->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	hmn_llegL = add_cylinder(hmn_slegL,skin,LEG_RADIUS,LLEG_LENGTH,60,1000,0,0,0,0,0,0);
	hmn_llegL->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	hmn_llegR = add_cylinder(hmn_slegR,skin,LEG_RADIUS,LLEG_LENGTH,60,1000,0,0,0,0,0,0);
	hmn_llegR->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	hmn_shandL = add_sphere(hmn_torso,skin,HAND_RADIUS,50,50,-(HAND_RADIUS),BODY_HEIGHT,(BODY_DEPTH/2.0),90,0,0);
	hmn_shandL->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	hmn_shandR = add_sphere(hmn_torso,skin,HAND_RADIUS,50,50,BODY_WIDTH+HAND_RADIUS,BODY_HEIGHT,(BODY_DEPTH/2.0),90,0,0);
	hmn_shandR->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	hmn_uarmL = add_cylinder(hmn_shandL,skin,HAND_RADIUS,UARM_LENGTH,60,1000,0,0,0,0,0,0);
	hmn_uarmL->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	hmn_uarmR = add_cylinder(hmn_shandR,skin,HAND_RADIUS,UARM_LENGTH,60,1000,0,0,0,0,0,0);
	hmn_uarmR->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	hmn_marmL = add_sphere(hmn_uarmL,skin,HAND_RADIUS,50,50,0,0,UARM_LENGTH,0,0,0);
	hmn_marmL->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	hmn_marmR = add_sphere(hmn_uarmR,skin,HAND_RADIUS,50,50,0,0,UARM_LENGTH,0,0,0);
	hmn_marmR->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	hmn_larmL = add_cylinder(hmn_marmL,skin,HAND_RADIUS,LARM_LENGTH,60,1000,0,0,0,0,0,0);
	hmn_larmL->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	hmn_larmR = add_cylinder(hmn_marmR,skin,HAND_RADIUS,LARM_LENGTH,60,1000,0,0,0,0,0,0);
	hmn_larmR->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	hmn_neck = add_cylinder(hmn_torso,skin,NECK_RADIUS,NECK_LENGTH,32,1000,(BODY_WIDTH/2.0),BODY_HEIGHT,(BODY_DEPTH/2.0),-90,0,0);
	hmn_neck->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	hmn_head = add_sphere(hmn_neck,face,HEAD_RADIUS,50,50,0,0,NECK_LENGTH+(HEAD_RADIUS*0.75),-10,-180,70);
	hmn_head->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);
	curr_node = hmn_head;

//non-humanoid model creation
	nhmn_centre = add_sphere(NULL, -1,CENTRE_RADIUS,50,50,+3.5,0,-5,0,0,0);
	nhmn_centre->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);	
	nhmn_head = add_sphere(nhmn_centre,evehead,HEAD_RADIUS_1*1.5,50,50,0,(HEAD_RADIUS_1)*1.5,0,75,0,-10);
	nhmn_head->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);	
	nhmn_torso = add_cone(nhmn_centre,evebody,TORSO_HEIGHT_1,TORSO_VANGLE_1,30,100,0,-(TORSO_HEIGHT_1),0,-90,0,0);
	nhmn_torso->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);	
	nhmn_handL = add_cylinder(nhmn_centre,evebody,HAND_RADIUS_1,HAND_LENGTH_1,60,1000,-3,-1,0,-90,180,0);
	nhmn_handL->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);	
	nhmn_handR = add_cylinder(nhmn_centre,evebody,HAND_RADIUS_1,HAND_LENGTH_1,60,1000,+3,-1,0,-90,180,0);
	nhmn_handR->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);

//adding environment
	environment = add_boundaries(NULL,-1,50,20,50,-30,-10,-20,0,0,0);
	environment->set_material(hmn_diffuse,hmn_specular,hmn_ambient,hmn_shininess);

}


void renderGL(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	matrixStack.clear();

	//Creating the lookat and the up vectors for the camera
	c_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(c_xrot), glm::vec3(1.0f,0.0f,0.0f));
	c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_yrot), glm::vec3(0.0f,1.0f,0.0f));
	c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_zrot), glm::vec3(0.0f,0.0f,1.0f));

	glm::vec4 c_pos = glm::vec4(c_xpos,c_ypos,c_zpos, 1.0)*c_rotation_matrix;
	glm::vec4 c_up = glm::vec4(c_up_x,c_up_y,c_up_z, 1.0)*c_rotation_matrix;
	//Creating the lookat matrix
	lookat_matrix = glm::lookAt(glm::vec3(c_pos),glm::vec3(0.0),glm::vec3(c_up));

	//creating the projection matrix
	projection_matrix = glm::perspective(glm::radians(90.0),1.0,0.1,200.0);
		// projection_matrix = glm::frustum(-14.0, 14.0, -14.0, 14.0, 1.0, 20.0);
		// projection_matrix = glm::perspective(60.0, 1.0, 0.1, 100.0);

	view_matrix = projection_matrix*lookat_matrix;
	glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, glm::value_ptr(view_matrix));
	glUniform1f(Light1,light1);
	glUniform1f(Light2,light2);
	glUniform1f(SpotLight,spotlight);

	matrixStack.push_back(view_matrix);

	hmn_centre->render_tree();
	nhmn_centre->render_tree();
	environment->render_tree();
}

int main(int argc, char** argv)
{
	//! The pointer to the GLFW window
	GLFWwindow* window;

	//! Setting up the GLFW Error callback
	glfwSetErrorCallback(csX75::error_callback);

	//! Initialize GLFW
	if (!glfwInit())
		return -1;

	//We want OpenGL 4.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//This is for MacOSX - can be omitted otherwise
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	//We don't want the old OpenGL 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

	//! Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(512, 512, "CS475 Assignment 3", NULL, NULL);
	if (!window){
		glfwTerminate();
		return -1;
	}

	//! Make the window's context current 
	glfwMakeContextCurrent(window);

	//Initialize GLEW
	//Turn this on to get Shader based OpenGL
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err){
		//Problem: glewInit failed, something is seriously wrong.
		std::cerr<<"GLEW Init Failed : %s"<<std::endl;
	}

	//Print and see what context got enabled
	std::cout<<"Vendor: "<<glGetString (GL_VENDOR)<<std::endl;
	std::cout<<"Renderer: "<<glGetString (GL_RENDERER)<<std::endl;
	std::cout<<"Version: "<<glGetString (GL_VERSION)<<std::endl;
	std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl;

	//Keyboard Callback
	glfwSetKeyCallback(window, csX75::key_callback);
	//Framebuffer resize callback
	glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//Initialize GL state
	csX75::initGL();
	initialize();

	// Loop until the user closes the window
	while (glfwWindowShouldClose(window) == 0){

		// Render here
		renderGL();

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
