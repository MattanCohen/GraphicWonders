#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;

uniform vec4 lightColor;
uniform sampler2D sampler1;
uniform vec4 lightDirection;
uniform float time;
uniform float x;
uniform float y;

uniform vec4 coeffs;
uniform int IterationNum;

out vec4 Color;
float norm(){return sqrt(texCoord0.s * texCoord0.s +
						texCoord0.t * texCoord0.t);}
void main()
{
	float angle = time;
	vec3 lightPos = vec3(0,0,1);

	vec2 tmp = vec2(abs(cos(angle)*texCoord0.s - sin(angle)*texCoord0.t), abs(sin(angle)*texCoord0.s + cos(angle)*texCoord0.t));
	if(position0.s < 0.5 && position0.t < 0.5)
//	Color = vec4(0,0,0,0);
		Color = vec4(1.0,(position0.t+1)/2.0,norm()+0.2,0);
	else
		Color = vec4(1,norm(),1,0);
//	Color = vec4(1,1,1,1);
	//Color = texture(sampler1, tmp)* lightColor; //you must have gl_FragColor
}
