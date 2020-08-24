#version 330 core

uniform vec2 u_resolution;
uniform float CircleRadius;

float circleshape(vec2 position, float radius){
  return step(radius, length(position - vec2(u_resolution.x/2.0,u_resolution.y/2.0)));
}

void main()
{
  vec2 position = gl_FragCoord.xy; // / u_resolution;

  vec3 color = vec3(0.0);

  float circle = circleshape(position,CircleRadius);
  
  color = vec3(circle);
  color.x = 1.0;

  gl_FragColor = vec4(color, (1.0 - circle) * 0.4);
}