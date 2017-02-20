#include "MyPrimitive.h"
MyPrimitive::MyPrimitive() { }
MyPrimitive::MyPrimitive(const MyPrimitive& other) { }
MyPrimitive& MyPrimitive::operator=(const MyPrimitive& other) { return *this; }
MyPrimitive::~MyPrimitive(void) { super::Release(); }
void MyPrimitive::CompileObject(vector3 a_v3Color)
{
	m_uVertexCount = static_cast<int> (m_lVertexPos.size());
	for (uint i = 0; i < m_uVertexCount; i++)
	{
		AddVertexColor(a_v3Color);
	}
	
	CompleteTriangleInfo(true);
	CompileOpenGL3X();

}
//C--D
//|\ |
//| \|
//A--B
//This will make the triang A->B->C and then the triang C->B->D
void MyPrimitive::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyPrimitive::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vCenter)
{
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vCenter);
}
void MyPrimitive::GeneratePlane(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;

	vector3 pointA(-fValue, -fValue, 0.0f); //0
	vector3 pointB(fValue, -fValue, 0.0f); //1
	vector3 pointC(fValue, fValue, 0.0f); //2
	vector3 pointD(-fValue, fValue, 0.0f); //3

	vector3 pointE(fValue, -fValue, -0.001f); //1
	vector3 pointF(-fValue, -fValue, -0.001f); //0
	vector3 pointG(fValue, fValue, -0.001f); //2
	vector3 pointH(-fValue, fValue, -0.001f); //3

											  //F
	AddQuad(pointA, pointB, pointD, pointC);
	//Double sided
	AddQuad(pointE, pointF, pointG, pointH);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

											  //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f * a_fHeight; //height 
	float gValue = 0.5f * a_fRadius; //radius

	//point array for n gon on the bottom of the cone
	vector3* points = new vector3[a_nSubdivisions];

	//Top point of the cone
	vector3 point0(0.0f, fValue, 0.0f);

	//Center of the n gon
	vector3 pointCenter(0.0f, -fValue, 0.0f);

	for (int i = 0; i < a_nSubdivisions; i++) {
		//Create points around the n gon by using sin and cosine functions
		//http://stackoverflow.com/questions/7198144/how-to-draw-a-n-sided-regular-polygon-in-cartesian-coordinates
		points[i] = vector3(gValue * cos((2 * PI * i) / a_nSubdivisions), -fValue, gValue * sin((2* PI * i ) /a_nSubdivisions ));
	}
	
	for (int i = 0; i < a_nSubdivisions - 1; i++) {
		//Create the triangle
		AddTri(points[i], points[i + 1], pointCenter);
		AddTri(points[i + 1], points[i], point0);
	}

	//The last triangle on the n gon always goes from the end to the beginning
	AddTri(points[a_nSubdivisions - 1], points[0], pointCenter);

	//Similarly for the last side traingle
	AddTri(points[0], points[a_nSubdivisions - 1], point0);
	
	//Don't forget to delete your new arrays
	delete[] points;

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f * a_fHeight; //height 
	float gValue = 0.5f * a_fRadius; //radius

	//point array for n gon on the bottom of the cylinder and top
	vector3* points = new vector3[a_nSubdivisions];
	vector3* points2 = new vector3[a_nSubdivisions];

	//Center of both n gons
	vector3 pointCenter(0.0f, -fValue, 0.0f);
	vector3 pointCenter2(0.0f, fValue, 0.0f);

	for (int i = 0; i < a_nSubdivisions; i++) {
		//Create points around the n gons by using sin and cosine functions
		//http://stackoverflow.com/questions/7198144/how-to-draw-a-n-sided-regular-polygon-in-cartesian-coordinates
		points[i] = vector3(gValue * cos((2 * PI * i) / a_nSubdivisions), -fValue, gValue * sin((2 * PI * i) / a_nSubdivisions));
		points2[i] = vector3(gValue * cos((2 * PI * i) / a_nSubdivisions), fValue, gValue * sin((2 * PI * i) / a_nSubdivisions));

	}

	for (int i = 0; i < a_nSubdivisions - 1; i++) {
		AddTri(points[i], points[i + 1], pointCenter);

		AddTri(points2[i + 1], points2[i], pointCenter2);
		//Add the quad on the side of the cylinder as you build the first n gon
		AddQuad(points[i + 1], points[i], points2[i + 1], points2[i]);
	}
	//The last triangle on the n gon always goes from the end to the beginning
	AddTri(points[a_nSubdivisions - 1], points[0], pointCenter);
	AddTri(points2[0], points2[a_nSubdivisions - 1], pointCenter2);

	//Build the last two quads to finish up the cylinder sides for the same reason as the n gon
	AddQuad(points[0],  points[a_nSubdivisions - 1], points2[0], points2[a_nSubdivisions - 1]);

	//Don't forget to delete your new arrays
	delete[] points;
	delete[] points2;


	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f * a_fOuterRadius;
	float gValue = 0.5f * a_fInnerRadius; 
	float hValue = 0.5f * a_fHeight;

	//point array for n gon
	vector3* points = new vector3[a_nSubdivisions]; //Top of tube outer
	vector3* points2 = new vector3[a_nSubdivisions]; // top of tube inner
	vector3* points3 = new vector3[a_nSubdivisions];// bottom of tube outer
	vector3* points4 = new vector3[a_nSubdivisions];// bottom of tuber inner

	for (int i = 0; i < a_nSubdivisions; i++) {
		//Create points around the n gons by using sin and cosine functions
		//http://stackoverflow.com/questions/7198144/how-to-draw-a-n-sided-regular-polygon-in-cartesian-coordinates
		points[i] = vector3(fValue * cos((2 * PI * i) / a_nSubdivisions), hValue, fValue * sin((2 * PI * i) / a_nSubdivisions));
		points2[i] = vector3(gValue * cos((2 * PI * i) / a_nSubdivisions), hValue, gValue * sin((2 * PI * i) / a_nSubdivisions));
		points3[i] = vector3(fValue * cos((2 * PI * i) / a_nSubdivisions), -hValue, fValue * sin((2 * PI * i) / a_nSubdivisions));
		points4[i] = vector3(gValue * cos((2 * PI * i) / a_nSubdivisions), -hValue, gValue * sin((2 * PI * i) / a_nSubdivisions));
	}

	for (int i = 0; i < a_nSubdivisions - 1; i++) {
		AddQuad(points[i + 1], points[i], points2[i+ 1], points2[i]); //Top of the tube
		AddQuad(points3[i], points3[i + 1], points4[i], points4[i + 1]); //Bottom of the tube
		AddQuad(points[i], points[i + 1], points3[i], points3[i + 1]); //Outside of the tube
		AddQuad(points2[i + 1], points2[i], points4[i + 1], points4[i]); //Inside of the tube
	}

	AddQuad(points[0], points[a_nSubdivisions - 1], points2[0], points2[a_nSubdivisions - 1]); // finish top of the tube
	AddQuad(points3[a_nSubdivisions - 1], points3[0], points4[a_nSubdivisions - 1], points4[0]); //finish bottom of the tube
	AddQuad(points[a_nSubdivisions - 1], points[0], points3[a_nSubdivisions - 1], points3[0]); //finish outside of the tube
	AddQuad(points2[0], points2[a_nSubdivisions - 1], points4[0], points4[a_nSubdivisions - 1]); //finish inside of the tube

	delete[] points;
	delete[] points2;
	delete[] points3;
	delete[] points4;

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius <= a_fInnerRadius + 0.1f)
		return;

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 25)
		a_nSubdivisionsA = 25;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 25)
		a_nSubdivisionsB = 25;

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f * a_fOuterRadius; //Outer Radius
	float gValue = 0.5f * a_fInnerRadius; // Inner Radius

	float r = fValue - gValue; //Outer - inner
	float R = gValue + r; // inner + the differencce

	for (int i = 0; i < a_nSubdivisionsA; i++) {
		float pL = 2 * PI * i / a_nSubdivisionsA; // Angle of Phi left
		float pR = 2 * PI * (i + 1) / a_nSubdivisionsA; // Angle of Phi Right
		for (int j = 0; j < a_nSubdivisionsB; j++) {
			float tL = 2 * PI * j / a_nSubdivisionsB; // Angle of Theta Left
			float tR = 2 * PI * (j + 1) / a_nSubdivisionsB; // Angle of Theta Right

			vector3 p1((R + r * cos(tL)) * cos(pL), r * sin(tL), (R + r * cos(tL)) * sin(pL)); //First point
			vector3 p2((R + r * cos(tL)) * cos(pR), r * sin(tL), (R + r * cos(tL)) * sin(pR)); //Second point
			vector3 p3((R + r * cos(tR)) * cos(pL), r * sin(tR), (R + r * cos(tR)) * sin(pL)); //Third Point
			vector3 p4((R + r * cos(tR)) * cos(pR), r * sin(tR), (R + r * cos(tR)) * sin(pR)); //Fourth Point

			AddQuad(p2, p1, p4, p3);
		}
	}


	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f * a_fRadius; //Radius

	for (int i = 0; i < a_nSubdivisions; i++) {
		float pL = PI * i / a_nSubdivisions;  // Angle of Phi left (only on rotation of zero - PI)
		float pR = PI * (i + 1) / a_nSubdivisions; // Angle of PHi Right
		for (int j = 0; j < a_nSubdivisions; j++) {
			float tL = 2 * PI * j / a_nSubdivisions;  // Angle of Theta left
			float tR = 2 * PI * (j + 1) / a_nSubdivisions; //  // Angle of Theta left
		
			vector3 p1(fValue * cos(tL) * sin(pL), fValue * cos(pL), fValue * sin(tL) * sin(pL));//First point
			vector3 p2(fValue * cos(tL) * sin(pR), fValue * cos(pR), fValue * sin(tL) * sin(pR));//Second point
			vector3 p3(fValue * cos(tR) * sin(pL), fValue * cos(pL), fValue * sin(tR) * sin(pL));//Third point
			vector3 p4(fValue * cos(tR) * sin(pR), fValue * cos(pR), fValue * sin(tR) * sin(pR));//Forth point

			AddQuad(p1, p2, p3, p4);
		}
 	}

	//Your code ends here
	CompileObject(a_v3Color);
}