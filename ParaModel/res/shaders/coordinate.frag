#version 330 core
out vec4 FragColor;

void main(){
  if(gl_FragCoord.y < 300)
    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
  else
    FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}
