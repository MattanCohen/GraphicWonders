#include "Assignment1.h"
#include <iostream>


static void printMat(const Eigen::Matrix4d& mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout<< mat(j,i)<<" ";
		std::cout<<std::endl;
	}
}

Assignment1::Assignment1()
{
	time = 0;
	coeffs = Eigen::Vector4cf::Zero();
}

//Assignment1::Assignment1(float angle ,float relationWH, float near, float far) : Scene(angle,relationWH,near,far)
//{
//}

int IterationNum = 1;
Eigen::Vector3cf roots;

int colorInd = 0;
const int numOfColors = 6;

int pallet[numOfColors][3] = {
	{ 253,190,23 },
	{ 12,180,0 },
	{ 10,0,210 },
	{ 250,0,247 },
	{ 235,251,0 },
	{ 0,220,203 }
};
/*
int pallet[numOfColors][3] = {
	{ 255,255,255 },
	{ 255,255,255 },
	{ 255,255,255 },
	{ 255,255,255 },
	{ 255,255,255 },
	{ 255,255,255 },
};
*/

void Assignment1::Init()
{
	unsigned int texIDs[3] = { 0 , 1, 2};
	unsigned int slots[3] = { 0 , 1, 2 };


	AddShader("shaders/pickingShader");
	//int myShaderIndex = AddShader("../../shaders/testShader");
	//int myShaderIndex = AddShader("shaders/assignment1ShaderFirst");
	int myShaderIndex = AddShader("shaders/assignment1Shader");

    AddTexture("textures/box0.bmp",2);
    AddTexture("textures/grass.bmp", 2);

	AddMaterial(texIDs,slots, 1);
	AddMaterial(texIDs+1, slots+1, 1);

	AddShape(Plane, -1, TRIANGLES,0);
	SetShapeShader(0,1);
	SetShapeMaterial(0, 0);
	// ShapeTransformation(zTranslate,-5,0);
	// pickedShape = -1;
	SetShapeStatic(0);
	
	coeffs[0] = 1;
	coeffs[1] = 0.000001;
	coeffs[2] = 0.000001;
	coeffs[3] =  -1;
    
	for (int i = 0; i < 4; i++) {
		move[i] = 0;
	}
	zoom = 1;
	
	roots = FindCubicRoots();
	
	switchColor();

	/*
	std::cout<<"the roots are:\n"<< roots<<std::endl;

	std::cout<<"first "<< coeffs[0]*roots[0]*roots[0]*roots[0] + coeffs[1]*roots[0]*roots[0] + coeffs[2]* roots[0] + coeffs[3]<<std::endl;
	std::cout<<"second "<< coeffs[0]*roots[1]*roots[1]*roots[1] + coeffs[1]*roots[1]*roots[1] + coeffs[2]* roots[1] + coeffs[3]<<std::endl;
	std::cout<<"third "<< coeffs[0]*roots[2]*roots[2]*roots[2] + coeffs[1]*roots[2]*roots[2] + coeffs[2]* roots[2] + coeffs[3]<<std::endl;
	*/


	


//	ReadPixel(); //uncomment when you are reading from the z-buffer
	//SetShapeViewport(6, 1);
}
float delta = 0.1;
Eigen::Vector3cf clamp(Eigen::Vector3cf v, float low, float high) {
	for (int i = 0; i < 3; i++) {
		if (v[i].real() < low) {
			v[i] = (low, v[i].imag());
		}
		else if (v[i].real() > high) {
			v[i] = (high, v[i].imag());
		}

		if (v[i].imag() < low) {
			v[i] = (v[i].real(), low);
		}
		else if (v[i].imag() > high) {
			v[i] = (v[i].real(), high);
		}
	}
	return v;
}
void Assignment1::Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((shapeIndx+1) & 0x000000FF) >>  0;
	int g = ((shapeIndx+1) & 0x0000FF00) >>  8;
	int b = ((shapeIndx+1) & 0x00FF0000) >> 16;

	 

	s->Bind();
	s->SetUniform1f("time",time);
	s->SetUniform1f("x", x);
	s->SetUniform1f("y",y);
	s->SetUniformMat4f("Proj", Proj);
	s->SetUniformMat4f("View", View);
	s->SetUniformMat4f("Model", Model);
	if (data_list[shapeIndx]->GetMaterial() >= 0 && !materials.empty())
	{
//		materials[shapes[pickedShape]->GetMaterial()]->Bind(textures);
		BindMaterial(s, data_list[shapeIndx]->GetMaterial());
	}
	if (shaderIndx == 0)
		s->SetUniform4f("lightColor", r / 255.0f, g / 255.0f, b / 255.0f, 0.0f);
	else
		s->SetUniform4f("lightColor", time/10.0f, 60 / 100.0f, 99 / 100.0f, 0.5f);
	//textures[0]->Bind(0);
	
	//roots = clamp(FindCubicRoots(), -1.0, 1.0);
	roots = (FindCubicRoots());

	s->SetUniform1f("zoom", zoom);
	s->SetUniform4f("move", move[0], move[1], 0, 0);
	
	s->SetUniform4f("rootsx", roots[0].real(), roots[1].real(), roots[2].real(), 0);
	s->SetUniform4f("rootsy", roots[0].imag(), roots[1].imag(), roots[2].imag(), 0);
	s->SetUniform4f("coeffs", coeffs[0].real(), coeffs[1].real(), coeffs[2].real(), coeffs[3].real());
	s->SetUniform1i("IterationNum", IterationNum);

	//std::cout << "coeffs: " << coeffs[0] << ", " << coeffs[1] << ", " << coeffs[2] << ", " << coeffs[3] << std::endl;
	//std::cout << "roots: " << roots[0] << ", " << roots[1] << ", " << roots[2]  << std::endl;
	//std::cout << "Iteration num " << IterationNum << std::endl;
	//s->SetUniform1i("sampler2", materials[shapes[pickedShape]->GetMaterial()]->GetSlot(1));
	//s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 0.0f);
