#version 330 core

in float Radius;
in vec2 ScreenResolution;
in vec2 CircleCentre;
in vec4 Color;

float circleshape(vec2 position, float radius){
  return step(radius, length(position - vec2(CircleCentre.x,CircleCentre.y)));
}

void main()
{
  vec2 position = gl_FragCoord.xy;  // CircleCentre;
  position.y = ScreenResolution.y - position.y;

  float circle = circleshape(position,Radius);
  
  gl_FragColor = vec4(Color.xyz, (1.0 - circle) * Color.w);
  //gl_FragColor = vec4(1.0,1.0,1.0,1.0);
}