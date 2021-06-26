//
// Created by Admin on 2021/6/18.
//

#ifndef PURENATIVECALCULATORDEMO_NATIVE_ACTIVITY_METHOD_H
#define PURENATIVECALCULATORDEMO_NATIVE_ACTIVITY_METHOD_H

//native activity's call back
static void engine_handle_cmd(struct android_app* app,int32_t cmd);

//input event's call back
static int32_t engine_handle_input(struct android_app* app,AInputEvent* event);

//init display,egl
static int engine_init_display(struct engine* engine);

//terminate display
static void engine_term_display(struct engine* engine);

//draw
static void engine_draw_frame(struct engine * engine);

//fetch sensor manager
ASensorManager* acquireASensorManagerInstance(android_app* app);

//
GLuint loadShader(GLenum type,const char* shaderSrc);

static void gl_draw(struct engine *engine);


#endif //PURENATIVECALCULATORDEMO_NATIVE_ACTIVITY_METHOD_H
