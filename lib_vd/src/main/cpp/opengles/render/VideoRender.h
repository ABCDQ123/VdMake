#ifndef VDMAKE_VIDEORENDER_H
#define VDMAKE_VIDEORENDER_H

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "FaceCheck.h"
#include "PixImage.h"
#include "GLUtils.h"
#include "Log.h"

#include <thread>
#include <mutex>
#include <unistd.h>


typedef void (*OnRenderFrameCallback)(void *, int, int, int, uint8_t *); // 回调函数 -> Record

class VideoRender {

public:

    //face
    void onFace(char *face, char *eye, char *nose, char *mouth);

    //normal
    void onBuffer(int format, int width, int height, int lineSize[3], uint8_t *data);

    void onBuffer(PixImage *pix);

    uint8_t *onFrameBuffer();

    int onFrameBufferSize();

    void onCamera(bool camera);

    void onRotate(float viewRot, int modelRot);

    void onSurfaceCreated();

    void onSurfaceChanged(int w, int h);

    void onMatrix(const char *gl_name, float viewRot, float modelRot);

    void onDrawFrame();

    void onResume();

    void onPause();

    void onStop();

    void onRelease();

    void onFrameBufferUpdate();

    OnRenderFrameCallback m_RenderFrameCallback = nullptr;
    void *m_CallbackContext = nullptr;

    void SetRenderCallback(void *ctx, OnRenderFrameCallback callback) {
        m_CallbackContext = ctx;
        m_RenderFrameCallback = callback;
    }

protected:
    //数据
    PixImage *m_Image = nullptr;
    int m_Width_display = 0;
    int m_Height_display = 0;
    uint8_t *m_data = nullptr;
    int m_dataSize = 0;
    float m_ViewRot = 0.0f;
    float m_ModelRot = 0.0f;
    bool m_CameraData = false;

    //VBO
    GLuint m_VBO[4];
    //显示部分
    GLuint m_Program;
    GLuint m_Texture[3];
    GLuint m_VAO[1];
    //离屏部分
    GLuint m_Program_Fbo_YUV420P;
    GLuint m_Program_Fbo_NV21;
    GLuint m_Program_Fbo_RGB;
    GLuint m_Program_Fbo_YUV420P_Face;
    GLuint m_Texture_Fbo[1];
    GLuint m_VAO_Fbo[1];
    GLuint m_Fbo[1];
    //互斥锁
    static std::mutex m_Mutex;
    //===
    volatile bool m_Interrupt = false;

private:
    //opencv
    FaceCheck *m_Face = nullptr;
    std::vector<cv::Rect> faces;
    std::vector<cv::Rect> eyes;
    std::vector<cv::Rect> noses;
    std::vector<cv::Rect> mouths;
    //线程
    std::thread *m_Thread_cv = nullptr;
    volatile int m_Interrupt_cv = 0;
    //互斥锁
    std::mutex m_Mutex_cv;

    static void onFaceLoop(VideoRender *p);
};


#endif //VDMAKE_VIDEORENDER_H
