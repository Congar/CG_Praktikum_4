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
#include "sonne.h"
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

    // Datenspeicher Vertices & Indicies  
    GLfloat*     vboData ;
    GLuint*      indexData ;
    unsigned int vboLength ;
    unsigned int iboLength ;
    bool         hasTexureCoord ;
    // Buffer
    QOpenGLBuffer vbo;
    QOpenGLBuffer ibo;
    void initalizeBuffer();
    void fillBuffer();

    // Matritzen
    QMatrix4x4 projectionMatrix ;
    QMatrix4x4 viewMatrix ;
    QMatrix4x4 modelMatrix ;
    std::stack<QMatrix4x4> modelMatrixStack ;

    // Camera - Parameter
    QVector3D cameraPos = QVector3D(0.0f, 0.0f, 3.0f);
    QVector3D cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    QVector3D cameraUp = QVector3D(0.0f, 1.0f, 0.0f);
    GLfloat   yaw   = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
    GLfloat   pitch =   0.0f;

    // Shader
    QOpenGLShaderProgram shaderProgram[2];
    void initalizeShader();
    void initializeShaderProgramDefault();
    void initializeShaderProgramNormalen();

    // Lokalisiere bzw. definiere die Schnittstelle für die Eckpunkte und Texturen, ..
    int attrVerticesDefault ;
    int attrTexCoordsDefault ;
    int attrNormalsDefault ;
    int unifMatrixPerspectiveDefault ;
    int unifMatrixViewDefault ;
    int unifka ; // ambient

    int attrVerticesNormalen ;
    int attrTexCoordsNormalen ;
    int attrNormalsNormalen ;
    int unifMatrixPerspectiveNormalen ;
    int unifMatrixViewNormalen ;
    int unifTimer ;

    // Beleuchtung
    QVector3D ka = QVector3D(0.2,0.2,0.2);  // Ambient


    // Textures

    QOpenGLTexture *textures[13];
    void initializeTextures();

    // Modell
    void loadModel();


    // Planeten
    Planet merkur, venus, erde, erdemond, mars, phobos, deimos, jupiter, saturn, uranus, neptun;
    Sonne sonne ;
    void initializePlanets();


    // Zeitmessung
    QElapsedTimer tmrRender ; // Misst vergangene Zeit zwischen Rendern ;

    int unifMatrixModelDefault = 0 ;       // Uniform Value für Model Matrix. Hier deklariert, um Referenz an die Planten Klasse zu übergeben.
    int unifMatrixModelNormalen = 0 ;
    int elapsedTime = 0 ;           // Zeit zwischen dem Rendern. Referenz wird an die Planeten übergeben, damit die damit arbeiten können.
    int counter = 0 ;
    float time = 0.0;


    // Paused
    bool paused = false ;

protected:

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();


    // Events
    void wheelEvent(QWheelEvent * event );
    void keyPressEvent(QKeyEvent *event);


public:
    MyGLWidget();
    MyGLWidget(QWidget *parent); // Konstruktur noch angeben?
    ~MyGLWidget();





public slots:
    void receiveRotationZ(int degrees);

signals:
    void zoomFactorChanged(int);


};

#endif // MYGLWIDGET_H
