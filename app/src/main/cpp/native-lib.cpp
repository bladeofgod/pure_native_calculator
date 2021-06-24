#include <jni.h>
#include <string> // c++ string
#include <memory>
#include <cstdlib>
//#include <cstring> // c string of c++ version
#include <error.h>
#include <cassert>
#include <dlfcn.h>

//draw ui
#include <EGL/egl.h>
#include <GLES/gl.h>
#include "gl_painter.h"

//relative android stuff
#include <android/sensor.h>
#include <android/log.h>
#include "android_native_app_glue.h"

#include "native_activity_method.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,"calculator-activity",__VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,"calculator-activity",__VA_ARGS__))

typedef std::string String;

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

};

/*
 * native android
 * entry point
 */

void android_main(struct android_app* state) {
    LOGE("native activity : android_main");

    struct engine engine{};

    //init a part memory for engine
    memset(&engine,0,sizeof(engine));

    state->userData = &engine;
    //register call back
    state->onAppCmd = engine_handle_cmd;
    //register input call back
    state->onInputEvent = engine_handle_input;

    //save state
    engine.app = state;

    engine.sensorManager = acquireASensorManagerInstance(state);
    engine.accelerometerSenor = ASensorManager_getDefaultSensor(
            engine.sensorManager,
            ASENSOR_TYPE_ACCELEROMETER
            );
    engine.sensorEventQueue = ASensorManager_createEventQueue(
            engine.sensorManager,
            state->looper,
            LOOPER_ID_USER,
            nullptr,
            nullptr
            );

    if(state->savedState != nullptr) {
        //make {engine.state}  pointer to {state->savedState}
        engine.state = *(struct saved_state *)state->savedState;
    }

    while (true) {
        int ident;
        int events;

        struct android_poll_source* source;

        //pull all event-data to source
        while ((ident = ALooper_pollAll(
                engine.animation ? 0 : 1, nullptr,&events,(void **)&source
                )) > 0) {

            if(source != nullptr) {
                //process the source data
                source->process(state,source);
            }

            if(ident == LOOPER_ID_USER) {
                //is user looper event data
                // [accelerometerSenor] will detect state of motion,and finitely in this loop(while)
                // but input-event can interrupt it shortly
                if(engine.accelerometerSenor != nullptr) {
                    ASensorEvent   event;
                    while (ASensorEventQueue_getEvents(engine.sensorEventQueue,//add queue
                                                       &event,//event data
                                                       1)
                                                       > 0) {
                        //disable it , because will rush screen...
//                        LOGI("accelerometer: x=%f   y=%f   z=%f",
//                             event.acceleration.x,
//                             event.acceleration.y,
//                             event.acceleration.z);
                    }
                }
            }

            //if app is dead, this {destroyRequested} will non-zero
            if(state->destroyRequested != 0) {
                engine_term_display(&engine);
                return;
            }

        }

        if(engine.animation) {
            engine.state.angle += .01f;
            if(engine.state.angle > 1 ) {
                engine.state.angle = 0;
            }
            engine_draw_frame(&engine);
        }



    }

}

static int engine_init_display(struct engine* engine) {


    //rgb 888

    const EGLint attributes[] = {
            EGL_SURFACE_TYPE,
            EGL_WINDOW_BIT,
            EGL_BLUE_SIZE,8,
            EGL_GREEN_SIZE,8,
            EGL_RED_SIZE,8,
            EGL_NONE
    };

    EGLint w,h,format;
    EGLint numConfigs;
    EGLConfig config = nullptr;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    //init egl
    // display, version..
    eglInitialize(display, nullptr, nullptr);

    eglChooseConfig(display,attributes, nullptr,0,&numConfigs);

    std::unique_ptr<EGLConfig[]> supportedConfigs(new EGLConfig[numConfigs]);

    assert(supportedConfigs);
    /*
     * {attributes} is your prefer
     * {supportedConfigs.get()} is system result
     */
    eglChooseConfig(display,attributes,supportedConfigs.get(),numConfigs,&numConfigs);
    assert(numConfigs);

    auto i = 0;
    // filter a config like rgb 888
    for(;i < numConfigs;i++){
        auto &cfg = supportedConfigs[i];
        EGLint r,g,b,d;
        if(eglGetConfigAttrib(display,cfg,EGL_RED_SIZE,&r)
           && eglGetConfigAttrib(display,cfg,EGL_GREEN_SIZE,&g)
           && eglGetConfigAttrib(display,cfg,EGL_BLUE_SIZE,&b)
           && eglGetConfigAttrib(display,cfg,EGL_DEPTH_SIZE,&d)
           && r ==8 && g == 8 && b == 8 && d == 8) {
            config = supportedConfigs[i];
            break;
        }
    }

    //if not match, chose first config
    if(i == numConfigs) {
        config = supportedConfigs[0];
    }

    //dont have any configs
    if(config == nullptr) {
        LOGE("unable to initialize EGLConfig");
        return -1;
    }

    eglGetConfigAttrib(display,config,EGL_NATIVE_VISUAL_ID,&format);

    //create egl surface from activity's window
    surface = eglCreateWindowSurface(display,config,engine->app->window, nullptr);
    context = eglCreateContext(display,config, nullptr, nullptr);

    //switch context,make sure. it will bind display&surface to context
    // than opengl's functions will work.
    //read/write on a same surface
    if(eglMakeCurrent(display,surface,surface,context) == EGL_FALSE) {
        LOGE("unable to egl make current");
        return -1;
    }

    eglQuerySurface(display,surface,EGL_WIDTH,&w);
    eglQuerySurface(display,surface,EGL_HEIGHT,&h);

    LOGI("surface width: %i , height:  %i",w,h);

    //save egl
    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;
    engine->state.angle = 0;

    auto opengl_info = {GL_VENDOR,GL_RENDERER,GL_VERSION,GL_EXTENSIONS};

    for(auto name : opengl_info) {
        auto info = glGetString(name);
        LOGI("opengl info : %s",info);
    }

    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_FASTEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DEPTH_TEST);

    return 0;

}


