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



void MyGLWidget::loadModel()
{
    // Lade Model aus Datei:
    ModelLoader model ;
    bool res = model.loadObjectFromFile("P3_models/sphere_low.obj");
    // Wenn erfolgreich, generiere VBO und Index-Array
    if (res) {
        // Frage zu erwartende Array-Längen ab
        //vboLength = model.lengthOfSimpleVBO();
        vboLength = model.lengthOfVBO(0,false,true);
        iboLength = model.lengthOfIndexArray();
        // Generiere VBO und Index-Array
        vboData = new GLfloat[vboLength];
        indexData = new GLuint[iboLength];
        //model.genSimpleVBO(vboData);
        model.genVBO(vboData,0,false,true);  // With textures
        model.genIndexArray(indexData);
    }
    else {
        // Modell konnte nicht geladen werden
        assert(0) ;
    }


}



/*
void MyGLWidget::addVertice(int     verticeNo ,
                            GLfloat x ,
                            GLfloat y ,
                            GLfloat z ,
                            GLfloat r ,
                            GLfloat g ,
                            GLfloat b )
{

    vertices[verticeNo].x = x ;
    vertices[verticeNo].y = y ;
    vertices[verticeNo].z = z ;
    vertices[verticeNo].h = 1 ;
    vertices[verticeNo].r = r ;
    vertices[verticeNo].g = g ;
    vertices[verticeNo].b = b ;
    vertices[verticeNo].t = 1 ;

}
*/

void MyGLWidget::initializeGL()
{

    glEnable(GL_DEPTH_TEST);                                // Activate depth comparisons and update depth buffer

    glEnable(GL_CULL_FACE);                                 // Draw Front or Back?

    glDepthFunc(GL_LEQUAL);                                 // Specify the depth buffer

    //glShadeModel(GL_SMOOTH);                                // !Deprecated GL_FLAT or GL_SMOOTH (interpolated)

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);      // Are there Interpretations? Set hint!

    glClearDepth(1.0f);                                     // Clear value for depth buffer (used by glClear)

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);                   // Clear values used by glClear (Color-Buffer)


    //
    // fillBuffer(); - Cube
    loadModel();
    initializeTextures();
    initalizeBuffer();
    initalizeShader();

}


void MyGLWidget::resizeGL(int width, int height)
{
   // Compute aspect ratio
   height = (height == 0) ? 1 : height;  // ?

   // Set viewport to cover the whole window
   glViewport(0, 0, width, height);

   // PROJECTION (altes OpenGL)
   // Set projection matrix to a perspective projection
   /*
   glMatrixMode(GL_PROJECTION);  // Use projection matrix
   glLoadIdentity(); // Einheitsmatrix laden

   glFrustum(-0.05, 0.05, -0.05, 0.05, 0.1, 100.0);
   */

   // PROJECTION (neues OpenGL)
   projectionMatrix.setToIdentity();
   projectionMatrix.frustum(-0.05, 0.05, -0.05, 0.05, 0.1, 1000.0);
}


