#version 330 core

in float Radius;
in vec2 ScreenResolution;
in vec2 CircleCentre;

float circleshape(vec2 position, float radius){
  return step(radius, length(position - vec2(CircleCentre.x,CircleCentre.y)));
}

void main()
{
  vec2 position = gl_FragCoord.xy;  // CircleCentre;
  position.y = ScreenResolution.y - position.y;

  vec3 color = vec3(0.0);

  float circle = circleshape(position,Radius);
  
  color = vec3(circle);
  color.x = 1.0;

  gl_FragColor = vec4(color, (1.0 - circle) * 0.4);
  //gl_FragColor = vec4(1.0,1.0,1.0,1.0);
}