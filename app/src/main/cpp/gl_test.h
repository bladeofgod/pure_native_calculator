//
// Created by 45010 on 2021/6/27.
//

//just for test

#ifndef PURENATIVECALCULATORDEMO_GL_TEST_H
#define PURENATIVECALCULATORDEMO_GL_TEST_H

#include "app_engine.h"
#include "rect_view.h"
#include "gl_painter.h"


static void gl_draw_some_rect(struct engine *engine) {
    float bg[4] = {
            255,0,85,1
    };

    RectView r1(200,100,Offset(50,100),bg);
    RectView r2(200,100,Offset(150,100),bg);

    GlPainter painter(engine);

    painter.drawRect(r1);
    painter.drawRect(r2);
    painter.flushCanvas();


}


//static void gl_draw_triangle(struct engine *engine) {
//    ///三个顶点
//    GLfloat vVertices[] = {
//            0.0f, 0.5f, 0.0f,
//            -0.5f, -0.5f, 0.0f,
//            0.5f, -0.5f, 0.0f
//    };
//
//    // 1. 通知OpenGL ES用于绘制的2D渲染表面的原点(x,y)坐标，宽度和高度
//    glViewport(0,0,engine->width,engine->height);
//    // 2. 清除颜色缓冲区；有颜色、深度和模板缓冲区
//    glClear(GL_COLOR_BUFFER_BIT);
//    // 3. 将程序设置为活程序
//    glUseProgram(engine->programObject);
//    // void glVertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized,
//    // GLsizei stride, const void *pointer);
//    // index为第几个属性，属性有顶点的位置为0/纹理为1/法线为3；size为一个顶点所有数据的个数，这里XYZ为3个；
//    // type为顶点描述数据的类型，这里为FLOAT；normalized为是否需要显卡把数据归一化到-1到+1区间，这里不需要为FALSE
//    // stride连续顶点属性之间的偏移量，0表示他们为紧密排列在一起的。pointer为顶点数组
//    // 4. 加载顶点位置到GL中
//    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,vVertices);
//    // 5. 使用顶点数组顶点位置属性，也就是0属性；如果什么都不使用的话，就使用常量顶点属性0
//    glEnableVertexAttribArray(0);
//    // 6. 绘制三角形
//    glDrawArrays(GL_TRIANGLES, 0, 3);
//
//    eglSwapBuffers(engine->display,engine->surface);
//}


//static void gl_draw_rect(struct engine *engine) {
//
//    GLfloat vVertices[] = {
//            -0.5f, 0.5f, 0.0f,
//            0.5f, 0.5f, 0.0f,
//            0.5f, -0.5f, 0.0f,
//            -0.5f, -0.5f, 0.0f
//    };
//
//    // 1. 通知OpenGL ES用于绘制的2D渲染表面的原点(x,y)坐标，宽度和高度
//    glViewport(0,0,engine->width,engine->height);
//    // 2. 清除颜色缓冲区；有颜色、深度和模板缓冲区
//    glClear(GL_COLOR_BUFFER_BIT);
//    // 3. 将程序设置为活程序
//    glUseProgram(engine->programObject);
//    // void glVertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized,
//    // GLsizei stride, const void *pointer);
//    // index为第几个属性，属性有顶点的位置为0/纹理为1/法线为3；size为一个顶点所有数据的个数，这里XYZ为3个；
//    // type为顶点描述数据的类型，这里为FLOAT；normalized为是否需要显卡把数据归一化到-1到+1区间，这里不需要为FALSE
//    // stride连续顶点属性之间的偏移量，0表示他们为紧密排列在一起的。pointer为顶点数组
//    // 4. 加载顶点位置到GL中
//    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,vVertices);
//    // 5. 使用顶点数组顶点位置属性，也就是0属性；如果什么都不使用的话，就使用常量顶点属性0
//    glEnableVertexAttribArray(0);
//    // 6. 设置线粗
//    glLineWidth(10);
//    // 7. 绘制矩形
//    glDrawArrays(GL_LINE_LOOP, 0, 4);
//
//    eglSwapBuffers(engine->display,engine->surface);
//}

#endif //PURENATIVECALCULATORDEMO_GL_TEST_H
