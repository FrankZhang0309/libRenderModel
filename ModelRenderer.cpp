//
// Created by zjk on 2020/10/9.
//

#include "ModelRenderer.h"
#include "Shader.h"
#include <glm/gtx/string_cast.hpp>

GLFWwindow* window;

ModelRenderer::ModelRenderer(string pathName, double scale, int width, int height) {

    m_width = width;
    m_height = height;

    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
    }


    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow( m_width, m_height, "Tutorial 01", NULL, NULL);
    glfwHideWindow(window);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwTerminate();
    }

    setImageSize(m_width, m_height);

    if (pathName != ""){
        m_model = new Model(pathName,scale);
        m_center = m_model->center();
    }

    m_defaultShader = new Shader("../shaders/default.vert", "../shaders/default.frag");
    m_idShader = new Shader("../shaders/id.vert", "../shaders/id.frag");

    m_projectionMatrix = glm::perspective(m_fov, double(m_width) / m_height , m_zNear, m_zFar);
    cout << glm::to_string(m_projectionMatrix) << endl;
    vec3 position = vec3(2,0,0);
    m_viewMatrix = glm::lookAt(position + m_center,  m_center, vec3(0,1,0));
    m_modelMatrix = glm::mat4(1.0f);

    glEnable(GL_DEPTH_TEST);
}

void ModelRenderer::setImageSize(int width, int height) {
    glViewport(0,0,width,height);
    m_width = width;
    m_height = height;

    glDeleteFramebuffers(1,&FBO);
    glGenFramebuffers(1, &FBO);

    glDeleteTextures(1, &TEXTURE);
    glGenTextures(1, &TEXTURE);

    glBindTexture(GL_TEXTURE_2D, TEXTURE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    glDeleteRenderbuffers(1,&RBO);
    glGenRenderbuffers(1,&RBO);
    glBindRenderbuffer(GL_RENDERBUFFER,RBO);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,m_width,m_height);

    glBindRenderbuffer(GL_RENDERBUFFER,0);
}

void ModelRenderer::setViewMatrix(glm::mat4 viewMatrix){
    m_viewMatrix = viewMatrix;
}
void ModelRenderer::setProjectionMatrix(glm::mat4 projMatrix){
    m_projectionMatrix = projMatrix;

}
void ModelRenderer::setModelMatrixMatrix(glm::mat4 modelMatrix){
    m_modelMatrix = modelMatrix;
}

void ModelRenderer::setProjMatrixFromIntrinsic(glm::mat3 projMatrix, int width, int height, double near_plane, double far_plane)
{
    m_zNear = near_plane;
    m_zFar = far_plane;

    float f_x = projMatrix[0][0];
    float f_y = projMatrix[1][1];

    float c_x = projMatrix[2][0];
    float c_y = projMatrix[2][1];
    std::cout<< f_x << std::endl;
    std::cout<< f_y << std::endl;
    std::cout<< c_y << std::endl;

    std::cout<< c_x << std::endl;

    glm::mat4 projection_matrix(0.0f);

    projection_matrix[0][0] = 2*f_x/width;

    projection_matrix[1][1] = 2*f_y/height;

    projection_matrix[2][0] = 2*c_x/width-1;
    projection_matrix[2][1] = 2*c_y/height-1;
    projection_matrix[2][2] = -(far_plane + near_plane)/(far_plane - near_plane);
    projection_matrix[3][2] = -2.0f*far_plane*near_plane/(far_plane - near_plane);
    projection_matrix[2][3] = -1.0f;

    cout << glm::to_string(projection_matrix) << endl;
    m_projectionMatrix = projection_matrix;
}


cv::Mat ModelRenderer::color(){

    // don't forget to enable shader before setting uniforms
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TEXTURE, 0);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,RBO);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_defaultShader->use();

    m_defaultShader->setMat4("projection", m_projectionMatrix);
    m_defaultShader->setMat4("view", m_viewMatrix);
    m_defaultShader->setMat4("model", m_modelMatrix);

    m_model->draw();

    //Initinalize buffer
    GLubyte *bufferColor;
    bufferColor = (GLubyte*) malloc((m_height)*(m_width)*4);

    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(0,0,m_width,m_height,GL_BGRA,GL_UNSIGNED_BYTE,&bufferColor[0]);

    cv::Mat color = cv::Mat::zeros(m_height,m_width,CV_8UC4);
    memcpy(color.data, bufferColor, (m_height)*(m_width)*4 * sizeof(GLubyte));

    //flip matrix because the different coordinate of openGL and normal image.
    cv::flip(color,color,0);

    free(bufferColor);
    return color;
}

