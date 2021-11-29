#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D ambientMap1;
uniform sampler2D ambientMap2;


void main(){
  if(gl_FrontFacing)
    FragColor = texture2D(ambientMap1, TexCoords);
  else
    FragColor = texture2D(ambientMap2, TexCoords);
}
