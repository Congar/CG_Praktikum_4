#ifndef PLANET_H
#define PLANET_H

#include <vector>
#include <stack>
#include <QOpenGLShaderProgram>  // Shader

class Planet
{
    std::vector<Planet*> subplantes ;
    int    radius ;    // Abstand zum übergeordneten Planeten
    int    angle ;     // Aktueller Rotationswinkel
    double scale ;     // Skalierungsfaktor für den Planet
    unsigned int iboLength ;
    QOpenGLShaderProgram* shaderProgram ;
    int unifMatrixModel ;
    std::stack<QMatrix4x4>* modelStack ;

public:
    Planet();
    Planet( QOpenGLShaderProgram* _shaderProgram, int _unifMatrixModel, std::stack<QMatrix4x4>* modelStack, unsigned int _iboLength, int _radius, int _angle , double _scale ) ;

    void addSubPlanet( Planet* _newSubPlanet ) ;
    void render() ;
};

#endif // PLANET_H
