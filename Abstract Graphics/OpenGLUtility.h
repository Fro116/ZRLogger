//
//  OpenGLUtility.h
//  Hearts
//
//  Created by Kundan Chintamaneni on 3/14/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#ifndef __Hearts__OpenGLUtility__
#define __Hearts__OpenGLUtility__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class OpenGLUtility {
public:
    static GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
    static GLFWwindow* InitializeWindow(const char* name, const int width, const int height);
    static GLuint CreateVertexArrayObject(const GLfloat vertexBufferData[],
                                          GLsizeiptr vertexBufferDataSize,
                                          GLuint vertexComponentCount,
                                          const GLfloat colorBufferData[],
                                          GLsizeiptr colorBufferDataSize,
                                          GLuint colorComponentCount,
                                          const GLushort elementBufferData[],
                                          GLsizeiptr elementBufferDataSize
                                          );
    static GLuint Load2DTexture(const char* name, GLint format);
private:
};

#endif /* defined(__Hearts__OpenGLUtility__) */
