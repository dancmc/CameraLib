// %BANNER_BEGIN%
// ---------------------------------------------------------------------
// %COPYRIGHT_BEGIN%
//
// Copyright (c) 2018 Magic Leap, Inc. All Rights Reserved.
// Use of this file is governed by the Creator Agreement, located
// here: https://id.magicleap.com/creator-terms
//
// %COPYRIGHT_END%
// ---------------------------------------------------------------------
// %BANNER_END%

#ifndef EGL_EGLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#endif

#ifndef __ANDROID__
#define __ANDROID__
#endif

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

#include <IUnityInterface.h>
#include "IUnityGraphics.h"
#include <chrono>
#include <cmath>
#include <ml_camera.h>
#include <ml_privileges.h>
#include <ml_privilege_ids.h>
#include <ml_privilege_functions.h>
#include <EGL/eglplatform.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <GLES3/gl31.h>
#include <GLES3/gl32.h>
#include <GLES/glext.h>
// #include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ml_graphics.h>
#include <ml_head_tracking.h>
#include <ml_perception.h>
#include <ml_lifecycle.h>
#include <ml_logging.h>
#include "CameraLib.h"


static unsigned char* textureHandle;
static IUnityInterfaces* s_UnityInterfaces = NULL;
static IUnityGraphics* s_Graphics = NULL;
static UnityGfxRenderer s_RendererType = kUnityGfxRendererNull;

// Camera
    

    static GLuint fbo;
    static GLuint cameraOutFbo;
    static GLuint vao;
    static unsigned int vbo;
    static unsigned int ebo;
    static GLuint cameraProgram;
    static GLuint cameraVertex;
    static GLuint cameraFragment;
    static GLuint cameraInTexture;
    static GLuint cameraOutTexture;
    static GLuint cameraInTextureLocation;
    static GLuint contentTextureLocation;
    static GLuint contentTexture;
    static GLuint depthStencilTex;
    static bool initialised = false;
    static bool initialisedUnity = false;

    static GLuint unityFbo;
    static uint8_t tmp2[960 * 540 * 4];
    static uint8_t tmp[1280 * 720 * 3];
    static uint8_t tmpwhite[1280 * 720 * 3];
    GLuint holdingTexture;

    // static const char *cameraVsh = "\
//     #version 330\n\
//     \n\
//     in vec2 point;\n\
//     in vec2 uv;\n\
//     out vec2 vUvCamera;\n\
//     out vec2 vUvContent;\n\
//     \n\
//     float cameraAspectRatio = 1920.0/1080.0;\n\
//     float renderAspectRatio = 1920.0/1080.0;\n\
//     float xfactor = cameraAspectRatio/renderAspectRatio;\n\
//     float factor = 1.4;\n\
//     \n\
//     void main() {\n\
//     vUvCamera = vec2(uv.x, 1.0 - uv.y);\n\
//     vUvContent = vec2(\n\
//         ((uv.x * 0.5) - 0.25) * xfactor*factor + 0.25,\n\
//         (uv.y - 0.5) * factor + 0.5\n\
//     );\n\
//     gl_Position = vec4(point, 1.0, 1.0);\n\
//     }\n\
//     ";

    // static const char *cameraFsh = "\
//     #version 330\n\
//     #extension GL_OES_EGL_image_external : enable\n\
//     \n\
//     in vec2 vUvCamera;\n\
//     in vec2 vUvContent;\n\
//     out vec4 fragColor;\n\
//     \n\
//     uniform samplerExternalOES cameraInTexture;\n\
//     uniform sampler2D contentTexture;\n\
//     \n\
//     void main() {\n\
//     vec4 cameraIn = texture2D(cameraInTexture, vUvCamera);\n\
//     \n\
//     if (vUvContent.x >= 0.0 && vUvContent.x <= 0.5 && vUvContent.y >= 0.0 && vUvContent.y <= 1.0) {\n\
//         vec4 content = texture2D(contentTexture, vUvContent);\n\
//         fragColor = vec4((cameraIn.rgb * (1.0 - content.a)) + (content.rgb * content.a), 1.0);\n\
//     } else {\n\
//         fragColor = vec4(cameraIn.rgb, 1.0);\n\
//     }\n\
//     }\n\
//     ";

