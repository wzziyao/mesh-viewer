// Bryn Mawr College, alinen, 2020
//

#include "AGL.h"
#include "AGLM.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;
using namespace glm;

glm::vec3 lookfrom;
glm::vec3 lookat = glm::vec3(0.0f, 0.0f, 0.0f);
float azimuth, elevation, dist;
bool zoom = false, mouse = false;
float old_x, old_y;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
   {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
   }
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   // Prevent a divide by zero
   if(height == 0) height = 1;
	
   // Set Viewport to window dimensions
   glViewport(0, 0, width, height);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
   double xpos, ypos;
   glfwGetCursorPos(window, &xpos, &ypos);

   // TODO: CAmera controls

   int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
   if (state == GLFW_PRESS)
   {
      mouse = true;
      int keyPress = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
      if (keyPress == GLFW_PRESS) {
         zoom = true;
      }
   }
   else if (state == GLFW_RELEASE)
   {
      zoom = false;
      mouse = false;
   }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
   // TODO: CAmera controls
   if (mouse) {
      if (zoom) {
         float change = (float)ypos - old_y;
         dist += change;
      } else {
         float x_change = (float)xpos - old_x;
         float y_change = (float)ypos - old_y;
         azimuth += x_change;
         elevation += y_change;
      }
      old_x = (float)xpos;
      old_y = (float)ypos;
   }
}

static void PrintShaderErrors(GLuint id, const std::string label)
{
   std::cerr << label << " failed\n";
   GLint logLen;
   glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLen);
   if (logLen > 0)
   {
      char* log = (char*)malloc(logLen);
      GLsizei written;
      glGetShaderInfoLog(id, logLen, &written, log);
      std::cerr << "Shader log: " << log << std::endl;
      free(log);
   }
}

static std::string LoadShaderFromFile(const std::string& fileName)
{
   std::ifstream file(fileName);
   if (!file)
   {
      std::cout << "Cannot load file: " << fileName << std::endl;
      return "";
   }

   std::stringstream code;
   code << file.rdbuf();
   file.close();

   return code.str();
}

