//
// Created by zjk on 2020/10/10.
//

#ifndef RENDERMODEL_MODEL_H
#define RENDERMODEL_MODEL_H

#include "happly.h"
#include <glm/glm.hpp>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;
using namespace glm;

struct Vertex {
    // position
    vec3 position;
    vec3 color;
    vec4 id;
};

glm::vec4 idIntToVec(unsigned int id);
unsigned int idVecToInt(glm::vec4 vectorId);

class Model {
public:
    Model(string path, double scale);
    GLuint VAO;
    //GLuint IDS;
    void draw();
    vector<Vertex> m_vertices;
    void processVertices();

    bool loadModel(string path, double scale);
    vec3 center(){return m_center;}

    bool isValidId(uint id);

private:
    GLuint VBO, EBO;
    vector<unsigned int> m_indices;
    unsigned int m_faceSize;

    int m_id = 0;
    happly::PLYData *m_ply;
    vec3 m_center = vec3(0,0,0);


};


#endif //RENDERMODEL_MODEL_H
