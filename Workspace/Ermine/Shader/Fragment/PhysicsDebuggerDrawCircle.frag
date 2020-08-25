#version 330 core

uniform vec2 u_resolution;
uniform vec2 CircleCentre;
uniform float CircleRadius;

float circleshape(vec2 position, float radius){
  return step(radius, length(position - vec2(CircleCentre.x,CircleCentre.y)));
}

void main()
{
  vec2 position = gl_FragCoord.xy;  // CircleCentre;
  position.y = u_resolution.y - position.y;

  vec3 color = vec3(0.0);

  float circle = circleshape(position,CircleRadius);
  
  color = vec3(circle);
  color.x = 1.0;

  gl_FragColor = vec4(color, (1.0 - circle) * 0.4);
  //gl_FragColor = vec4(1.0,1.0,1.0,1.0);
}