/**************************************************************************
**
**    Copyright (C) 2013 by Philip Schuchardt
**    www.cavewhere.com
**
**************************************************************************/

// simple fragment shader
//#version 330

#ifdef GL_ES
precision highp float;
#endif

//varying vec3 gTriangleDistance;
//varying vec3 vPosition;
varying vec2 vTexCoord;
//varying float elevation;

//out vec4 fragColor;

//uniform float vAngle;
//uniform vec4 colorBG;
uniform sampler2D Texture;

//float amplify(float d, float scale, float offset) {
//  d = scale * d + offset;
//  d = clamp(d, 0.0, 1.0);
// // d = 1.0 - exp2(-2.0 * d * d);
//  return d;
//}

void main() {
//  float triangleDistance = min(gTriangleDistance.x, min(gTriangleDistance.y, gTriangleDistance.z));

  //vec4 sauce = vec4(vTexCoord, 0.0, 1.0); //(0.5, 0.5, 1.0, 1.0);
  vec4 textureSample = texture2D(Texture, vTexCoord);

//  float normElevation = (elevation - 100.0) / (300.0 - 100.0);
//  vec3 elevationColor = normElevation * vec3(1.0, 0.0, 0.0) + vec3(0.5, 0.5, 0.5);


//  float amplifyValue = amplify(triangleDistance, 85.0, 0.0);
  gl_FragColor = textureSample;

//  gl_FragColor = (1.0 - amplifyValue) * vec4(0.0, 0.2, .0, 1.0) + amplifyValue * textureSample;
}