//	if(shaderIndx == 0)
//		s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
//	else
//		s->SetUniform4f("lightColor",0.7f,0.8f,0.1f,1.0f);
	s->SetUniformMat4f("colors", colors);
	
	s->Unbind();
}

void Assignment1::changeIterationNum(bool append) {
	if (append)
		IterationNum++;
	else if (IterationNum > 1)
		IterationNum--;
}

void Assignment1::changeCoeffs(int i, bool append) {
	if (append)
		coeffs[i] = coeffs[i].real() + 0.01;
	else
		coeffs[i] = coeffs[i].real() - 0.01;
}

void Assignment1::resetCoeffs() {
	coeffs[0] = 1;
	coeffs[1] = 0.000001;
	coeffs[2] = 0.000001;
	coeffs[3] = -1;
	zoom = 1;
	move[0] = 0;
	move[1] = 0;
	colorInd = 0;
	switchColor();
	IterationNum = 1;
}

void Assignment1::switchColor() {
	int s = sizeof(pallet) - 2;
	int s1 = (colorInd + 1) % s;
	int s2 = (colorInd + 2) % s;
	colors << pallet[colorInd][0] / 280.0, pallet[colorInd][1] / 280.0, pallet[colorInd][2] / 280.0, 0.00,

		pallet[s1][0] / 280.0, pallet[s1][1] / 280.0, pallet[s1][2] / 280.0, 00,

		pallet[s2][0] / 280.0, pallet[s2][1] / 280.0, pallet[s2][2] / 280.0, 00,

		1, 1, 1, 00;
	colorInd = (colorInd + 1) % 6;
}
void Assignment1::WhenRotate(bool append)
{
	if (append)
		zoom += 0.01;
	else
		if (zoom - 0.01 > 0.005)
			zoom -= 0.01;
		else
			if (zoom - 0.05 > 0.001)
				zoom -= 0.05;
	std::cout << "Each pixel original width: " << 1.0 / 800.0 << " decimal or 1/800." << std::endl;
	float updatedWidth = (1.0 / 800.0) * (1.0 / zoom);
	std::cout << "Each pixel updated width: " << updatedWidth << std::endl;
}

