#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


#include "gl_framework.hpp"


namespace csX75	 { 

	// A simple class that represents a node in the hierarchy tree
	class Model {
		//glm::vec4 * vertices;
		//glm::vec4 * colors;
		GLfloat tx,ty,tz,rx,ry,rz;

		std::size_t vertex_buffer_size;
		std::size_t color_buffer_size;
		std::size_t normal_buffer_size;
		std::size_t texture_buffer_size;

		glm::vec4 diffuse; 
		glm::vec4 specular;
		glm::vec4 ambient;
		GLfloat shininess;

		GLuint num_vertices,mytex;
		GLuint type;
		GLuint vao,vbo;


		glm::mat4 rotation;
		glm::mat4 translation;
		
		std::vector<Model*> children;
		Model* parent;

		void update_matrices();

	  public:
		Model (Model*, GLuint, GLuint, GLuint, glm::vec4*,  glm::vec4*,  glm::vec4*, glm::vec2*, std::size_t, std::size_t, std::size_t, std::size_t);

		void add_child(Model*);
		void render();
		void set_material(glm::vec4, glm::vec4, glm::vec4, GLfloat);
		void change_parameters(GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat);
		void render_tree();
		void inc_rx();
		void inc_ry();
		void inc_rz();
		void dec_rx();
		void dec_ry();
		void dec_rz();
		void inc_tx();
		void inc_ty();
		void inc_tz();
		void dec_tx();
		void dec_ty();
		void dec_tz();
	};

	glm::mat4* multiply_stack(std::vector <glm::mat4> );
	glm::mat4* multiply_stack2(std::vector <glm::mat4> );
};	

#endif