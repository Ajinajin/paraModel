#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D ambientMap;
uniform float halfWindowWidth;

void main(){
  if(gl_FragCoord.x < halfWindowWidth)
    FragColor = texture2D(ambientMap, TexCoords);
  else
    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
