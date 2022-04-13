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

uniform mat4 colors;
uniform vec4 rootsx;
uniform vec4 rootsy;


uniform vec4 coeffs;
uniform int IterationNum;

out vec4 Color;


vec2 multScalar (vec2 a, float c){return vec2(a.x*c, a.y*c);}
vec2 mult (vec2 a, vec2 b){return vec2(a.x*b.x-a.y*b.y, a.x*b.y+a.y*b.x);}
vec2 divide(vec2 a, vec2 b) {return vec2(((a.x*b.x+a.y*b.y)/(b.x*b.x+b.y*b.y)),((a.y*b.x-a.x*b.y)/(b.x*b.x+b.y*b.y)));}
vec2 add (vec2 a, vec2 b, vec2 c, vec2 d){return vec2(a.x + b.x + c.x + d.x, a.y + b.y + c.y + d.y);}
vec2 sub (vec2 a, vec2 b){return vec2(a.x-b.x, a.y-b.y);}
vec2 power(vec2 a, int c){
	vec2 b = vec2(1.0, 1.0);
	while (c > 0){
		b = mult(b,a);
		c--;
	}
	return b;
}
// get distance from rootsx(i),rootsy(i) to x,y
float dist(int i, float x, float y){
	float distx = x - rootsx[i];
	float disty = y - rootsy[i];
	return sqrt(distx*distx + disty*disty);
	}

// get index of closest root to x,y from roots 
int minDist(float x, float y){
	int minInd = 0;
	float minDist = 100;
	int i = 0;
	while (i != 3){
		float temp = dist(i,x,y);
		if (temp < minDist){
			minInd = i;
			minDist = temp;
		}
		i++;
	}
	return minInd;
}

// get color index 0 <= i <= 3
vec4 getColor(int i){
	return vec4(colors[0][i],colors[1][i],colors[2][i],colors[3][i]) ;
}

vec2 der(vec2 xy){
	vec2 second = power(xy,2);
	second = multScalar(second, 3*coeffs[0]);
	vec2 first = xy;
	first = multScalar(first, 2*coeffs[1]);
	vec2 zero = vec2(coeffs[2],0.0);
	return add(second, first, zero, vec2(0.0,0.0));
}
vec2 func(vec2 xy){
	vec2 third = power(xy, 3);
	third = multScalar(third, coeffs[0]);
	vec2 second = power(xy, 2);
	second = multScalar(second,coeffs[1]);
	vec2 first = xy;
	first = multScalar(first, coeffs[2]);
	vec2 zero = xy;
	zero = multScalar(zero, coeffs[3]);
	return add(third, second, first, zero);
}

vec2 delta(vec2 f, vec2 df){
	return divide(f,df);
}

bool funcNotZero(vec2 xy){
	float d = sqrt(0.001);
	return (func(xy).x > d || func(xy).y > d); 
}

// do function IterationNum times
// set color to closest root's color via index
void main()
{
	int i = 0;
	vec2 xy = vec2(position0.x,position0.y);
	while (i < IterationNum && funcNotZero(xy)){
		xy = add(sub(xy, delta(func(xy), der(xy))),vec2(0,0), vec2(0,0), vec2(0,0));
		i++;
	}
	Color = getColor(minDist(xy.x, xy.y));	
}