// uniform samplerExternalOES texture1; \n\
//uniform sampler2D texture1; \n\

//     static const char *cameraFsh = "\
// #version 330\n\
// #extension GL_OES_EGL_image_external : enable\n\
// \n\
// in vec2 TexCoord;\n\
// out vec4 FragColor;\n\
// \n\
// uniform samplerExternalOES texture1; \n\
// \n\
// void main(){\n\
//     vec4 text = texture2D(texture1, TexCoord);\n\
//     if(TexCoord.x>=0.5){\n\
// 	    FragColor = texture2D(texture1, TexCoord);\n\
//     } else {\n\
//         FragColor = vec4(255.0,255.0,255.0,0.0);\n\
//     }\n\
// }\n\
// ";

    static const char *cameraFsh = "\
#version 330\n\
#extension GL_OES_EGL_image_external : enable\n\
\n\
in vec2 TexCoord;\n\
out vec4 FragColor;\n\
\n\
uniform sampler2D texture1; \n\
\n\
void main(){\n\
    FragColor = texture2D(texture1, TexCoord);\n\
}\n\
";

    static const char *cameraVsh = "\
#version 330\n\
layout (location = 0) in vec3 pos;\n\
layout (location = 1) in vec2 aTexCoord;\n\
\n\
out vec2 TexCoord;\n\
\n\
void main()\n\
{\n\
	gl_Position = vec4(pos, 1.0);\n\
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n\
}\n\
";

    static float vertices[] = {
        // positions       // texture coords
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // top right
        1.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // bottom right
        -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, // bottom left
        -1.0f, 1.0f, 0.0f, 0.0f, 0.0f   // top left
    };

    static unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

