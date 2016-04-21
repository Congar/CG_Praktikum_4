#include "planet.h"



Planet::Planet()
{

}

Planet::Planet( QOpenGLShaderProgram* _shaderProgram, int _unifMatrixModel, std::stack<QMatrix4x4>* _modelStack, unsigned int _iboLength, int _radius, int _angle , double _scale )
{
    shaderProgram = _shaderProgram ;
    unifMatrixModel = _unifMatrixModel ;
    modelStack = _modelStack ;
    radius = _radius ;
    angle = _angle ;
    scale = _scale ;
    iboLength = _iboLength ;
}



void Planet::addSubPlanet( Planet* _newSubPlanet )
{
    subplantes.push_back(_newSubPlanet);
}

void Planet::render()
{
    QMatrix4x4 modelMatrix ;

    modelMatrix = modelStack->top() ;   // Worauf bezieht sich das aktuelle rendern

    // Transformationen für das aktuelle Objekt machen
    modelMatrix.rotate(angle, 0, 1, 0);
    modelMatrix.translate(radius, 0, 0);
    // Beim Skalieren muss ich aufpassen. Wenn ich hier mein Koordinatensystem "kleiner" mache,
    // bezieht sich dass auch auf die Subsysteme. Deshalb skaliere ich es in dem Punkt wo ich es
    // hinhaben will und mach die Skalierung danach wieder rückgängig.
    modelMatrix.scale(scale);

    shaderProgram->setUniformValue(unifMatrixModel,modelMatrix);

    glDrawElements ( GL_TRIANGLES,
                     iboLength,
                     GL_UNSIGNED_INT,
                     0);


    modelMatrix.scale(1/scale) ;

    // Das aktuelle Objekt ist gezeichnet. Jetzt betrachtet man noch die untergeordneten Systeme.
    // Damit die wissen worauf sie sich beziehen, legt man das aktuelle Model Koordinatensystem auf den Stack
    // welches die Subplanenten dann verwenden.
    modelStack->push(modelMatrix);

    for ( unsigned int i = 0 ; i < subplantes.size() ; i++ )
    {
        subplantes[i]->render();
    }

    // Der Planet ist gezeichnet und es gibt keine Subplaneten mehr.
    // Deshalb kann man die Model-Matrix wieder vom Stack runter nehmen.
    modelStack->pop();

    /*

    modelMatrix.setToIdentity();
    //modelMatrix.rotate(zRotation, 0, 1, 0);
    //modelMatrixStack.push(modelMatrix) ;
    shaderProgram->setUniformValue(unifMatrixModel,modelMatrix);

    glDrawElements ( GL_TRIANGLES,
                     iboLength,
                     GL_UNSIGNED_INT,
                     0);

*/
}
