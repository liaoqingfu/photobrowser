#include "qplaywidget.h"

#include <QOpenGLTexture>

#include <QOpenGLBuffer>

#include <QMouseEvent>


QPlayWidget::QPlayWidget(QWidget *parent,int width,int height):QOpenGLWidget(parent)
{

    textureUniformY = 0;

    textureUniformU = 0;

    textureUniformV = 0;

    id_y = 0;

    id_u = 0;

    id_v = 0;

    m_pBufYuv420p = NULL;

    m_pVSHader = NULL;

    m_pFSHader = NULL;

    m_pShaderProgram = NULL;

    m_pTextureY = NULL;

    m_pTextureU = NULL;

    m_pTextureV = NULL;

    //m_pYuvFile = NULL;

    //m_nVideoH = m_frame_height;

    //m_nVideoW = m_frame_width;

    m_pBufYuv420p = (unsigned char *)malloc(sizeof(unsigned char) * width * height * 3 / 2);
    memset(m_pBufYuv420p,0,sizeof(unsigned char) * width * height * 3 / 2);

    isPause = false;


    m_frame_width = width;

    m_frame_height = height;

}

QPlayWidget::~QPlayWidget()
{
    qDebug()<<"delete opengl";
    // free(m_pBufYuv420p);
}


void QPlayWidget::PlayOneFrame(unsigned char *yuv)
{

    m_pBufYuv420p = yuv;

    update();

    return;

}

void QPlayWidget::initializeGL()
{

    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);

    //现代opengl渲染管线依赖着色器来处理传入的数据

    //着色器：就是使用openGL着色语言(OpenGL Shading Language, GLSL)编写的一个小函数,

    //       GLSL是构成所有OpenGL着色器的语言,具体的GLSL语言的语法需要读者查找相关资料

    //初始化顶点着色器 对象

    m_pVSHader = new QOpenGLShader(QOpenGLShader::Vertex, this);

    //顶点着色器源码

    const char *vsrc = "attribute vec4 vertexIn; \
            attribute vec2 textureIn; \
    varying vec2 textureOut;  \
    void main(void)           \
    {                         \
        gl_Position = vertexIn; \
        textureOut = textureIn; \
    }";
    //编译顶点着色器程序
    bool bCompile = m_pVSHader->compileSourceCode(vsrc);
    if(!bCompile)
    {

    }

    //初始化片段着色器 功能gpu中yuv转换成rgb

    m_pFSHader = new QOpenGLShader(QOpenGLShader::Fragment, this);

    //片段着色器源码

    const char *fsrc = "varying vec2 textureOut; \
            uniform sampler2D tex_y; \
    uniform sampler2D tex_u; \
    uniform sampler2D tex_v; \
    void main(void) \
    { \
        vec3 yuv; \
        vec3 rgb; \
        yuv.x = texture2D(tex_y, textureOut).r; \
        yuv.y = texture2D(tex_u, textureOut).r - 0.5; \
        yuv.z = texture2D(tex_v, textureOut).r - 0.5; \
        rgb = mat3( 1,       1,         1, \
                    0,       -0.39465,  2.03211, \
                    1.13983, -0.58060,  0) * yuv; \
        gl_FragColor = vec4(rgb, 1); \
    }";

    //将glsl源码送入编译器编译着色器程序

    bCompile = m_pFSHader->compileSourceCode(fsrc);

    if(!bCompile)

    {
        qDebug()<<false;
    }

#define PROGRAM_VERTEX_ATTRIBUTE 0