extern "C"
{
    
    const char application_name[] = "com.company.livestream";

    // Callback type example - callback is a function taking an int
    typedef void (*CallbackInt)(int);
    CallbackInt ci;

    typedef void (*CallbackString)(char *);
    CallbackString cs;

    typedef void (*CallbackBytes)(int size, unsigned char *);
    CallbackBytes cb;

    int AddNum(int first, int second)
    {
        return first + second;
    }
    
    void checkError()
    {
        
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR)
        {
            ci(err);
        }

        EGLenum err1;
        while ((err1 = eglGetError()) != EGL_SUCCESS)
        {
            ci(err1);
        }
    }

    void MessageCallback(GLenum source,
                         GLenum type,
                         GLuint id,
                         GLenum severity,
                         GLsizei length,
                         const GLchar *message,
                         const void *userParam)
    {
        
        char s[100];
        sprintf(s, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
                (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
        cs(s);
    }

    // Setting callback example - pass a function taking an int
    void SetCallbackInt(CallbackInt callback)
    {
        ci = callback;
        ci(12);
    }

    void sendString(char *str)
    {
        char buffer[50];
        strncpy(buffer, str, sizeof buffer);
        buffer[sizeof buffer - 1] = '\0';

        cs(buffer);
    }

    void SetCallbackString(CallbackString callback)
    {
        cs = callback;
        // sendString("callback string set");
    }

    void SetCallbackBytes(CallbackBytes callback)
    {
        cb = callback;
        unsigned char buffer[2];
        buffer[0] = 5;
        buffer[1] = 3;
        cb(sizeof(buffer), buffer);
    }

    void SetTextureFromUnity(unsigned char *th)
    {

        
        textureHandle = th;
        ML_LOG(Info, "Unity Texture set" , application_name);
        ci(sizeof(th));
        ci((GLuint)(size_t)th);
    }

    

    

   

    void error(MLCameraError error, void *data)
    {
        sendString("Camera status error");
    }

    void deviceAvailableCallback(void *data)
    {
        sendString("Camera status available");
    }

    void deviceUnavailableCallback(void *data)
    {
        sendString("Camera status unavailable");
    }

    void deviceOpenedCallback(void *data)
    {
        sendString("Camera status opened");
    }

    void deviceClosedCallback(void *data)
    {
        sendString("Camera status closed");
    }

    void deviceDisconnectedCallback(void *data)
    {
        sendString("Camera status disconnected");
    }

    MLCameraDeviceStatusCallbacks a;

    // Callbacks
    // Structures

    // Structures
    struct application_context_t
    {
        int dummy_value;
    };

    struct graphics_context_t
    {

        EGLDisplay egl_display;
        EGLContext egl_context;

        GLuint framebuffer_id;
        GLuint vertex_shader_id;
        GLuint fragment_shader_id;
        GLuint program_id;

        graphics_context_t();
        ~graphics_context_t();

        void makeCurrent();
        void swapBuffers();
        void unmakeCurrent();
    };

    EGLint context_attribs[] = {
            EGL_CONTEXT_MAJOR_VERSION_KHR, 3,
            EGL_CONTEXT_MINOR_VERSION_KHR, 3,
            EGL_NONE};
    EGLConfig egl_config = nullptr;

    graphics_context_t::graphics_context_t()
    {
        ML_LOG(Info, "Creating context", application_name);
        egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

        EGLint major = 4;
        EGLint minor = 0;
        eglInitialize(egl_display, &major, &minor);
        eglBindAPI(EGL_OPENGL_API);

        EGLint config_attribs[] = {
            EGL_RED_SIZE, 5,
            EGL_GREEN_SIZE, 6,
            EGL_BLUE_SIZE, 5,
            EGL_ALPHA_SIZE, 0,
            EGL_DEPTH_SIZE, 24,
            EGL_STENCIL_SIZE, 8,
            EGL_NONE};
        
        EGLint config_size = 0;
        eglChooseConfig(egl_display, config_attribs, &egl_config, 1, &config_size);

        egl_context = eglCreateContext(egl_display, egl_config, EGL_NO_CONTEXT, context_attribs);
    }

    void graphics_context_t::makeCurrent()
    {
        eglMakeCurrent(egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, egl_context);
        glEnable(GL_TEXTURE_EXTERNAL_OES);
        
    }

    void graphics_context_t::unmakeCurrent()
    {
        eglMakeCurrent(NULL, EGL_NO_SURFACE, EGL_NO_SURFACE, NULL);
    }

    void graphics_context_t::swapBuffers()
    {
        // buffer swapping is implicit on device (MLGraphicsEndFrame)
    }

    graphics_context_t::~graphics_context_t()
    {
        eglDestroyContext(egl_display, egl_context);
        eglTerminate(egl_display);
    }

    graphics_context_t graphics_context;
    graphics_context_t graphics_context2;

    // Callbacks
    static void onStop(void *application_context)
    {
        ((struct application_context_t *)application_context)->dummy_value = 0;
        ML_LOG(Info, "%s: On stop called.", application_name);
    }

    static void onPause(void *application_context)
    {
        ((struct application_context_t *)application_context)->dummy_value = 1;
        ML_LOG(Info, "%s: On pause called.", application_name);
    }

    static void onResume(void *application_context)
    {
        ((struct application_context_t *)application_context)->dummy_value = 2;
        ML_LOG(Info, "%s: On resume called.", application_name);
    }

    void makeUnityCurrent(){
        eglMakeCurrent(eglGetCurrentDisplay(), EGL_NO_SURFACE, EGL_NO_SURFACE, eglGetCurrentContext());
        // MLGraphicsOptions graphics_options = {0, MLSurfaceFormat_RGBA8UNorm, MLSurfaceFormat_D32Float};
        // MLHandle opengl_context = reinterpret_cast<MLHandle>(eglGetCurrentContext());

        // MLHandle graphics_client = ML_INVALID_HANDLE;
        // MLGraphicsCreateClientGL(&graphics_options, opengl_context, &graphics_client);
        checkError();
    }

    void initialiseEGL(bool fromUnity)
    {
        if(!fromUnity){
            checkError();

            ML_LOG(Info, "Create Client", application_name);
        struct application_context_t application_context;
        application_context.dummy_value = 2;

        // Get ready to connect our GL context to the MLSDK graphics API
        graphics_context.makeCurrent();
        // glGenFramebuffers(1, &graphics_context.framebuffer_id);

        // MLGraphicsOptions graphics_options = {0, MLSurfaceFormat_RGBA8UNorm, MLSurfaceFormat_D32Float};
        // MLHandle opengl_context = reinterpret_cast<MLHandle>(graphics_context.egl_context);

        // MLHandle graphics_client = ML_INVALID_HANDLE;
        // MLGraphicsCreateClientGL(&graphics_options, opengl_context, &graphics_client);

        checkError();

        ML_LOG(Info, "Initialising", application_name);

        checkError();
        } else {
            ML_LOG(Info, "Make Unity Current", application_name);
            
        }

        // create shaders
        // link program
        // create cameraIn and Out textures
        // create cameraOutFbo and attach cameraOut texture
        // create vao and vbo
        // bind vao, bind vbo, then copy the quad vertices in
        // bind ebo and copy in element indices
        // also set vertex attribute pointers

        ML_LOG(Info, "Creating Vertex Shader", application_name);
        cameraVertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(cameraVertex, 1, &cameraVsh, NULL);
        glCompileShader(cameraVertex);
        GLint success;
        glGetShaderiv(cameraVertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            ML_LOG(Info, "ML camera vertex shader compilation failed", application_name);
        };
        checkError();

        ML_LOG(Info, "Creating Fragment Shader", application_name);
        cameraFragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(cameraFragment, 1, &cameraFsh, NULL);
        glCompileShader(cameraFragment);
        glGetShaderiv(cameraFragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            ML_LOG(Info, "ML camera fragment shader compilation failed", application_name);
        };
        checkError();

        ML_LOG(Info, "Creating Program", application_name);
        cameraProgram = glCreateProgram();
        glAttachShader(cameraProgram, cameraVertex);
        glAttachShader(cameraProgram, cameraFragment);
        glLinkProgram(cameraProgram);
        glGetProgramiv(cameraProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            ML_LOG(Info, "ML camera program linking failed", application_name);
        };
        checkError();

        ML_LOG(Info, "Gen textures", application_name);
        glGenTextures(1, &contentTexture);
        glGenTextures(1, &cameraInTexture);
        glGenTextures(1, &cameraOutTexture);
        checkError();

        ML_LOG(Info, "Bind cameraout texture", application_name);
        glBindTexture(GL_TEXTURE_2D, cameraOutTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 960, 540, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        checkError();

        ML_LOG(Info, "Gen camerafbo", application_name);
        glGenFramebuffers(1, &cameraOutFbo);
        checkError();
        ML_LOG(Info, "Bind camerafbo", application_name);
        glBindFramebuffer(GL_FRAMEBUFFER, cameraOutFbo);
        checkError();
        ML_LOG(Info, "Bind texture to camerafbo", application_name);
        // TODO change this
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cameraOutTexture, 0);
        checkError();

        // glGenFramebuffers(1, &fbo);
        // checkError();
        // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
        // checkError();
        // ML_LOG(Info, "Bind depth", application_name);
        // glGenTextures(1, &depthStencilTex);
        // checkError();
        // glBindTexture(GL_TEXTURE_2D, depthStencilTex);
        // checkError();
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // checkError();
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
        // checkError();
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 1920, 1080, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
        // checkError();
        // glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilTex, 0);
        // checkError();

        ML_LOG(Info, "Bind content texture", application_name);
        glBindTexture(GL_TEXTURE_2D, (GLuint)(size_t)textureHandle);
        checkError();

        ML_LOG(Info, "Context Texture Params", application_name);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // suspect might need to bind framebuffer, bind texturehandle as the color attachment of framebuffer then 
        // probably don't want to use glTexImage2D as not generating the texture
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        checkError();

        // ML_LOG(Info, "Framebuffer Texture Params", application_name);
        // glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, contentTexture, 0);
        // checkError();

        ML_LOG(Info, "gen buffers", application_name);
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        checkError();

        ML_LOG(Info, "bind vao", application_name);
        glBindVertexArray(vao);
        checkError();

        ML_LOG(Info, "bind vbo", application_name);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        checkError();

        ML_LOG(Info, "bind ebo", application_name);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        checkError();

        ML_LOG(Info, "attributes", application_name);
        // position attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        checkError();

        ML_LOG(Info, "get location", application_name);
        glUseProgram(cameraProgram);
        cameraInTextureLocation = glGetUniformLocation(cameraProgram, "texture1");
        contentTextureLocation = glGetUniformLocation(cameraProgram, "texture1");

        checkError();

        ML_LOG(Info, "initialised", application_name);
        
        
    }




    void SetFrame(MLHandle output){
        

        graphics_context.makeCurrent();
        ML_LOG(Info, "buffer callback", application_name);

        EGLImageKHR image = eglCreateImageKHR(eglGetDisplay(EGL_DEFAULT_DISPLAY), EGL_NO_CONTEXT, EGL_NATIVE_BUFFER_ANDROID, (EGLClientBuffer)(void *)output, nullptr);
        if (image == EGL_NO_IMAGE_KHR)
        {
            ML_LOG(Info, "EGLImage creation error", application_name);
        }

        
        glUseProgram(cameraProgram);

        ML_LOG(Info, "Gen Vertex and Program", application_name);

        // we create vertex and fragment shaders
        // fragment shader should at least have an external texture sampler for cameraIn

        // i think need to gen vao somewhere else, then bind vertices that we use to draw the quad later
        // cameraIn and content textures i think are textures we sample to render to framebuffer using the shader program
        // cameraIn is the external texture from the external cam

        // content texture is probably optional at this point, i think you attach another fbo to it and render the in-game content there
        // then we read the pixels off the framebuffer

        // we need to create a framebuffer, create cameraout texture, attach cameraout texture to camerafbo
        // the cameraout texture should be
        
        
        glBindFramebuffer(GL_FRAMEBUFFER, cameraOutFbo);
        

        ci(glCheckFramebufferStatus(GL_FRAMEBUFFER));
        checkError();

        ML_LOG(Info, "TexturesActive", application_name);
        glActiveTexture(GL_TEXTURE0);
        checkError();

        ML_LOG(Info, "TexturesBind", application_name);
        // glBindTexture(GL_TEXTURE_EXTERNAL_OES, cameraInTexture);
        ci((GLuint)(size_t)textureHandle);
        glBindTexture(GL_TEXTURE_2D, (GLuint)(size_t)textureHandle);
        // glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        // glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        checkError();

        ML_LOG(Info, "TexturesEGL", application_name);
        // glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, image);
        checkError();

        glUseProgram(cameraProgram);
        ML_LOG(Info, "Textures GL Uniform", application_name);
        // must activate program/shaders first
        // we bind cameraInTexture to the location/texture unit 0
        // get the sampler uniform's location, then point it to the appropriate texture unit (and therefore texture)
        glUniform1i(cameraInTextureLocation, 0);
        checkError();


        ML_LOG(Info, "Viewport", application_name);
        glViewport(0, 0, 960, 540);
        checkError();

        ci(glCheckFramebufferStatus(GL_FRAMEBUFFER));

        ML_LOG(Info, "GL Clear", application_name);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        checkError();

        glBindVertexArray(vao);
        ML_LOG(Info, "GL Draw arrays", application_name);
        // we draw one big rectangle made of 2 triangles, so 2+2 vertices required
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        checkError();

        ML_LOG(Info, "Destroy image", application_name);
        eglDestroyImageKHR(graphics_context.egl_display, image);
        checkError();

        ML_LOG(Info, "Read1 Pixels", application_name);
        // glClearColor(0.5, 0.0, 0.0, 0.0);

        glReadPixels(0, 0, 960, 540, GL_RGBA, GL_UNSIGNED_BYTE, tmp2);
        
        graphics_context.unmakeCurrent();
    }

     void preview_buffer_callback(MLHandle output, void *data)
    {

    
        
     if(!initialised){
                 

        // Now that graphics is connected, the app is ready to go


        initialiseEGL(false);
                initialised = true;
            }

        SetFrame(output);



        cb(sizeof(tmp2), tmp2);
        checkError();

        // ci(tmp[100]);
        // ci(tmp[101]);
        // ci(tmp[102]);
        // ci(tmp[103]);
        // ci(tmp[104]);
        // ci(tmp[105]);
    }

    unsigned char* GrabFrame(){
       
            MLHandle output;
            MLCameraGetPreviewStream(&output);
            SetFrame(output);
            return tmp2;
    }

    unsigned char* GrabTestFrame(){
    //     glGenFramebuffers(1, &unityFbo);
    // glBindFramebuffer(GL_FRAMEBUFFER, unityFbo);
    
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,cameraOutTexture, 0);
    // ci(glCheckFramebufferStatus(GL_FRAMEBUFFER));
    // ML_LOG(Info, "OPENGL READ PIXELS", application_name);
    // glReadPixels(0, 0, 960, 540, GL_RGB, GL_UNSIGNED_BYTE, tmp);

    // ci(tmp[100]);
    // ci(tmp[101]);
    // ci(tmp[102]);


            return tmp;
    }



    void SetPreviewCallback()
    {

        memset(tmp, 100, sizeof(tmp));

   
  
        MLPrivilegesRequestPrivilege(MLPrivilegeID_CameraCapture);

        a = (MLCameraDeviceStatusCallbacks){
            .on_device_available = deviceAvailableCallback,
            .on_device_unavailable = deviceUnavailableCallback,
            .on_device_opened = deviceOpenedCallback,
            .on_device_closed = deviceClosedCallback,
            .on_device_disconnected = deviceDisconnectedCallback,
            .on_device_error = error,
            .on_preview_buffer_available = preview_buffer_callback};


        a = (MLCameraDeviceStatusCallbacks){
            .on_device_available = deviceAvailableCallback,
            .on_device_unavailable = deviceUnavailableCallback,
            .on_device_opened = deviceOpenedCallback,
            .on_device_closed = deviceClosedCallback,
            .on_device_disconnected = deviceDisconnectedCallback,
            .on_device_error = error,
            .on_preview_buffer_available = preview_buffer_callback};

        // MLCameraSetDeviceStatusCallbacks(&a, NULL);
        ML_LOG(Info, "C API Callbacks set", application_name);
        MLResult b = MLCameraConnect();
        

        if(!initialised){
        initialiseEGL(false);
                initialised = true;
            }
    }

    int InitialiseExternalTexture(){
        EGLContext cc =  eglGetCurrentContext();
    EGLDisplay dd =  eglGetCurrentDisplay();
    EGLSurface sr =  eglGetCurrentSurface(EGL_READ);
    EGLSurface sd =  eglGetCurrentSurface(EGL_DRAW);
        graphics_context2.makeCurrent();
        glGenTextures(1, &holdingTexture);
        ci(holdingTexture);
            glBindTexture(GL_TEXTURE_2D, holdingTexture);
            uint8_t tmptmp[960 * 540 * 4];
            memset(tmptmp, 255, sizeof(tmptmp));
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 960, 540, 0, GL_RGBA, GL_UNSIGNED_BYTE, tmptmp);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            graphics_context2.unmakeCurrent();
            eglMakeCurrent(dd,sd,sr,cc);

            return holdingTexture;
    }
}

