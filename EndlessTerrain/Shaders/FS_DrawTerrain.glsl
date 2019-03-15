#version 430 core

in VS_OUT 
{
    float height;
} fs_in;

out vec4 color;

void main()
{
	if(length(fs_in.height) < 0.06f)
	{
		color = vec4(0,0,1,1);
	}
	else
	{
		color = vec4(0,1,0,1);
	}
	color = vec4(fs_in.height*5,0,0,1);
}