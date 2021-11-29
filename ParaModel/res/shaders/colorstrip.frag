#version 330 core

in vec3 Pos;

out vec4 FragColor;

uniform float height;

void main(){
	if(gl_FragCoord.y>(height * 17 / 20))
	{
		FragColor = vec4(1.0f,0.0f,0.0f,1.0f);
	}
	else if(gl_FragCoord.y<=(height * 17 / 20) && gl_FragCoord.y>=(height * 14 / 20))
	{
		FragColor = vec4(0.0f,0.0f,1.0f,1.0f);
	}
	else if(gl_FragCoord.y<(height * 14 / 20) && gl_FragCoord.y>=(height * 11 / 20))
	{
		FragColor = vec4(1.0f,1.0f,0.0f,1.0f);
	}
	else
	{
		FragColor = vec4(0.0f,1.0f,0.0f,1.0f);
	}
	//if(gl_FragCoord.y>600)
	//{
		//FragColor = vec4(1.0f,0.0f,0.0f,1.0f);
	//}
	
	//FragColor = vec4(1.0f,1.0f,0.0f,1.0f);
}
