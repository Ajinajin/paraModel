#version 330 core
out vec4 FragColor;

uniform int R;
uniform int G;
uniform int B;

void main(){
  
    FragColor = vec4(R/255.0, G/255.0, B/255.0, 0.25f);
}
