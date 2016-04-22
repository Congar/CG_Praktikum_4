#ifndef PLANET_H
#define PLANET_H

#include <vector>
#include <stack>
#include <QOpenGLShaderProgram>  // Shader

class Planet
{
    std::vector<Planet*> subplantes ;
    int    radius ;    // Abstand zum übergeordneten Planeten
    int    angleCenter ;     // Aktueller Rotationswinkel bezogen auf den übergeordneten Planeten (Umlauf)
    int    selfRotation ;   // Eigendrehung des Planeten
    double scale ;     // Skalierungsfaktor für den Planet
    unsigned int iboLength ;
    QOpenGLShaderProgram* shaderProgram ;
    int unifMatrixModel ;
    std::stack<QMatrix4x4>* modelStack ;

public:
    Planet();
    Planet( QOpenGLShaderProgram* _shaderProgram, int _unifMatrixModel, std::stack<QMatrix4x4>* _modelStack, unsigned int _iboLength, int _radius, int _angleCenter , int _selfRotation , double _scale ) ;

    void addSubPlanet( Planet* _newSubPlanet ) ;
    void render() ;
};

#endif // PLANET_H