// Unity plugin load event
static void UNITY_INTERFACE_API
    OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType)
{
    switch (eventType)
    {
        case kUnityGfxDeviceEventInitialize:
        {
            ML_LOG(Info, "PLUGIN LOADED", application_name);
            s_RendererType = s_Graphics->GetRenderer();

    //     if(!initialisedUnity){
    // initialiseEGL(true);  
    // initialisedUnity=true;
    // } 
            break;
        }
        case kUnityGfxDeviceEventShutdown:
        {
            s_RendererType = kUnityGfxRendererNull;
            //TODO: user shutdown code
            break;
        }
        case kUnityGfxDeviceEventBeforeReset:
        {
            //TODO: user Direct3D 9 code
            break;
        }
        case kUnityGfxDeviceEventAfterReset:
        {
            //TODO: user Direct3D 9 code
            break;
        }
    };
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API
    UnityPluginLoad(IUnityInterfaces* unityInterfaces)
{
    	s_UnityInterfaces = unityInterfaces;
	s_Graphics = s_UnityInterfaces->Get<IUnityGraphics>();
	s_Graphics->RegisterDeviceEventCallback(OnGraphicsDeviceEvent);
    OnGraphicsDeviceEvent(kUnityGfxDeviceEventInitialize);
}


// Unity plugin unload event
extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API
    UnityPluginUnload()
{
    s_Graphics->UnregisterDeviceEventCallback(OnGraphicsDeviceEvent);
    
}


EGLContext shared_context;


// Plugin function to handle a specific rendering event
static void UNITY_INTERFACE_API OnRenderEvent(int eventID)
{
    std::string a = "NATIVE RENDER ";
    std::string result = a + std::to_string((GLuint)(size_t) textureHandle);
    
    ML_LOG(Info, result.c_str(), application_name);

    EGLContext cc =  eglGetCurrentContext();
    EGLDisplay dd =  eglGetCurrentDisplay();
    EGLSurface sr =  eglGetCurrentSurface(EGL_READ);
    EGLSurface sd =  eglGetCurrentSurface(EGL_DRAW);

    // shared_context = eglCreateContext(dd, egl_config, cc, context_attribs);
    // eglMakeCurrent(eglGetDisplay(EGL_DEFAULT_DISPLAY), EGL_NO_SURFACE, EGL_NO_SURFACE, shared_context);
     
     checkError();

    // EGLint imageAttributes[] = { 
    // EGL_GL_TEXTURE_LEVEL_KHR, 0, // mip map level to reference 
    // EGL_IMAGE_PRESERVED_KHR, EGL_FALSE, 
    // EGL_NONE 
    // }; 
    // EGLImageKHR eglImage = eglCreateImageKHR(eglGetCurrentDisplay(), eglGetCurrentContext(), GL_TEXTURE_2D, (EGLClientBuffer)textureHandle, imageAttributes);
    //  if (eglImage == EGL_NO_IMAGE_KHR)
    //     {
    //         ML_LOG(Info, "EGLImage creation error", application_name);
    //     }

    glGenFramebuffers(1, &unityFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, unityFbo);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cameraOutTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, (GLuint)(size_t)textureHandle, 0);
    ci(glCheckFramebufferStatus(GL_FRAMEBUFFER));
    ML_LOG(Info, "OPENGL READ PIXELS", application_name);
    glReadPixels(0, 0, 960, 540, GL_RGB, GL_UNSIGNED_BYTE, tmp);

    

    

    ci(tmp[100]);
    ci(tmp[101]);
    ci(tmp[102]);

    
    
    graphics_context2.makeCurrent();
    if(!initialisedUnity){
        
        memset(tmpwhite, 255, sizeof(tmpwhite));
        initialisedUnity = true;
        initialiseEGL(true);
    }


    
    // Comment out this block if not trying to use rendertexture directly
    // ML_LOG(Info, "Gen textures", application_name);
    // glGenTextures(1, &cameraOutTexture);
    // checkError();
    // ML_LOG(Info, "Bind cameraout texture", application_name);
    // glBindTexture(GL_TEXTURE_2D, cameraOutTexture);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 960, 540, 0, GL_BGRA8_EXT, GL_UNSIGNED_BYTE, textureHandle);
    // checkError();
    
    graphics_context2.unmakeCurrent();
    eglMakeCurrent(dd,sd,sr,cc);

    
    

    
    
    
    
}
    
// Freely defined function to pass a callback to plugin-specific scripts
extern "C" UnityRenderingEvent UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API
    GetRenderEventFunc()
{

    return OnRenderEvent;
}
