#version 330 core
out vec4 FragColor;


uniform int layer;

void main(){

	if(layer==0)
	{
		FragColor = vec4( 1.0, 0.0, 0.0, 1.0f);
	}
	if(layer==1)
	{
		FragColor = vec4( 0.0, 0.0, 1.0, 1.0f);
	}
	if(layer==2)
	{
		FragColor = vec4( 0.0, 1.0, 0.0, 1.0f);
	}
}
