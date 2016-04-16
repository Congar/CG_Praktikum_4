#include "MyGLWidget.h"
#include <iostream>



MyGLWidget::MyGLWidget()
{

}



MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(parent)
    , vbo(QOpenGLBuffer::VertexBuffer)
    , ibo(QOpenGLBuffer::IndexBuffer)
{

     std::cout << glGetString (GL_VERSION ) ;
     setFocusPolicy(Qt::StrongFocus);               // To catch the KeyPressEvents.
}



void MyGLWidget::addVertice(int     verticeNo ,
                            GLfloat x ,
                            GLfloat y ,
                            GLfloat z ,
                            GLfloat r ,
                            GLfloat g ,
                            GLfloat b )
{
    int arrayPos = verticeNo*sizeof(Vertex);

    vertices[verticeNo].x = x ;
    vertices[verticeNo].y = y ;
    vertices[verticeNo].z = z ;
    vertices[verticeNo].r = r ;
    vertices[verticeNo].g = g ;
    vertices[verticeNo].b = b ;
}


void MyGLWidget::initializeGL()
{

    glEnable(GL_DEPTH_TEST);                                // Activate depth comparisons and update depth buffer

    glEnable(GL_CULL_FACE);                                 // Draw Front or Back?

    glDepthFunc(GL_LEQUAL);                                 // Specify the depth buffer

    glShadeModel(GL_SMOOTH);                                // !Deprecated GL_FLAT or GL_SMOOTH (interpolated)

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);      // Are there Interpretations? Set hint!

    glClearDepth(1.0f);                                     // Clear value for depth buffer (used by glClear)

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);                   // Clear values used by glClear (Color-Buffer)

    //
    fillBuffer();
    initalizeBuffer();

}


void MyGLWidget::resizeGL(int width, int height)
{
   // Compute aspect ratio
   height = (height == 0) ? 1 : height;  // ?

   // Set viewport to cover the whole window
   glViewport(0, 0, width, height);

   // Set projection matrix to a perspective projection
   glMatrixMode(GL_PROJECTION);  // Use projection matrix
   glLoadIdentity(); // Einheitsmatrix laden

   glFrustum(-0.05, 0.05, -0.05, 0.05, 0.1, 100.0);

}


void MyGLWidget::paintGL()
{

    // Test
    glClear(GL_COLOR_BUFFER_BIT);

    // Clear buffer to set color and alpha
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);            // !Deprecated // Which matrix is active?

    // Apply model view transformations
    glMatrixMode(GL_MODELVIEW);             // !Deprecated // Which matrix is active?
    glLoadIdentity(); // !Deprecated // Einheitsmatrix laden

    // Transformation
    glTranslatef(0.0f, 0.0f, -7.0f);        // !Deprecated // Initial
    //glTranslatef(0.0f, 0.0f, -zoom); // Zoom
    glTranslatef(moveX, moveY, -zoom); // !Deprecated // Zoom

    //glRotatef(-45, 0, 0, 1);
    glRotatef((zRotation), 0, 1, 0);        // !Deprecated

    // Set color for drawing
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);      // !Deprecated

    // Zeichnen
    vbo.bind();
    ibo.bind();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer ( 3 ,                               // Anzahl der Koordinaten des Vertex
                      GL_FLOAT,                         // Datentyp
                      sizeof(GLfloat)*6,                // Wo sind die nächsten Koordinanten.
                      (char*)NULL+0);                   // Offset
    glColorPointer  ( 3,                                // Anzahl Daten pro Farbe
                      GL_FLOAT,                         // Datentyp
                      sizeof(GLfloat)*6,                // Wo findet man die nächste Farbe
                      (char*)NULL+sizeof(GLfloat)*3) ;  // Offset (Wo steht die erste Farbe)

    glDrawElements ( GL_QUADS,                          // Primitive
                     24,                                // Wieviele Indizies
                     GL_UNSIGNED_BYTE,                  // Datentyp
                     0);                                // 0 = Nehme den Index Buffer

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    vbo.release();
    ibo.release();

}

void MyGLWidget::initalizeBuffer()
{
    // Erzeuge vbo
    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.allocate( vertices,                                 // Vertex-Array
                  sizeof(GLfloat) * 6 * verticesCount );    // Speicherbedarf pro Vertex
    vbo.release();
    // Erzeuge Index-Buffer
    ibo.create();
    ibo.bind();
    ibo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    ibo.allocate( indicies,                                 // Index-Array
                  sizeof(GLubyte) * 4 * 6);                 // Speicherbedarf Indizies
    ibo.release();
}

void MyGLWidget::fillBuffer()
{
    // Vertices
    // Back
    addVertice(0 , -1.0f , -1.0f , -1.0f , 1 , 0 , 0 );
    addVertice(1 ,  1.0f , -1.0f , -1.0f , 0 , 1 , 0 );
    addVertice(2 ,  1.0f ,  1.0f , -1.0f , 1 , 0 , 1 );
    addVertice(3 , -1.0f ,  1.0f , -1.0f , 0 , 0 , 1 );
    // Front
    addVertice(4 , -1.0f , -1.0f ,  1.0f , 1 , 0 , 1 );
    addVertice(5 ,  1.0f , -1.0f ,  1.0f , 1 , 1 , 0 );
    addVertice(6 ,  1.0f ,  1.0f ,  1.0f , 0 , 1 , 1 );
    addVertice(7 , -1.0f ,  1.0f ,  1.0f , 1 , 0 , 0 );

    // Indicies - Cube
    // Front
    indicies[0] = 4 ;
    indicies[1] = 5 ;
    indicies[2] = 6 ;
    indicies[3] = 7 ;
    // Back
    indicies[4] = 3 ;
    indicies[5] = 2 ;
    indicies[6] = 1 ;
    indicies[7] = 0 ;
    // Left
    indicies[8] = 7 ;
    indicies[9] = 3 ;
    indicies[10] = 0 ;
    indicies[11] = 4 ;
    // Right
    indicies[12] = 6 ;
    indicies[13] = 5 ;
    indicies[14] = 1 ;
    indicies[15] = 2 ;
    // Top
    indicies[16] = 6 ;
    indicies[17] = 2 ;
    indicies[18] = 3 ;
    indicies[19] = 7 ;
    // Bottom
    indicies[20] = 4 ;
    indicies[21] = 0 ;
    indicies[22] = 1 ;
    indicies[23] = 5 ;

}


void MyGLWidget::wheelEvent ( QWheelEvent * event )
{
    zoom += event->delta() / 120 ;
    emit zoomFactorChanged(zoom);
    glDraw() ;                      // !Deprecated
}


void MyGLWidget::keyPressEvent(QKeyEvent *event)
{

    switch ( event->key()) {
        case Qt::Key_W : moveY += 0.1 ;
             break ;
        case Qt:: Key_S : moveY -= 0.1 ;
             break ;
        case Qt::Key_A : moveX -= 0.1 ;
             break ;
        case Qt::Key_D : moveX += 0.1 ;
             break ;
        default : QGLWidget::keyPressEvent(event) ;
    }

    glDraw() ;                       // !Deprecated
}


void MyGLWidget::receiveRotationZ( int degrees )
{
    zRotation = degrees ;
    glDraw();                       // !Deprecated
}