/*
 * draw
 */

static void engine_draw_frame(struct engine * engine) {
    if(engine->display == nullptr) {
        return;
    }

    //set pre-fill-color
    glClearColor(((float )engine->state.x)/engine->width,
                 engine->state.angle,
                 ((float )engine->state.y)/engine->height,
                 1);

    //white color
    //glClearColor(255,255,255,1);

    glClear(GL_COLOR_BUFFER_BIT);

    eglSwapBuffers(engine->display,engine->surface);

}


static void engine_term_display(struct engine* engine) {
    if(engine->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(engine->display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);
        if(engine->context != EGL_NO_CONTEXT) {
            eglDestroyContext(engine->display,engine->context);
        }
        if(engine->surface != EGL_NO_SURFACE) {
            eglDestroySurface(engine->display,engine->surface);
        }
    }

    engine->animation = 0;
    engine->display = EGL_NO_DISPLAY;
    engine->surface = EGL_NO_SURFACE;
    engine->context = EGL_NO_CONTEXT;

}

/*
 * android input events call back
 */

static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
    auto *engine = (struct engine*)app->userData;
    if(AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        engine->animation = 1;
        engine->state.x = AMotionEvent_getX(event,0);
        engine->state.y = AMotionEvent_getY(event,0);
        LOGI("input event : x=%i  y=%i",
             engine->state.x,
             engine->state.y);
        return 1;
    }
    return 0;
}


/*
 * android native activity's callback
 *
 */

static void engine_handle_cmd(struct android_app* app,int32_t cmd) {

    auto* engine = (struct engine*)app->userData;

    //some call backs ...
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            //alloc memory for save app's state
            engine->app->savedState = malloc(sizeof(struct saved_state));

            //do save
            *((struct saved_state*)engine->app->savedState) = engine->state;
            engine->app->savedStateSize = sizeof(struct saved_state);
            break;
        case APP_CMD_INIT_WINDOW:
            //window has initiate
            if(engine->app->window != nullptr) {
                //init egl display
                engine_init_display(engine);
                engine_draw_frame(engine);

            }
            break;
        case APP_CMD_TERM_WINDOW:
            engine_term_display(engine);
            break;
        case APP_CMD_GAINED_FOCUS:
            if(engine->accelerometerSenor != nullptr) {
                //receive app's event
                ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                                               engine->accelerometerSenor);

                ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                                               engine->accelerometerSenor,
                                               (1000L/60) * 1000);
            }
            break;
        case APP_CMD_LOST_FOCUS:
            if(engine->accelerometerSenor != nullptr) {
                ASensorEventQueue_disableSensor(engine->sensorEventQueue,
                                                engine->accelerometerSenor);
            }
            engine->animation = 0;
            engine_draw_frame(engine);
            break;


    }

}

/*
 * Linux在<dlfnc.h>库中提供了加载和处理动态连接库的系统调用
 * dlopen : 打开一个动态连接库，并返回一个类型为void*的handler,flag为打开模式，可选的模式有两种
 * RTLD_LAZY 暂缓决定，等有需要时再解出符号
 * RTLD_NOW 立即决定，返回前解除所有未决定的符号。
 * dlerror : 返回dl操作的错误，若没有出现错误，则返回NUlL，否则打印错误信息
 * dlsym : 查找动态链接库中的符号symbol,并返回该符号所在的地址
 * dlclose : 关闭动态链接库句柄
 */

ASensorManager* acquireASensorManagerInstance(android_app* app) {
    if(!app)
        return nullptr;

    //define a method-pointer name is :PF_GETINSTANCEFORPACKAGE
    // @param name :  package name
    typedef ASensorManager *(*PF_GETINSTANCEFORPACKAGE) (const char *name);

    void* androidHandler = dlopen("libandroid.so",RTLD_NOW);

    /*
     * dlsym will return void*
     * and force-transform void* to PF_GETINSTANCEFORPACKAGE
     */
    auto getInstanceForPackageFunc = (PF_GETINSTANCEFORPACKAGE)
            dlsym(androidHandler,"ASensorManager_getInstanceForPackage");

    if(getInstanceForPackageFunc) {
        JNIEnv* env = nullptr;
        //init env
        app->activity->vm->AttachCurrentThread(&env, nullptr);

        jclass android_content_context = env->GetObjectClass(app->activity->clazz);//CalculatorNativeActivity-Activity

        jmethodID midGetPackageName = env->GetMethodID(android_content_context,
                                                       "getPackageName",
                                                       "()Ljava/lang/String;");

        auto packageName = (jstring)env->CallObjectMethod(app->activity->clazz,midGetPackageName);

        const char* nativePackageName = env->GetStringUTFChars(packageName, nullptr);

        //get sensor manager
        ASensorManager *mgr = getInstanceForPackageFunc(nativePackageName);

        env->ReleaseStringUTFChars(packageName,nativePackageName);
        app->activity->vm->DetachCurrentThread();

        if(mgr) {
            dlclose(androidHandler);
            return mgr;
        }

    }

    //pointer-function
    typedef ASensorManager *(*PF_GETINSTANCE)();
    auto getInstanceFunc = (PF_GETINSTANCE)
            dlsym(androidHandler,"ASensorManager_getInstance");
    assert(getInstanceFunc);
    dlclose(androidHandler);

    return getInstanceFunc();

}






















