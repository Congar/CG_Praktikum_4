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

     QTimer *timer = new QTimer(this) ;
     connect(timer, SIGNAL(timeout()),this,SLOT(updateGL()));
     timer->start(0);

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


 void MyGLWidget::createPlantes()
 {


 }




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


    // MODEL TRANSFORMATION (Neues OpenGL)
    QMatrix4x4 modelMatrix ;
    // VIEW TRANSFORMATION
    QMatrix4x4 viewMatrix ;



    // Binde das Shader-Programm an den OpenGL-Kontext
    shaderProgram.bind();
    vbo.bind();
    ibo.bind();





    // Lokalisiere bzw. definiere die Schnittstelle für die Eckpunkte
    int attrVertices = 0;
    attrVertices = shaderProgram.attributeLocation("vert");  // #version 130

    int attrTexCoords = 3 ;
    attrTexCoords = shaderProgram.attributeLocation("texCoord"); // #version 130

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


    int unifMatrixView = 0 ;
    unifMatrixView = shaderProgram.uniformLocation("viewlMatrix");
    Q_ASSERT(unifMatrixView >= 0) ;


    int unifMatrixPerspective = 0 ;
    unifMatrixPerspective = shaderProgram.uniformLocation("perspectiveMatrix");
    Q_ASSERT(unifMatrixPerspective >= 0) ;
    shaderProgram.setUniformValue(unifMatrixPerspective,projectionMatrix);

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


    // VIEW TRANSFORMATION

    viewMatrix.setToIdentity();
     viewMatrix.translate(-moveX,-moveY,-5);
    viewMatrix.translate(-moveX,-moveY,-zoom);
    shaderProgram.setUniformValue(unifMatrixView,viewMatrix);

    // MODEL TRANSFORMATION (Neues OpenGL)
/*
    modelMatrix.setToIdentity();
    modelMatrix.translate(0.0f, 0.0f, -7.0f);
    modelMatrix.translate(moveX, moveY, 0);
    modelMatrix.rotate(zRotation, 0, 1, 0);
 */

    // Initialisierung des Modells
    modelMatrix.setToIdentity();
    modelMatrixStack.push(modelMatrix);

    degrees += 1 ;

    // Logische Anordnung der Planeten
    Planet sonne   (&shaderProgram, unifMatrixModel, &modelMatrixStack, iboLength, 0    , 0             , -degrees  , 1) ;
    Planet merkur  (&shaderProgram, unifMatrixModel, &modelMatrixStack, iboLength, 10   , 0.05*degrees  , 0  , 0.07) ;
    Planet venus   (&shaderProgram, unifMatrixModel, &modelMatrixStack, iboLength, 14   , 2*degrees     , 0  , 0.1) ;

    Planet erde    (&shaderProgram, unifMatrixModel, &modelMatrixStack, iboLength, 18   , 0             , 0  , 0.1) ;
    Planet erdemond(&shaderProgram, unifMatrixModel, &modelMatrixStack, iboLength, 1    , 0             , 0  , 0.06) ;

    Planet mars    (&shaderProgram, unifMatrixModel, &modelMatrixStack, iboLength, 25   , 0             , 0  , 0.08) ;
    Planet phobos  (&shaderProgram, unifMatrixModel, &modelMatrixStack, iboLength, 1    , 0             , 0  , 0.03) ;
    Planet deimos  (&shaderProgram, unifMatrixModel, &modelMatrixStack, iboLength, 2    , 0             , 0  , 0.03) ;

    Planet jupiter (&shaderProgram, unifMatrixModel, &modelMatrixStack, iboLength, 40   , 0             , 0  , 0.3) ;
    Planet saturn  (&shaderProgram, unifMatrixModel, &modelMatrixStack, iboLength, 50   , 0             , 0  , 0.25) ;
    Planet uranus  (&shaderProgram, unifMatrixModel, &modelMatrixStack, iboLength, 60   , 0             , 0  , 0.15) ;
    Planet neptun  (&shaderProgram, unifMatrixModel, &modelMatrixStack, iboLength, 70   , 0             , 0  , 0.15) ;


    sonne.addSubPlanet(&merkur);
    sonne.addSubPlanet(&venus);
    sonne.addSubPlanet(&erde);
    erde.addSubPlanet(&erdemond);

    sonne.addSubPlanet(&mars);
    mars.addSubPlanet(&phobos);
    mars.addSubPlanet(&deimos);

    sonne.addSubPlanet(&jupiter);
    sonne.addSubPlanet(&saturn);
    sonne.addSubPlanet(&uranus);
    sonne.addSubPlanet(&neptun);


    // Triggern des Renderns
    sonne.render();

    // Stack wieder säubern.
    modelMatrixStack.pop();

/*
    modelMatrix.setToIdentity();
    modelMatrix.rotate(zRotation, 0, 1, 0);
    modelMatrixStack.push(modelMatrix) ;
    shaderProgram.setUniformValue(unifMatrixModel,modelMatrix);

    glDrawElements ( GL_TRIANGLES,
                     iboLength,
                     GL_UNSIGNED_INT,
                     0);
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
    vbo.allocate(vboData,sizeof(GLfloat) * vboLength);
    vbo.release();

    // Erzeuge Index-Buffer
    ibo.create();
    ibo.bind();
    ibo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    ibo.allocate(indexData,sizeof(GLuint) * iboLength);
    ibo.release();
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
    zoom += event->delta() / 30 ;
    emit zoomFactorChanged(zoom);
    glDraw() ;                      // !Deprecated
}


void MyGLWidget::keyPressEvent(QKeyEvent *event)
{

    /*
    switch ( event->key()) {
        case Qt::Key_W : moveY += 0.2 ;
             break ;
        case Qt:: Key_S : moveY -= 0.2 ;
             break ;
        case Qt::Key_A : moveX -= 0.2 ;
             break ;
        case Qt::Key_D : moveX += 0.2 ;
             break ;
        default : QGLWidget::keyPressEvent(event) ;
    }
    */
    //glDraw() ;                       // !Deprecated
}


void MyGLWidget::receiveRotationZ( int degrees )
{
    zRotation = degrees ;
   // glDraw();                       // !Deprecated
}


