//
// Created by 45010 on 2021/6/27.
//

#ifndef PURENATIVECALCULATORDEMO_APP_ENGINE_H
#define PURENATIVECALCULATORDEMO_APP_ENGINE_H

#include <android/sensor.h>
#include <EGL/egl.h>

struct saved_state {
    float angle;
    int32_t x;
    int32_t y;
};

//engine context
struct engine{
    struct android_app* app;

    ASensorManager      *sensorManager;
    const ASensor       *accelerometerSenor;
    ASensorEventQueue   *sensorEventQueue;

    //will draw when value = 1
    int animation;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;

    int32_t     width;
    int32_t     height;

    struct saved_state state;

    GLuint programObject;

};


#endif //PURENATIVECALCULATORDEMO_APP_ENGINE_H
