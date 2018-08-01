#ifndef _TOYS_HPP_
#define _TOYS_HPP_

// Defining the ESCAPE Key Code
#define ESCAPE 27
// Defining the DELETE Key Code
#define DELETE 127

#include <vector>
#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "model.hpp"

// Camera position and rotation Parameters
GLfloat c_xpos = 0.0, c_ypos = 0.0, c_zpos = 4.0;
GLfloat c_up_x = 0.0, c_up_y = 1.0, c_up_z = 0.0;
GLfloat c_xrot=0.0,c_yrot=0.0,c_zrot=0.0;

//Running variable to toggle culling on/off
bool enable_culling=true;
float light1 = 1, light2=1,spotlight=1;
//Running variable to toggle wireframe/solid modelling
bool solid=true;
//Enable/Disable perspective view
bool enable_perspective=true;
//Shader program attribs

csX75::Model* curr_node;

// material constants
glm::vec4 hmn_diffuse = glm::vec4(1.0, 1.0, 1.0, 1.0); 
glm::vec4 hmn_specular = glm::vec4(1.0, 1.0, 1.0, 1.0);
glm::vec4 hmn_ambient = glm::vec4(0.3, 0.3, 0.3, 1.0);
GLfloat hmn_shininess = 10.0;

csX75::Model* environment;
csX75::Model* hmn_centre, *hmn_torso, *hmn_ulegL, *hmn_ulegR, *hmn_slegL, *hmn_slegR, *hmn_llegL, *hmn_llegR, *hmn_shandL, *hmn_shandR, *hmn_uarmL, *hmn_uarmR, *hmn_marmL, *hmn_marmR, *hmn_larmL, *hmn_larmR, *hmn_neck, *hmn_head;
const double CENTRE_RADIUS = 0.0,BODY_WIDTH = 3.0,BODY_HEIGHT = 6.0,BODY_DEPTH = 1.2;
const double LEG_RADIUS = 0.5,ULEG_LENGTH = 3,LLEG_LENGTH = 3.5;
const double HAND_RADIUS = 0.5,UARM_LENGTH = 3,LARM_LENGTH = 3.2;
const double NECK_RADIUS = 0.7,NECK_LENGTH = 0.5,HEAD_RADIUS = 1.5;

csX75::Model* nhmn_centre, *nhmn_head, *nhmn_torso, *nhmn_handL, *nhmn_handR;
const double HEAD_RADIUS_1 = 1.5, HAND_RADIUS_1 = 0.3, HAND_LENGTH_1 = 4.0, TORSO_HEIGHT_1 = 5.0, TORSO_VANGLE_1 = 2.53073;  


//global matrix stack for hierarchical modelling
std::vector<glm::mat4> matrixStack;

#endif