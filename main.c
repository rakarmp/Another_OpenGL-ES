#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "shader_utils.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <jni.h>

// Vertex Shader Source
const char* vertexShaderSource = 
    "attribute vec4 a_Position;\n"
    "void main() {\n"
    "   gl_Position = a_Position;\n"
    "}\n";

// Fragment Shader Source
const char* fragmentShaderSource = 
    "precision mediump float;\n"
    "void main() {\n"
    "   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "}\n";

void render_loop(ANativeWindow* window) {
    // EGL initialization
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, NULL, NULL);

    EGLConfig config;
    EGLint numConfigs;
    EGLint attributes[] = {EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE};
    
    eglChooseConfig(display, attributes, &config, 1, &numConfigs);
    
    EGLSurface surface = eglCreateWindowSurface(display, config, window, NULL);
    
    EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);

    eglMakeCurrent(display, surface, surface, context);

    GLuint program = create_program(vertexShaderSource, fragmentShaderSource);
    glUseProgram(program);

    GLfloat vertices[] = {0.0f,  0.5f, 0.0f, 
                          -0.5f, -0.5f, 0.0f, 
                          0.5f, -0.5f, 0.0f};

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLint posAttrib = glGetAttribLocation(program, "a_Position");
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(posAttrib);

    while (1) {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        eglSwapBuffers(display, surface);
    }

    // Cleanup
    eglDestroyContext(display, context);
    eglDestroySurface(display, surface);
    eglTerminate(display);
}

JNIEXPORT void JNICALL Java_com_example_myapp_NativeRenderer_00024Companion_render(JNIEnv *env, jobject obj, jobject surface) {
    ANativeWindow* window = ANativeWindow_fromSurface(env, surface);
    render_loop(window);
    ANativeWindow_release(window);
}
