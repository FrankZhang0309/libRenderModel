//
// Created by zjk on 2020/10/10.
//


#include <ModelRenderer.h>

int main(){
    ModelRenderer renderer("../models/dance-low-res.ply",1, 800,600);

    //You can show images to see if you are using right settings(view matrix, projection matrix, model matrix)
    renderer.showColor();
    renderer.showDepth();
    renderer.showId();

    //You can query color, depth, id map
    cv::Mat myColor = renderer.color(); //uchar, (height x width x 4), BGRA value of the rendered image
    cv::Mat myDepth = renderer.depth(); //float, (height x width), the value scale is aligned with the world coordinate
    cv::Mat myId = renderer.id(); //int, (height x width)

    //You may want to set pose and projection matrix by your desire, try below functions
    //Here are the definitions of them, please transform your data to the right form, then set them.
    //NOTICE: Be careful of the transformation of the openGL world coordinate and your original world coordinate. It
    //usually happens on z-axis and y-axis, which makes you see nothing or see y-mirrored images. You may need to
    // implement a transform function by your own.

//    void setViewMatrix(glm::mat4 viewMatrix);
//    void setProjectionMatrix(glm::mat4 projMatrix);
//    void setModelMatrixMatrix(glm::mat4 modelMatrix);
//    void setProjMatrixFromIntrinsic(glm::mat3 projMatrix, int width, int height, double near_plane, double far_plane);
}