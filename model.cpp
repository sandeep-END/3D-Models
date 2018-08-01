#include "model.hpp"
#include <iostream>

extern GLuint vPosition,vColor,vNormal,texCoord,uModelViewMatrix,ModelMatrix,viewMatrix,normalMatrix,Mdiffuse,Mspecular,Mambient,Mshine;
extern std::vector<glm::mat4> matrixStack;

namespace csX75
{

	Model::Model(Model* a_parent, GLuint num_v, GLuint tex, GLuint draw, glm::vec4* a_vertices, glm::vec4* a_colours, glm::vec4* a_normals, glm::vec2* a_textures, std::size_t v_size, std::size_t c_size, std::size_t n_size, std::size_t t_size){

		num_vertices = num_v;
		vertex_buffer_size = v_size;
		color_buffer_size = c_size;
		normal_buffer_size = n_size;
		texture_buffer_size = t_size;
		type = draw;

		mytex = tex;

		// initialize vao and vbo of the object;


		//Ask GL for a Vertex Attribute Objects (vao)
		glGenVertexArrays (1, &vao);
		//Ask GL for aVertex Buffer Object (vbo)
		glGenBuffers (1, &vbo);

		//bind them
		glBindVertexArray (vao);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);

		
		glBufferData (GL_ARRAY_BUFFER, vertex_buffer_size + color_buffer_size + normal_buffer_size + texture_buffer_size, NULL, GL_STATIC_DRAW);
		glBufferSubData( GL_ARRAY_BUFFER, 0, vertex_buffer_size, a_vertices );
		glBufferSubData( GL_ARRAY_BUFFER, vertex_buffer_size, color_buffer_size, a_colours );
		glBufferSubData( GL_ARRAY_BUFFER, vertex_buffer_size + color_buffer_size, normal_buffer_size, a_normals );
		glBufferSubData( GL_ARRAY_BUFFER, vertex_buffer_size + color_buffer_size + normal_buffer_size, texture_buffer_size, a_textures );

		//setup the vertex array as per the shader
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

		glEnableVertexAttribArray( vColor );
		glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertex_buffer_size));

		glEnableVertexAttribArray( vNormal );
		glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertex_buffer_size+color_buffer_size));

		glEnableVertexAttribArray( texCoord );
		glVertexAttribPointer( texCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertex_buffer_size+color_buffer_size+normal_buffer_size));


		// set parent

		if(a_parent == NULL){
			parent = NULL;
		}
		else{
			parent = a_parent;
			parent->add_child(this);
		}

		//initial parameters are set to 0;

		tx=ty=tz=rx=ry=rz=0;

		diffuse = glm::vec4(0.0);
		specular = glm::vec4(0.0);
		ambient = glm::vec4(0.0);
		shininess = 0.0;

		update_matrices();
	}

	void Model::update_matrices(){

		rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rx), glm::vec3(1.0f,0.0f,0.0f));
		rotation = glm::rotate(rotation, glm::radians(ry), glm::vec3(0.0f,1.0f,0.0f));
		rotation = glm::rotate(rotation, glm::radians(rz), glm::vec3(0.0f,0.0f,1.0f));

		translation = glm::translate(glm::mat4(1.0f),glm::vec3(tx,ty,tz));


	}

	void Model::add_child(Model* a_child){
		children.push_back(a_child);

	}

	void Model::set_material(glm::vec4 diff, glm::vec4 spec, glm::vec4 amb, GLfloat shin){
		diffuse = diff;
		specular = spec;
		ambient = amb;
		shininess = shin;
	}


	void Model::change_parameters(GLfloat atx, GLfloat aty, GLfloat atz, GLfloat arx, GLfloat ary, GLfloat arz){
		tx = atx;
		ty = aty;
		tz = atz;
		rx = arx;
		ry = ary;
		rz = arz;

		update_matrices();
	}

	void Model::render(){

		//matrixStack multiply
		glm::mat4* ms_mult = multiply_stack(matrixStack);
		glm::mat4* ms_mult2 = multiply_stack2(matrixStack);
		glm::mat3 normal_matrix = glm::transpose (glm::inverse(glm::mat3(*ms_mult)));
		glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));
		glUniformMatrix4fv(ModelMatrix,1,GL_FALSE,glm::value_ptr(*ms_mult2));
		glUniformMatrix3fv(normalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));
		glUniform4fv(Mdiffuse,1,glm::value_ptr(diffuse));
		glUniform4fv(Mspecular,1,glm::value_ptr(specular));
		glUniform4fv(Mambient,1,glm::value_ptr(ambient));
		glUniform1f(Mshine,shininess);
		glBindVertexArray (vao);
		glBindTexture(GL_TEXTURE_2D, mytex);
		glDrawArrays(type, 0, num_vertices);

		// for memory 
		delete ms_mult;

	}

	void Model::render_tree(){
		
		matrixStack.push_back(translation);
		matrixStack.push_back(rotation);

		render();
		for(int i=0;i<children.size();i++){
			children[i]->render_tree();
		}
		matrixStack.pop_back();
		matrixStack.pop_back();

	}

	void Model::inc_rx(){
		rx+=3;
		update_matrices();
	}


	void Model::inc_ry(){
		ry+=3;
		update_matrices();
	}

	void Model::inc_rz(){
		rz+=3;
		update_matrices();
	}

	void Model::dec_rx(){
		rx-=3;
		update_matrices();
	}

	void Model::dec_ry(){
		ry-=3;
		update_matrices();
	}

	void Model::dec_rz(){
		rz-=3;
		update_matrices();
	}

	void Model::inc_tx(){
		tx+=0.2;
		update_matrices();
	}


	void Model::inc_ty(){
		ty+=0.2;
		update_matrices();
	}

	void Model::inc_tz(){
		tz+=0.2;
		update_matrices();
	}

	void Model::dec_tx(){
		tx-=0.2;
		update_matrices();
	}

	void Model::dec_ty(){
		ty-=0.2;
		update_matrices();
	}

	void Model::dec_tz(){
		tz-=0.2;
		update_matrices();
	}


	glm::mat4* multiply_stack(std::vector<glm::mat4> matStack){
		glm::mat4* mult;
		mult = new glm::mat4(1.0f);
	
		for(int i=0;i<matStack.size();i++){
			*mult = (*mult) * matStack[i];
		}	

		return mult;
	}

	glm::mat4* multiply_stack2(std::vector<glm::mat4> matStack){
		glm::mat4* mult;
		mult = new glm::mat4(1.0f);
	
		for(int i=1;i<matStack.size();i++){
			*mult = (*mult) * matStack[i];
		}	

		return mult;
	}

}