void MyGLWidget::paintGL()
{

    // Clear buffer to set color and alpha
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




    // Set color for drawing
    //glColor4f(1.0f, 0.0f, 0.0f, 1.0f);      // !Deprecated

    // MODEL TRANSFORMATION (Neues OpenGL)
    QMatrix4x4 modelMatrix ;
    modelMatrix.setToIdentity();
    modelMatrix.translate(0.0f, 0.0f, -7.0f);
    modelMatrix.translate(moveX, moveY, 0);
    modelMatrix.rotate(zRotation, 0, 1, 0);

    // VIEW TRANSFORMATION
    QMatrix4x4 viewMatrix ;
    viewMatrix.setToIdentity();
    viewMatrix.translate(0,0,-zoom);

    // Binde das Shader-Programm an den OpenGL-Kontext
    shaderProgram.bind();
    vbo.bind();
    ibo.bind();


    // Lokalisiere bzw. definiere die Schnittstelle für die Eckpunkte
    int attrVertices = 0;
    attrVertices = shaderProgram.attributeLocation("vert");  // #version 130

    int attrTexCoords = 3 ;
    attrTexCoords = shaderProgram.attributeLocation("texCoord"); // #version 130


    // Lokalisiere bzw. definiere die Schnittstelle für die Farben
    // P3.5 - Farben deaktivieren
    //int attrColors = 1;
    //attrColors = shaderProgram.attributeLocation("color");  // #version 130

    // Aktiviere die Verwendung der Attribute-Arrays
    shaderProgram.enableAttributeArray(attrVertices);
    //shaderProgram.enableAttributeArray(attrColors);  // P3.5 - Farben deaktivieren
    shaderProgram.enableAttributeArray(attrTexCoords);

    // MATRITZEN an den Shader übergeben
    // Lokalisiere bzw. definierte die Schnittstelle für die Transformationsmatrix
    // Die Matrix kann direkt übergeben werden, da setUniformValue für diesen Typ überladen ist.
    int unifMatrixModel = 0 ;
    unifMatrixModel = shaderProgram.uniformLocation("modelMatrix");
    Q_ASSERT(unifMatrixModel >= 0) ;
    shaderProgram.setUniformValue(unifMatrixModel,modelMatrix);

    int unifMatrixView = 0 ;
    unifMatrixView = shaderProgram.uniformLocation("viewlMatrix");
    Q_ASSERT(unifMatrixView >= 0) ;
    shaderProgram.setUniformValue(unifMatrixView,viewMatrix);

    int unifMatrixPerspective = 0 ;
    unifMatrixPerspective = shaderProgram.uniformLocation("perspectiveMatrix");
    Q_ASSERT(unifMatrixPerspective >= 0) ;
    shaderProgram.setUniformValue(unifMatrixPerspective,projectionMatrix);


    // Fülle die Attribute-Buffer mit den konkreten Daten
    /*
    int offset = 0 ;
    int stride = 4 * sizeof(GLfloat) ;
    shaderProgram.setAttributeBuffer(attrVertices,GL_FLOAT,offset,4,stride);
    */

    // P3.5 - Farben entfernen
    //offset += 4 * sizeof(GLfloat);
    //shaderProgram.setAttributeBuffer(attrColors,GL_FLOAT,offset,4,stride);

    qTex->bind();
    // Übergebe die Textur an die Uniform Variable
    // Die 0 steht dabei für die verwendete Unit (0=Standard)
    shaderProgram.setUniformValue("texture",0);
    // Ein paar Hilfsvariablen - die 8 steht für
    // 4 Eckpunktkoordinaten + 4 Texturkoordinaten
    int offset = 0 ;
    size_t stride = 8 * sizeof(GLfloat);
    shaderProgram.setAttributeBuffer(attrVertices,GL_FLOAT,offset,4,stride);
    offset += 4*sizeof(GLfloat);
    shaderProgram.setAttributeBuffer(attrTexCoords,GL_FLOAT,offset,4,stride);



/*
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer ( 4 ,                               // Anzahl der Koordinaten des Vertex
                      GL_FLOAT,                         // Datentyp
                      sizeof(GLfloat)*8,                // Wo sind die nächsten Koordinanten.
                      (char*)NULL+0);                   // Offset
    glColorPointer  ( 4,                                // Anzahl Daten pro Farbe
                      GL_FLOAT,                         // Datentyp
                      sizeof(GLfloat)*8,                // Wo findet man die nächste Farbe
                      (char*)NULL+sizeof(GLfloat)*4) ;  // Offset (Wo steht die erste Farbe)
*/
    glDrawElements ( GL_TRIANGLES,                      // Primitive
                     iboLength,                         // Wieviele Indizies
                     GL_UNSIGNED_INT,                   // Datentyp
                     0);                                // 0 = Nehme den Index Buffer


/*
   // modelMatrix.setToIdentity();
    modelMatrix.translate(10,0,0);
    shaderProgram.setUniformValue(unifMatrixModel,modelMatrix);

    glDrawElements ( GL_TRIANGLES,                      // Primitive
                     iboLength,                         // Wieviele Indizies
                     GL_UNSIGNED_INT,                   // Datentyp
                     0);                                // 0 = Nehme den Index Buffer
*/
/*
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
*/

    // Deaktiviere die Verwendung der Attribute Arrays
    shaderProgram.disableAttributeArray(attrVertices);
    //shaderProgram.disableAttributeArray(attrColors);
    shaderProgram.disableAttributeArray(attrTexCoords);

    vbo.release();
    ibo.release();
    qTex->release();
    shaderProgram.release();

}

void MyGLWidget::initalizeBuffer()
{
    // Erzeuge vbo
    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
//    vbo.allocate( vertices,                                 // Vertex-Array
//                  sizeof(GLfloat) * 8 * verticesCount );    // Speicherbedarf pro Vertex
    vbo.allocate(vboData,sizeof(GLfloat) * vboLength);
    vbo.release();

    // Erzeuge Index-Buffer
    ibo.create();
    ibo.bind();
    ibo.setUsagePattern(QOpenGLBuffer::StaticDraw);
//    ibo.allocate( indicies,                                 // Index-Array
//                  sizeof(GLubyte) * 24 );                   // Speicherbedarf Indizies
    ibo.allocate(indexData,sizeof(GLuint) * iboLength);
    ibo.release();
}

void MyGLWidget::fillBuffer()
{
    /*
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
    */

}

void MyGLWidget::initalizeShader()
{


    // Initialisierung Shader
    // Lade Shader-Source aus externen Dateien
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                          ":/default130.vert") ;

    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                          ":/default130.frag") ;
    // Kompiliere und linke die Shader-Programme
    shaderProgram.link() ;


}

void MyGLWidget::initializeTextures()
{
    // Initialization
    glEnable(GL_TEXTURE_2D);

    qTex = new QOpenGLTexture(QImage(":/mercurymap.jpg").mirrored()) ;
    qTex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    qTex->setMagnificationFilter(QOpenGLTexture::Linear);
    //Q_ASSERT( qTex->textureId() == 0 ) ;


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

