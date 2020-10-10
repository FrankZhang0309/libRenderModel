//
// Created by zjk on 2020/10/10.
//

#include "Model.h"
#include <iostream>
#include <cmath>

Model::Model(string path, double scale)
{
//    if( !glfwInit() )
//    {
//        fprintf( stderr, "Failed to initialize GLFW\n" );
//    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    loadModel(path, scale);
}

glm::vec4 idIntToVec(int id){
    glm::vec4 vectorId;
    vectorId.x=((id & 0xFF000000)>>24);
    vectorId.y=((id & 0x00FF0000)>>16);
    vectorId.z=((id & 0x0000FF00)>>8);
    vectorId.w=((id & 0x000000FF));
//    cout << vectorId.x << " " << vectorId.y << " " << vectorId.z << " " << vectorId.w << endl;
    vectorId/=255;
    return vectorId;
}

unsigned int idVecToInt(glm::vec4 vectorId){
    unsigned int r = (unsigned int)(lround(vectorId.x*255)) << 24;
    unsigned int g = (unsigned int)(lround(vectorId.y*255)) << 16;
    unsigned int b = (unsigned int)(lround(vectorId.z*255)) << 8;
    unsigned int a = (unsigned int)lround(vectorId.w*255);
    //cout << r << " " << g << " " << b << " " << a << endl;
    return r+g+b+a;
}

bool Model::loadModel(string path, double scale){
    m_vertices.clear();
    m_indices.clear();
    happly::PLYData plyIn(path,true);
    std::vector<std::array<double, 3>> positions = plyIn.getVertexPositions();
    std::vector<std::array<unsigned char, 3>> colors = plyIn.getVertexColors();
    std::vector<std::vector<size_t>> faces;
    if (plyIn.hasElement("face"))
        faces = plyIn.getFaceIndices<size_t>();

    m_id = 1;
    for(int i = 0;i<positions.size();i++){
        Vertex vertex;
        vertex.position = vec3(scale*positions[i][0],scale*positions[i][1],scale*positions[i][2]);
        vertex.color = vec3(colors[i][0],colors[i][1],colors[i][2]);
        vertex.id = idIntToVec(m_id);
        m_id += 1;
        m_vertices.push_back(vertex);
    }
    if(faces.size() != 0){
        m_faceSize = faces[0].size();
        for (unsigned int i = 0; i < faces.size(); ++i) {
            for (unsigned int j = 0; j < m_faceSize; ++j) {
                m_indices.push_back(faces[i][j]);
            }
        }
    }else{
        for (unsigned int i = 0; i < positions.size(); ++i) {
            m_indices.push_back(i);
        }
    }
//        for(unsigned int i = 0; i < m_vertices.size() ; ++i) {
//            m_vertices.at(i).position -= m_center;
//        }
    for(unsigned int i = 0; i < m_vertices.size() ; ++i) {
        m_center += m_vertices.at(i).position/float(m_vertices.size());
    }

    processVertices();
    return true;
}

void Model::processVertices() {
    //glGenBuffers(1, &IDS);
    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex Colors
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    // vertex Ids
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,sizeof(Vertex), (void*)offsetof(Vertex, id));
    //Bind to Nothing
    glBindVertexArray(0);
}

void Model::draw() {
    glBindVertexArray(VAO);
    if(m_indices.size() > m_vertices.size())
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
    else{
        glDrawElements(GL_POINTS, m_indices.size(), GL_UNSIGNED_INT, nullptr);
    }
}

bool Model::isValidId(uint id){
    return id > 0 && id < m_vertices.size();
}