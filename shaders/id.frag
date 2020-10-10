#version 430 core
out vec4 FragColor;

in vec4 Id;
void main()
{
    FragColor = Id;
    //FragColor = vec4(1,1,1,1);
}

//uniform sampler2D qt_Texture0;
//varying vec4 qt_TexCoord0;

//void main(void)
//{
//    gl_FragColor = texture2D(qt_Texture0, qt_TexCoord0.st);
//}
