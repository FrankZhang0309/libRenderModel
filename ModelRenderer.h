//
// Created by zjk on 2020/10/9.
//

#ifndef LIBRENDERMODEL_MODELRENDERER_H
#define LIBRENDERMODEL_MODELRENDERER_H

#include <glm/glm.hpp> // vec2, vec3, mat4, radians

// Include all GLM extensions
#include <glm/ext.hpp> // perspective, translate, rotate

#include <iostream>
#include "Model.h"
#include "Shader.h"

using namespace std;
using namespace glm;

class ModelRenderer {
public:
    ModelRenderer(string pathName = "", double scale = 1, int width = 800, int height = 600);

    void loadModel(string pathName);

    //Set functions
    void setViewMatrix(glm::mat4 viewMatrix);
    void setProjectionMatrix(glm::mat4 projMatrix);
    void setModelMatrixMatrix(glm::mat4 modelMatrix);
    void setProjMatrixFromIntrinsic(glm::mat3 projMatrix, int width, int height, double near_plane, double far_plane);

    // Save functions
//    void saveDepth(string pathName);
//    void saveColor(string pathName);
//    void saveId(string pathName);

    //Show functions
    void showColor();
    void showDepth();
    void showId();

    //Query functions
    cv::Mat id();
    cv::Mat depth();
    cv::Mat color();

    //Resize image size;
    void setImageSize(int width, int height);

private:
    mat4 m_modelMatrix, m_viewMatrix, m_projectionMatrix;

    double m_zNear = 0.1, m_zFar = 10.0, m_fov = 45.0;
    int m_width, m_height;
    vec3 m_center = vec3(0,0,0);

    GLuint FBO,RBO,TEXTURE;

    Model* m_model;

    Shader *m_defaultShader, *m_idShader;
};


#endif //LIBRENDERMODEL_MODELRENDERER_H
