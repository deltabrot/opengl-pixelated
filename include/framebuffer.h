#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glut.h>
#include <GL/glew.h>

void initFrameBufferDepthBuffer(unsigned int *fbo_depth){
        glGenRenderbuffersEXT(1, fbo_depth); // Generate one render buffer and store the ID in fbo_depth  
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, *fbo_depth); // Bind the fbo_depth render buffer   
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, 800*2, 600*2); // Set the render buffer storage to be a depth component, with a width and height of the window 
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, *fbo_depth); // Set the render buffer of this buffer to the depth buffer
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0); // Unbind the render buffer
}

void initFrameBufferTexture(unsigned int *fbo_texture){
        glGenTextures(1, fbo_texture); // Generate one texture  
        glBindTexture(GL_TEXTURE_2D, *fbo_texture); // Bind the texture fbo_texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800*2, 600*2, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // Create a standard texture with the width and height of our window
        // Setup the basic texture parameters
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // Unbind the texture  
        glBindTexture(GL_TEXTURE_2D, 0);  
}
void initFrameBuffer(unsigned int *fbo, unsigned int *fbo_depth, unsigned int *fbo_texture){
        initFrameBufferDepthBuffer(fbo_depth); // Initialize our frame buffer depth buffer  
        initFrameBufferTexture(fbo_texture); // Initialize our frame buffer texture
        glGenFramebuffersEXT(1, fbo); // Generate one frame buffer and store the ID in fbo  
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, *fbo); // Bind our frame buffer  
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, *fbo_texture, 0); // Attach the texture fbo_texture to the color buffer in our frame buffer
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, *fbo_depth); // Attach the depth buffer fbo_depth to our frame buffer
        GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT); // Check that status of our generated frame buffer
        if (status != GL_FRAMEBUFFER_COMPLETE_EXT){ // If the frame buffer does not report back as complete   
                std::cout << "Couldn't create frame buffer" << std::endl;
                exit(0);
        }
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // Unbind our frame buffer  
}

#endif
