
const char* GetVertexShaderCode(bool multiview) 
{
	if(multiview) {
	return R""""(#version 330
#extension GL_OVR_multiview2 : enable
layout(num_views = 2) in;
uniform mat4 MVP[2];
in vec3 vCol;
in vec3 vPos;
out vec3 color;
void main()
{
	gl_Position = MVP[gl_ViewID_OVR] * vec4(vPos, 1.0);
	color = vCol;
})"""";
	}
	else {
		return R""""(#version 330
uniform mat4 MVP;
in vec3 vCol;
in vec3 vPos;
out vec3 color;
void main()
{
	gl_Position = MVP * vec4(vPos, 1.0);
	color = vCol;
})"""";
	}
}

const char* GetFragmentShaderCode()
{
	return R""""(#version 330
in vec3 color;
out vec4 fragment;
void main()
{
    fragment = vec4(color, 1.0);
})"""";
}


