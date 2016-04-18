#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H


#include <QOpenGLBuffer>        // Buffer
#include <QGLWidget>
#include <QWidget>
#include <QInputEvent>
//#include <QOpenGLFunctions_3_1>
#include <QOpenGLShaderProgram>  // Shader
#include <stack>
#include <modelloader.h>

#include <iostream>


typedef struct {
    GLfloat x, y, z, h;
    GLfloat r, g, b, t;
} Vertex;

static const int verticesCount = 8 ;


class MyGLWidget : public QGLWidget
{


private:

    Q_OBJECT    // declare own signals and slots (MOC-System)

    // Transformation-Variables
    int     zRotation = 0 ;
    GLfloat zoom      = 0 ;
    GLfloat moveY     = 0 ;
    GLfloat moveX     = 0 ;

    // Datenspeicher Vertices & Indicies
    // Statisch
    //Vertex   vertices[verticesCount] ;
    //GLubyte  indicies[24] ; // 6 Flächen mit je 4 Indizies
    // Dynamisch
    GLfloat* vboData ;
    GLuint* indexData ;
    unsigned int vboLength ;
    unsigned int iboLength ;

    // Matritzen
    QMatrix4x4 projectionMatrix ;
    std::stack<QMatrix4x4> modelViewMatrixStack ;

    // Buffer
    QOpenGLBuffer vbo;
    QOpenGLBuffer ibo;


    // Shader
    QOpenGLShaderProgram shaderProgram;

    // Modelle
    void loadModel();


    // Help
   /*
    void addVertice(int     verticeNo ,
                    GLfloat x ,
                    GLfloat y ,
                    GLfloat z ,
                    GLfloat r ,
                    GLfloat g ,
                    GLfloat b ) ;
    */

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    // Buffer
    void initalizeBuffer();
    void fillBuffer();

    // Shader
    void initalizeShader();

    // Events
    void wheelEvent(QWheelEvent * event );
    void keyPressEvent(QKeyEvent *event);


public:
    MyGLWidget();
    MyGLWidget(QWidget *parent); // Konstruktur noch angeben?






public slots:
    void receiveRotationZ(int degrees);

signals:
    void zoomFactorChanged(int);


};

#endif // MYGLWIDGET_H