#define PROGRAM_TEXCOORD_ATTRIBUTE 1

    //创建着色器程序容器

    m_pShaderProgram = new QOpenGLShaderProgram;

    //将片段着色器添加到程序容器

    m_pShaderProgram->addShader(m_pFSHader);

    //将顶点着色器添加到程序容器

    m_pShaderProgram->addShader(m_pVSHader);

    //绑定属性vertexIn到指定位置ATTRIB_VERTEX,该属性在顶点着色源码其中有声明

    m_pShaderProgram->bindAttributeLocation("vertexIn", ATTRIB_VERTEX);

    //绑定属性textureIn到指定位置ATTRIB_TEXTURE,该属性在顶点着色源码其中有声明

    m_pShaderProgram->bindAttributeLocation("textureIn", ATTRIB_TEXTURE);

    //链接所有所有添入到的着色器程序

    m_pShaderProgram->link();

    //激活所有链接

    m_pShaderProgram->bind();

    //读取着色器中的数据变量tex_y, tex_u, tex_v的位置,这些变量的声明可以在

    //片段着色器源码中可以看到

    textureUniformY = m_pShaderProgram->uniformLocation("tex_y");

    textureUniformU =  m_pShaderProgram->uniformLocation("tex_u");

    textureUniformV =  m_pShaderProgram->uniformLocation("tex_v");

    // 顶点矩阵
    /*
    static const GLfloat vertexVertices[] = {

        -1.0f, -1.0f,

         1.0f, -1.0f,

         -1.0f, 1.0f,

         1.0f, 1.0f,

    };
*/
    static const GLfloat vertexVertices[] = {

        -1.0f, -1.0f,

        1.0f, -1.0f,

        -1.0f, 1.0f,

        1.0f, 1.0f,

    };

    //纹理矩阵

    static const GLfloat textureVertices[] = {

        0.0f,  1.0f,

        1.0f,  1.0f,

        0.0f,  0.0f,

        1.0f,  0.0f,

    };

    //设置属性ATTRIB_VERTEX的顶点矩阵值以及格式

    glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, vertexVertices);

    //设置属性ATTRIB_TEXTURE的纹理矩阵值以及格式

    glVertexAttribPointer(ATTRIB_TEXTURE, 2, GL_FLOAT, 0, 0, textureVertices);

    //启用ATTRIB_VERTEX属性的数据,默认是关闭的

    glEnableVertexAttribArray(ATTRIB_VERTEX);

    //启用ATTRIB_TEXTURE属性的数据,默认是关闭的

    glEnableVertexAttribArray(ATTRIB_TEXTURE);

    //分别创建y,u,v纹理对象

    m_pTextureY = new QOpenGLTexture(QOpenGLTexture::Target2D);

    m_pTextureU = new QOpenGLTexture(QOpenGLTexture::Target2D);

    m_pTextureV = new QOpenGLTexture(QOpenGLTexture::Target2D);

    m_pTextureY->create();

    m_pTextureU->create();

    m_pTextureV->create();

    //获取返回y分量的纹理索引值

    id_y = m_pTextureY->textureId();

    //获取返回u分量的纹理索引值

    id_u = m_pTextureU->textureId();

    //获取返回v分量的纹理索引值

    id_v = m_pTextureV->textureId();

    glClearColor(0.0,0.0,0.0,0.0);//设置背景色

    //qDebug("addr=%x id_y = %d id_u=%d id_v=%d\n", this, id_y, id_u, id_v);

}

void QPlayWidget::resizeGL(int w, int h)

{

    if(h == 0)// 防止被零除

    {

        h = 1;// 将高设为1

    }

    //设置视口

    glViewport(0,0, w,h);

}

void QPlayWidget::paintGL()
{

    //qDebug()<<"paint";

    //激活纹理单元GL_TEXTURE0
    if(isPause)
        return;

    glActiveTexture(GL_TEXTURE0);

    //使用来自y数据生成纹理

    glBindTexture(GL_TEXTURE_2D, id_y);

    //使用内存中m_pBufYuv420p数据创建真正的y数据纹理

    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RED_EXT, m_frame_width, m_nVideoH, 0, GL_RED_EXT, GL_UNSIGNED_BYTE, m_pBufYuv420p);
    glTexImage2D ( GL_TEXTURE_2D, 0, GL_LUMINANCE, m_frame_width, m_frame_height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, m_pBufYuv420p);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    m_pShaderProgram->setUniformValue(textureUniformY, 0);
    //加载u数据纹理

    glActiveTexture(GL_TEXTURE1);//激活纹理单元GL_TEXTURE1

    glBindTexture(GL_TEXTURE_2D, id_u);

    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RED_EXT, m_frame_width/2, m_frame_height/2, 0, GL_RED_EXT, GL_UNSIGNED_BYTE, (char*)m_pBufYuv420p+m_frame_width*m_nVideoH);
    glTexImage2D ( GL_TEXTURE_2D, 0, GL_LUMINANCE, m_frame_width/2, m_frame_height/2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, (char*)m_pBufYuv420p+m_frame_width*m_frame_height);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    m_pShaderProgram->setUniformValue(textureUniformU, 1);
    //加载v数据纹理

    glActiveTexture(GL_TEXTURE2);//激活纹理单元GL_TEXTURE2

    glBindTexture(GL_TEXTURE_2D, id_v);

    glTexImage2D ( GL_TEXTURE_2D, 0, GL_LUMINANCE, m_frame_width/2, m_frame_height/2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, (char*)m_pBufYuv420p+m_frame_width*m_frame_height*5/4);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    m_pShaderProgram->setUniformValue(textureUniformV, 2);


    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    return;

}