cv::Mat ModelRenderer::id(){

    // don't forget to enable shader before setting uniforms
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TEXTURE, 0);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,RBO);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_idShader->use();

    m_idShader->setMat4("projection", m_projectionMatrix);
    m_idShader->setMat4("view", m_viewMatrix);
    m_idShader->setMat4("model", m_modelMatrix);

    m_model->draw();
    glfwSwapBuffers(window);

    //Initinalize buffer
    float *bufferId;

    bufferId = (float*) malloc(m_height*m_width*4*sizeof(float));

    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(0,0,m_width,m_height,GL_RGBA,GL_FLOAT,&bufferId[0]);

//    cv::Mat id_color = cv::Mat::zeros(m_height,m_width,CV_32FC4);
//    memcpy(id_color.data, bufferId, (m_height)*(m_width)* 4 * sizeof(float));

    cv::Mat id = cv::Mat::zeros(m_height,m_width,CV_32SC1);
    vector<int> ids;
    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {

            glm::vec4 temp;
            temp.x = bufferId[4*(i*m_width+j)];
            temp.y = bufferId[4*(i*m_width+j)+1];
            temp.z = bufferId[4*(i*m_width+j)+2];
            temp.w = bufferId[4*(i*m_width+j)+3];
            int idInt = (int)idVecToInt(temp);

            if(m_model->isValidId(idInt)){
                id.at<int>(i,j) = idInt;
            }
        }
    }

    free(bufferId);

    cv::flip(id,id,0);

    return id;

}

cv::Mat recoverDepthFromClippedSpace(cv::Mat depth, double n, double f){

    cv::Mat z=(2.0*depth)-1.0;
    return (2.0*n*f)/(f+n-(z*(f-n)));
}

cv::Mat ModelRenderer::depth(){

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TEXTURE, 0);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,RBO);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_defaultShader->use();

    m_defaultShader->setMat4("projection", m_projectionMatrix);
    m_defaultShader->setMat4("view", m_viewMatrix);
    m_defaultShader->setMat4("model", m_modelMatrix);

    m_model->draw();


    //Initinalize buffer
    float *bufferDepth;
    bufferDepth = (float*) malloc(m_height*m_width*sizeof(float));
    //Read pixels from Id FBO
    //glReadPixels(0,0,m_width,1,GL_RGBA,GL_UNSIGNED_BYTE,&bufferId[m_width*i*4]);
    glReadBuffer(GL_DEPTH_ATTACHMENT);
    glReadPixels(0,0,m_width,m_height,GL_DEPTH_COMPONENT,GL_FLOAT,&bufferDepth[0]);

    cv::Mat depth = cv::Mat::zeros(m_height,m_width,CV_32F);
    memcpy(depth.data, bufferDepth, (m_height)*(m_width) * sizeof(float));

    threshold(depth, depth, 0.99999, 0, CV_THRESH_TOZERO_INV);
    depth = recoverDepthFromClippedSpace(depth, m_zNear, m_zFar);
    depth = depth/m_zFar;
//    threshold(depth, depth, m_zNear+0.0001, 0, CV_THRESH_TOZERO);
    cv::flip(depth,depth,0);

    free(bufferDepth);
    return depth;

}

void ModelRenderer::showColor() {

    cv::Mat myColor = color();

    cv::imshow("Color",myColor);
    cv::waitKey(0);

}

void ModelRenderer::showDepth(){

    cv::Mat myDepth = depth();

    cv::imshow("Depth",myDepth);
    cv::waitKey(0);

}

void ModelRenderer::showId(){
    cv::Mat myId = id();

    cv::imshow("Id",myId);
    cv::waitKey(0);
}

