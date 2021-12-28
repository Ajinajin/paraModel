#version 330 core
out vec4 FragColor;

uniform int if_sectionalMode;
uniform int layer;


in vec3 pos;

void main(){


	if(if_sectionalMode==0)
	{
		if(layer==0)
		{
			FragColor = vec4( 1.0, 0.0, 0.0, 1);
		}
		if(layer==1)
		{
			FragColor = vec4( 0.0, 0.0, 1.0, 1);
		}
		if(layer==2)
		{
			FragColor = vec4( 0.0, 1.0, 0.0, 1);
		}
	}
	else
	{
		if(pos[2]<=0)
		{
			if(layer==0)
			{
				FragColor = vec4( 1.0, 0.0, 0.0, 1);
			}
			if(layer==1)
			{
				FragColor = vec4( 0.0, 0.0, 1.0, 1);
			}
			if(layer==2)
			{
				FragColor = vec4( 0.0, 1.0, 0.0, 1);
			}
			
		}
		else
		{
			discard;
		}
		
	}

	
}
