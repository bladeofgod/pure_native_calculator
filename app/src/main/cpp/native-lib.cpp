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
                //is user event data
                if(engine.accelerometerSenor != nullptr) {
                    ASensorEvent   event;
                    while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
                                                       &event,1) > 0) {

                    }
                }
            }

        }



    }

}

/*
 * android input events call back
 */

static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {}


/*
 * android native activity's callback
 *
 */

static void engine_handle_cmd(struct android_app* app,int32_t cmd) {

    auto* engine = (struct engine*)app->userData;
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

}






















