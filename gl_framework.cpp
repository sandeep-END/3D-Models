#include "gl_framework.hpp"
#include "model.hpp"

extern GLfloat c_xrot,c_yrot,c_zrot;
extern GLfloat c_xpos,c_ypos,c_zpos;

extern bool enable_perspective;
extern float light1,light2,spotlight;
extern csX75::Model* hmn_centre, *hmn_torso, *hmn_ulegL,
 *hmn_ulegR, *hmn_slegL, *hmn_slegR, *hmn_llegL, *hmn_llegR,
  *hmn_shandL, *hmn_shandR, *hmn_uarmL, *hmn_uarmR, *hmn_marmL,
   *hmn_marmR, *hmn_larmL, *hmn_larmR, *hmn_neck, *hmn_head, *curr_node,
    *nhmn_centre, *nhmn_head, *nhmn_torso, *nhmn_handL, *nhmn_handR;
namespace csX75
{
  //! Initialize GL State
  void initGL(void)
  {
    //Set framebuffer clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //Set depth buffer furthest depth
    glClearDepth(1.0);
    //Set depth test to less-than
    glDepthFunc(GL_LESS);
    //Enable depth testing
    glEnable(GL_DEPTH_TEST);
  }
  
  //!GLFW Error Callback
  void error_callback(int error, const char* description)
  {
    std::cerr<<description<<std::endl;
  }
  
  //!GLFW framebuffer resize callback
  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    //!Resize the viewport to fit the window size - draw to entire window
    glViewport(0, 0, width, height);
  }
  
  //!GLFW keyboard callback
  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    //!Close the window if the ESC key was pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
    else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
      curr_node = hmn_ulegR;
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
      curr_node = hmn_ulegL;
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
      curr_node = hmn_slegR;
    else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
      curr_node = hmn_slegL;
    else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
      curr_node = hmn_torso;
    else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
      curr_node = hmn_shandR;
    else if (key == GLFW_KEY_7 && action == GLFW_PRESS)
      curr_node = hmn_shandL;
    else if (key == GLFW_KEY_8 && action == GLFW_PRESS)
      curr_node = hmn_marmR;
    else if (key == GLFW_KEY_9 && action == GLFW_PRESS)
      curr_node = hmn_marmL;
    else if (key == GLFW_KEY_0 && action == GLFW_PRESS)
      curr_node = hmn_head;
    else if (key == GLFW_KEY_C && action == GLFW_PRESS)
      curr_node = hmn_centre;
    else if (key == GLFW_KEY_V && action == GLFW_PRESS)
      curr_node = nhmn_centre;
    else if (key == GLFW_KEY_B && action == GLFW_PRESS)
      curr_node = nhmn_head;
    else if (key == GLFW_KEY_N && action == GLFW_PRESS)
      curr_node = nhmn_handR;
    else if (key == GLFW_KEY_M && action == GLFW_PRESS)
      curr_node = nhmn_handL;
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS){
      if(curr_node!=hmn_slegR&&curr_node!=hmn_slegL&&curr_node!=hmn_torso&&curr_node!=hmn_marmL&&curr_node!=hmn_marmR)
      curr_node->dec_ry();
    }
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
      if(curr_node!=hmn_slegR&&curr_node!=hmn_slegL&&curr_node!=hmn_torso&&curr_node!=hmn_marmL&&curr_node!=hmn_marmR)
      curr_node->inc_ry();
    }
    else if (key == GLFW_KEY_UP && action == GLFW_PRESS){
      curr_node->dec_rx();
    }
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
      curr_node->inc_rx();
    }
    else if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS){
      if(curr_node!=hmn_slegR&&curr_node!=hmn_slegL&&curr_node!=hmn_torso&&curr_node!=hmn_marmL&&curr_node!=hmn_marmR)
      curr_node->dec_rz();
    }
    else if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS){
      if(curr_node!=hmn_slegR&&curr_node!=hmn_slegL&&curr_node!=hmn_torso&&curr_node!=hmn_marmL&&curr_node!=hmn_marmR)
      curr_node->inc_rz();
    }
    else if (key == GLFW_KEY_F && action == GLFW_PRESS){
      if(curr_node==hmn_centre||curr_node==nhmn_centre)
      curr_node->dec_ty();
    }
    else if (key == GLFW_KEY_H && action == GLFW_PRESS){
      if(curr_node==hmn_centre||curr_node==nhmn_centre)
      curr_node->inc_ty();
    }
    else if (key == GLFW_KEY_T && action == GLFW_PRESS){
      if(curr_node==hmn_centre||curr_node==nhmn_centre)
      curr_node->dec_tx();
    }
    else if (key == GLFW_KEY_G && action == GLFW_PRESS){
      if(curr_node==hmn_centre||curr_node==nhmn_centre)
      curr_node->inc_tx();
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS){
      if(curr_node==hmn_centre||curr_node==nhmn_centre)
      curr_node->dec_tz();
    }
    else if (key == GLFW_KEY_Y && action == GLFW_PRESS){
      if(curr_node==hmn_centre||curr_node==nhmn_centre)
      curr_node->inc_tz();
    }
    else if (key == GLFW_KEY_A  && action == GLFW_PRESS)
      c_ypos -= 0.2;
    else if (key == GLFW_KEY_D  && action == GLFW_PRESS)
      c_ypos += 0.2;
    else if (key == GLFW_KEY_W  && action == GLFW_PRESS)
      c_xpos -= 0.2;
    else if (key == GLFW_KEY_S  && action == GLFW_PRESS)
      c_xpos += 0.2;        
    else if (key == GLFW_KEY_Q  && action == GLFW_PRESS)
      c_zpos -= 0.2;
    else if (key == GLFW_KEY_E  && action == GLFW_PRESS)
      c_zpos += 0.2;  
    else if (key == GLFW_KEY_J  && action == GLFW_PRESS)
      c_yrot -= 2;
    else if (key == GLFW_KEY_L  && action == GLFW_PRESS)
      c_yrot += 2;
    else if (key == GLFW_KEY_I  && action == GLFW_PRESS)
      c_xrot -= 2;
    else if (key == GLFW_KEY_K  && action == GLFW_PRESS)
      c_xrot += 2;        
    else if (key == GLFW_KEY_U  && action == GLFW_PRESS)
      c_zrot -= 2;
    else if (key == GLFW_KEY_O  && action == GLFW_PRESS)
      c_zrot += 2;  
    else if (key == GLFW_KEY_P && action == GLFW_PRESS)
      spotlight = 1 - spotlight;
    else if (key == GLFW_KEY_X  && action == GLFW_PRESS)
      light1 = 1 - light1;
    else if (key == GLFW_KEY_Z  && action == GLFW_PRESS)
      light2 = 1 - light2;
  }
};
