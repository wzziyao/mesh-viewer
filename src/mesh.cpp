
#include "mesh.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include <cmath>
#include <iostream>
#include <fstream>

#include <sstream>

using namespace std;
using namespace glm;
using namespace agl;

Mesh::Mesh() 
{
   num_vertices = 0;
   num_polygons = 0;
   min_bounds = glm::vec3(0, 0, 0);
   max_bounds = glm::vec3(0, 0, 0);
   vertex = NULL;
   normal = NULL;
   index = NULL;
}

Mesh::~Mesh()
{
   delete[] vertex;
   delete[] normal;
   delete[] index;
}

bool Mesh::loadPLY(const std::string& filename)
{
   delete[] vertex;
   delete[] normal;
   delete[] index;

   ifstream file(filename);
   if (!file) {
      file.close();
      return false;
   }

   string next;
   // file >> next; // ply
   // file >> next; // format
   // file >> next; // ascii/binary
   // file >> next; // format version number
   // file >> next; // next comment

   while (next != "element") {
      file >> next;
   }
   file >> next; // vertex
   file >> num_vertices;

   // cout << num_vertices << endl;

   while (next != "element") {
      file >> next;
   }
   file >> next; // face
   file >> num_polygons;

   // cout << num_polygons << endl;

   while (next != "end_header") {
      file >> next;
   }

   vertex = new float[3 * num_vertices];
   normal = new float[3 * num_vertices];
   index = new unsigned int[3 * num_polygons];

   string line;
   // read vertices and normals
   for (int i = 0; i < num_vertices; i++) {
      // cout << i << endl;
      file >> vertex[3 * i];
      file >> vertex[3 * i + 1];
      file >> vertex[3 * i + 2];
      file >> normal[3 * i];
      file >> normal[3 * i + 1];
      file >> normal[3 * i + 2];
      // cout << "i: " << i << ", " << vertex[3 * i] << ", " << vertex[3 * i + 1] << ", " << vertex[3 * i + 2] << endl;

      if (filename == "cube.ply" || filename == "pyramid.ply" || filename == "sphere.ply") {
         file >> next; // s
         file >> next; // t
      } 

      if (i == 0) {
         min_bounds.x = vertex[0];
         min_bounds.y = vertex[1];
         min_bounds.z = vertex[2];
         max_bounds.x = vertex[0];
         max_bounds.y = vertex[1];
         max_bounds.z = vertex[2];
      } else {
         if (vertex[3 * i] < min_bounds.x) min_bounds.x = vertex[3 * i];
         if (vertex[3 * i + 1] < min_bounds.y) min_bounds.y = vertex[3 * i + 1];
         if (vertex[3 * i + 2] < min_bounds.z) min_bounds.z = vertex[3 * i + 2];
         if (vertex[3 * i] > max_bounds.x) max_bounds.x = vertex[3 * i];
         if (vertex[3 * i + 1] > max_bounds.y) max_bounds.y = vertex[3 * i + 1];
         if (vertex[3 * i + 2] > max_bounds.z) max_bounds.z = vertex[3 * i + 2];
      }
   }

   // read polygon indices
   for (int j = 0; j < num_polygons; j++) {
      file >> next; // 3 
      file >> index[3 * j];
      file >> index[3 * j + 1];
      file >> index[3 * j + 2];
      // cout << "j: " << j << ", " << index[3 * j] << ", " << index[3 * j + 1] << ", " << index[3 * j + 2] << endl;
   }

   cout << filename << endl;
   return true;
}

glm::vec3 Mesh::getMinBounds() const
{
  return min_bounds;
}

glm::vec3 Mesh::getMaxBounds() const
{
  return max_bounds;
}

int Mesh::numVertices() const
{
   return num_vertices;
}

int Mesh::numTriangles() const
{
   return num_polygons;
}

float* Mesh::positions() const
{
   return vertex;
}

float* Mesh::normals() const
{
   return normal;
}

unsigned int* Mesh::indices() const
{
   return index;
}