int main(int argc, char** argv)
{
   GLFWwindow* window;

   if (!glfwInit())
   {
      return -1;
   }

   // Explicitly ask for a 4.0 context 
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   /* Create a windowed mode window and its OpenGL context */
   window = glfwCreateWindow(500, 500, "Hello World", NULL, NULL);
   if (!window)
   {
      glfwTerminate();
      return -1;
   }

   // Make the window's context current 
   glfwMakeContextCurrent(window);

   glfwSetKeyCallback(window, key_callback);
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
   glfwSetMouseButtonCallback(window, mouse_button_callback);
   glfwSetScrollCallback(window, scroll_callback);
   glfwSetCursorPosCallback(window, cursor_position_callback);

#ifndef APPLE
   if (glewInit() != GLEW_OK)
   {
      return -1;
   }
#endif

   glEnable(GL_DEPTH_TEST);
   glDisable(GL_CULL_FACE);
   glClearColor(0, 0, 0, 1);

   const float positions[] =
   {
      1.0, -1.0, 0.5,
     -1.0, -1.0, 0.5,
      0.0, 1.0,  0.5,
      -1.0, -1.0, 0.5,
      0.0, -1.0, -0.5,
      0.0, 1.0, 0.5,
      0.0,-1.0,-0.5,
      1.0, -1.0, 0.5,
      0.0, 1.0, 0.5
   };

   const float normals[] =
   {
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.67f, -0.33f, 0.67f,
      0.67f, -0.33f, 0.67f,
      0.67f, -0.33f, 0.67f,
      -0.67f, -0.33f, 0.67f,
      -0.67f, -0.33f, 0.67f,
      -0.67f, -0.33f, 0.67f
   };

   const unsigned int indices[] =
   {
      0, 1, 2, 3, 4, 5, 6, 7, 8
   };

   int numTriangles = 3;

   GLuint vboPosId;
   glGenBuffers(1, &vboPosId);
   glBindBuffer(GL_ARRAY_BUFFER, vboPosId);
   glBufferData(GL_ARRAY_BUFFER, numTriangles * 3 * 3 * sizeof(float), positions, GL_STATIC_DRAW);

   GLuint vboNormalId;
   glGenBuffers(1, &vboNormalId);
   glBindBuffer(GL_ARRAY_BUFFER, vboNormalId);
   glBufferData(GL_ARRAY_BUFFER, numTriangles * 3 * 3 * sizeof(float), normals, GL_STATIC_DRAW);

   GLuint elementbuffer;
   glGenBuffers(1, &elementbuffer);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, numTriangles * 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

   GLuint vaoId;
   glGenVertexArrays(1, &vaoId);
   glBindVertexArray(vaoId);

   glEnableVertexAttribArray(0); // 0 -> Sending VertexPositions to array #0 in the active shader
   glBindBuffer(GL_ARRAY_BUFFER, vboPosId); // always bind before setting data
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

   glEnableVertexAttribArray(1); // 1 -> Sending Normals to array #1 in the active shader
   glBindBuffer(GL_ARRAY_BUFFER, vboNormalId); // always bind before setting data
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

   GLint result;
   std::string vertexShader = LoadShaderFromFile("../shaders/unlit.vs");
   const char* vertexShaderRaw = vertexShader.c_str();
   GLuint vshaderId = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vshaderId, 1, &vertexShaderRaw, NULL);
   glCompileShader(vshaderId);
   glGetShaderiv(vshaderId, GL_COMPILE_STATUS, &result);
   if (result == GL_FALSE)
   {
      PrintShaderErrors(vshaderId, "Vertex shader");
      return -1;
   }

   std::string fragmentShader = LoadShaderFromFile("../shaders/unlit.fs");
   const char* fragmentShaderRaw = fragmentShader.c_str();
   GLuint fshaderId = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fshaderId, 1, &fragmentShaderRaw, NULL);
   glCompileShader(fshaderId);
   glGetShaderiv(fshaderId, GL_COMPILE_STATUS, &result);
   if (result == GL_FALSE)
   {
      PrintShaderErrors(fshaderId, "Fragment shader");
      return -1;
   }

   GLuint shaderId = glCreateProgram();
   glAttachShader(shaderId, vshaderId);
   glAttachShader(shaderId, fshaderId);
   glLinkProgram(shaderId);
   glGetShaderiv(shaderId, GL_LINK_STATUS, &result);
   if (result == GL_FALSE)
   {
      PrintShaderErrors(shaderId, "Shader link");
      return -1;
   }

   glUseProgram(shaderId);

   lookfrom = glm::vec3(0.0f, 0.0f, 5.0f);
   azimuth = 0.0f;
   elevation = 0.0f;
   dist = 5.0f;
   old_x = 0.0f;
   old_y = 0.0f;

   GLuint matrixParam = glGetUniformLocation(shaderId, "mvp");
   glm::mat4 transform(1.0);
   glm::mat4 projection = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 10.0f);
   // glm::mat4 camera = glm::lookAt(glm::vec3(0,0,5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

   // Loop until the user closes the window 
   while (!glfwWindowShouldClose(window))
   {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffers

      // float dx = 0.1 * sin(glfwGetTime());
      // transform = glm::translate(glm::mat4(1.0), glm::vec3(dx, 0, 0));
      // glm::mat4 mvp = projection * camera * transform;
      // glUniformMatrix4fv(matrixParam, 1, GL_FALSE, &mvp[0][0]);

      lookfrom.x = dist * sin(glm::radians(azimuth)) * cos(glm::radians(elevation));
      lookfrom.y = dist * sin(glm::radians(elevation));
      lookfrom.z = dist * cos(glm::radians(azimuth)) * cos(glm::radians(elevation));
      glm::mat4 camera = glm::lookAt(lookfrom, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
      glm::mat4 mvp = projection * camera * transform;
      glUniformMatrix4fv(matrixParam, 1, GL_FALSE, &mvp[0][0]);

      // Draw primitive
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
      glDrawElements(GL_TRIANGLES, numTriangles * 3, GL_UNSIGNED_INT, (void*)0);

      // Swap front and back buffers
      glfwSwapBuffers(window);

      // Poll for and process events
      glfwPollEvents();
   }

   glfwTerminate();
   return 0;
}