void Assignment1::WhenTranslate(bool xAxis, bool append)
{
	int i = 1;
	if (xAxis)
		i = 0;
	if (append)
		move[i] += 0.01;
	else
		move[i] -= 0.01;
}

void Assignment1::Animate() {
    //if(isActive)
		time += 0.01f;
}

void Assignment1::ScaleAllShapes(float amt,int viewportIndx)
{
	for (int i = 1; i < data_list.size(); i++)
	{
		if (data_list[i]->Is2Render(viewportIndx))
		{
            data_list[i]->MyScale(Eigen::Vector3d(amt, amt, amt));
		}
	}
}
Eigen::Vector3cf Assignment1::FindCubicRoots()
{
	Eigen::Vector2cf reduceCoeffs = Eigen::Vector2cf::Zero();
	Eigen::Vector3cf roots;
	std::complex<float> bOver3a = (coeffs[1]/coeffs[0])/3.0f;
	reduceCoeffs[0] = coeffs[2]/coeffs[0] - 3.0f*bOver3a*bOver3a;
	reduceCoeffs[1] = coeffs[2]/coeffs[0]*bOver3a - coeffs[3]/coeffs[0] - 2.0f*bOver3a*bOver3a*bOver3a;
	if(reduceCoeffs.norm()>0.000001)
	{
		roots =	FindRootsOfReduceEquation(reduceCoeffs);
		roots[0] -= bOver3a;
		roots[1] -= bOver3a;
		roots[2] -= bOver3a;
	}
	else
	{
		roots[0] = -1.0f*bOver3a;
		roots[1] = std::complex<float>(cosf(3.14159f/3.0f),sinf(3.14159f/3.0f))*bOver3a;
		roots[2] = std::complex<float>(cosf(2.0f*3.14159f/3.0f),sinf(2*3.14159f/3.0f))*bOver3a;
	}

	return roots;
}

std::complex<float> Assignment1::NewtonCubicRoot(std::complex<float> num)
{
	std::complex<float> root = num ;
	const int iter = 9;
	bool isSmall = false;
	if(std::abs(num)<1e-3)
	{
		if(std::abs(num)==0)
			return num;
		isSmall = true;
		num = num*1e6f;
		root = num;
	}
	else
		if(std::abs(num)<0.9f)
		 	root = 1;
	for (int k = 0; k < iter; k++)
	{
		root = (2.0f*root*root*root + num)/root/root/3.0f;
	}
	if(isSmall)
		root = root/100.0f;
	return root;
}

Eigen::Vector3cf Assignment1::FindRootsOfReduceEquation(Eigen::Vector2cf reduceCoeffs)
{
	Eigen::Vector3cf roots = Eigen::Vector3cf::Zero();
	std::complex<float> sqroot = std::sqrt(reduceCoeffs[0]*reduceCoeffs[0]*reduceCoeffs[0]/27.0f + reduceCoeffs[1]*reduceCoeffs[1]/4.0f);
	std::complex<float> p = NewtonCubicRoot(reduceCoeffs[1]/2.0f + sqroot);
	std::complex<float> n = NewtonCubicRoot(reduceCoeffs[1]/2.0f - sqroot);
	roots[0] = p + n;
	roots[1] = p *std::complex<float>(cosf(2.0f*3.14159f/3.0f),sinf(2*3.14159f/3.0f)) - n * std::complex<float>(cosf(1.0f*3.14159f/3.0f),sinf(1*3.14159f/3.0f));
	roots[2] = -p * std::complex<float>(cosf(1.0f*3.14159f/3.0f),sinf(1*3.14159f/3.0f)) + n * std::complex<float>(cosf(2.0f*3.14159f/3.0f),sinf(2*3.14159f/3.0f));
	return roots;
}
Assignment1::~Assignment1(void)
{
}
