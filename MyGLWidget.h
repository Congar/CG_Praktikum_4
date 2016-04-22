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
#include <QOpenGLTexture>

#include <iostream>
#include "planet.h"
#include <QTimer>
#include <QElapsedTimer>



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
    GLfloat moveZ     = -5 ;
    GLfloat lookY     = 0 ;
    GLfloat lookX     = 0 ;

    // Datenspeicher Vertices & Indicies  
    // Dynamisch
    GLfloat* vboData ;
    GLuint* indexData ;
    unsigned int vboLength ;
    unsigned int iboLength ;

    // Matritzen
    QMatrix4x4 projectionMatrix ;
    std::stack<QMatrix4x4> modelMatrixStack ;

    // Buffer
    QOpenGLBuffer vbo;
    QOpenGLBuffer ibo;

    // Shader
    QOpenGLShaderProgram shaderProgram;

    // Textures
    QOpenGLTexture* qTex ;

    // Modelle
    void loadModel();

    // Planeten
    void createPlantes();

    // Zeitmessung
    QElapsedTimer tmrRender ; // Misst vergangene Zeit zwischen Rendern ;


protected:

    int unifMatrixModel = 0 ;       // Uniform Value für Model Matrix. Hier deklariert, um Referenz an die Planten Klasse zu übergeben.
    int elapsedTime = 0 ;           // Zeit zwischen dem Rendern. Referenz wird an die Planeten übergeben, damit die damit arbeiten können.

    // Planeten
    Planet sonne, merkur, venus, erde, erdemond, mars, phobos, deimos, jupiter, saturn, uranus, neptun;

    // Paused
    bool paused = false ;

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();


    // Buffer
    void initalizeBuffer();
    void fillBuffer();

    // Shader
    void initalizeShader();

    // Textures
    void initializeTextures();

    // Planeten
    void initializePlanets();

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
