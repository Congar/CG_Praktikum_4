#version 130
// default130.frag: a simple fragment shader

varying vec4 texC;                  // diffuseMapCoord Texturkoordinaten
varying vec4 normalen;

uniform float t;
uniform sampler2D texture;          // diffuseMap
uniform sampler2D textureSphere;    // distortionMap
void main()
{

    vec4 diffuseMapCoord = texC.xyzw;

    // Koordinaten zum Bestimmen der auszulesenden Stelle in der Verzerrungstextur
   vec4 distortionMapCoord = diffuseMapCoord ;

    // Verschiebe die Verzerrungskoordinate abhängig von der Zeit nach oben
    // Die Konstsante beeinflusst die Geschwindigkeit
    distortionMapCoord.y = distortionMapCoord.y - (t * 0.2) ;

    // Lese Werte aus der Verzerrungstextur aus
    vec4 distortionValue =   texture2D(textureSphere,distortionMapCoord.xy);

    // Verwende die ersten zwei Komponenten als Verschiebung
    vec2 distortionOffset = distortionValue.xy ;

    // Von OpenGL normalisierte Werte leigen im Intervall [0,1]
    // Wir wollen eine Verschiebung in beide Richtungen, also im Intervall [-1,1]
    distortionOffset = distortionOffset - (0.5,0.5) ;
    distortionOffset = distortionOffset - 2 ;

    // Skaliere die Verschiebung und bestimme damit die Stärke der Verzerrung
    distortionOffset = distortionOffset * 0.05 ;

    // Mildere den effekt im oberen Bereich der Textur (optional)
    distortionOffset *= (1 - texC.y) ;

    // Berechne die verschobenen Texturkoordinaten
    vec2 distortedCoord = texC.xy + distortionOffset ;

    vec4 color1 = texture2D(texture,vec2(texC.x,texC.y));
    vec4 color2 = texture2D(textureSphere,vec2(distortedCoord.x,distortedCoord.y));



    vec4 outputColor = color1 * color2 ;
    gl_FragColor = outputColor ;

 //   gl_FragColor = vec4( distortionMapCoord.y , 1.0 , 0.0 , 1.0);
/*
    gl_FragColor = texture2D(texture,vec2(texC.x,texC.y));
    //gl_FragColor = texture2D(textureSphere,vec2(texC.x,texC.y));
   // gl_FragColor = vec4( t , 1.0 , 0.0 , 1.0);
    gl_FragColor = texture2D(texture,newTex);
    */

}

