#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <string>
#include <limits>
#include<math.h>
using namespace std;

//default command line paramaters
const double default_theta1 = -51.0;
const double default_theta2 = 39.0;
const double default_theta3 = 65.0;
const double default_l1 = 4.0;
const double default_l2 = 3.0;
const double default_l3 = 2.5;
const double default_l0 = 1.0;
vector <vector <double>> M_prev;
const double eps = std::numeric_limits<double>::epsilon();

const vector <vector <double>> u0 = { {2, 2, 1, 1}, {-2, 2, 1, 1}, {-2, -2, 1, 1}, {2, -2, 1, 1}, {2, 2, 0, 1}, {-2, 2, 0, 1}, {-2, -2, 0, 1}, {2, -2, 0, 1} };
const vector <vector <double>> u_L = { { 0.5, 0.5, 0, 1},{-0.5, 0.5, 0, 1},{ -0.5, -0.5, 0, 1},{ 0.5, -0.5, 0, 1},{ 0.5, 0.5, 0, 1},{ -0.5, 0.5, 0, 1},{ -0.5, -0.5, 0, 1},{ 0.5, -0.5, 0, 1} };

const vector <double> rconst0 = { 1.0, 1.0, 1.0 };

//defining value of PI explicitly
const double pi = 3.14159265358979323846;

//Class Paramaters 
class Parameters
{
	double theta1, theta2, theta3, l1, l2, l3;

public:
	//define Functions of Class
	void setValues(double, double, double, double, double, double);
	double getTheta1();
	double getTheta2();
	double getTheta3();
	double getL1();
	double getL2();
	double getL3();

};

//Function to set values from command line
void Parameters::setValues(double theta_1, double theta_2, double theta_3, double l_1, double l_2, double l_3)
{
	theta1 = theta_1;
	theta2 = theta_2;
	theta3 = theta_3;
	l1 = l_1;
	l2 = l_2;
	l3 = l_3;

}

//Functions to retrieve Parameters 

double Parameters::getTheta1()
{
	return theta1;
}

double Parameters::getTheta2()
{
	return theta2;
}

double Parameters::getTheta3()
{
	return theta3;
}

double Parameters::getL1()
{
	return l1;
}

double Parameters::getL2()
{

	return l2;
}

double Parameters::getL3()
{
	return l3;
}

//Check if command line option is passed 
int found(char* s[], string sub, int len) {
	int pos = 0;
	bool flag = false;

	while (pos < (len - 1)) {

		if (s[pos] == sub) {
			flag = true;
			return pos;

		}

		pos++;
	}

	if (flag)
		return pos;
	else
		return -1;
}


//Extract an Argument from a string else return null string
string extractArgument(char* s[], string sub, int len) {

	int a = found(s, sub, len);
	string z = "";

	//If option is passed, then extract the next argument as the parameter
	if (a != -1) {
		a = a + 1;
		z = s[a];
		return z;
	}
	else
		return z;
}

//Assign Values and Return True if values are assigned successfully
void argumentAssign(char* args[], int len, Parameters &param)
{

	double theta1, theta2, theta3, l1, l2, l3;

	string found1 = extractArgument(args, "-t", len);
	string found2 = extractArgument(args, "-u", len);
	string found3 = extractArgument(args, "-v", len);
	string found4 = extractArgument(args, "-l", len);
	string found5 = extractArgument(args, "-m", len);
	string found6 = extractArgument(args, "-n", len);

	//find theta1 in args 
	if (found1 != "") {
		theta1 = atof(found1.c_str());
	}
	//else compute default_theta1
	else {
		theta1 = default_theta1;
	}

	//find theta2 in args 
	if (found2 != "") {
		theta2 = atof(found2.c_str());
	}
	//else compute default_theta2
	else {
		theta2 = default_theta2;
	}

	//find theta3 in args
	if (found3 != "") {
		theta3 = atof(found3.c_str());
	}
	//else assign default_r
	else {
		theta3 = default_theta3;
	}

	//find L1 in args
	if (found4 != "") {
		l1 = atof(found4.c_str());
	}
	//else assign default_l1
	else {
		l1 = default_l1;
	}

	//find L2 in args
	if (found5 != "") {
		l2 = atof(found5.c_str());
	}
	//else assign default_l2
	else {
		l2 = default_l2;
	}

	//find L3 in args
	if (found6 != "") {
		l3 = atof(found6.c_str());
	}
	//else assign default_l3
	else {
		l3 = default_l3;
	}

	//set values for paramters
	param.setValues(theta1, theta2, theta3, l1, l2, l3);

}


//precision of 6
double precision6(double value) {

	double result = round(value * (double)1000000) / (double)1000000;
	return result;
}

//Translate along z
vector<vector <double>> tz(double L) {
	vector<vector <double>> T = { {1,0,0,0},  {0,1,0,0} , {0, 0, 1, L}, {0, 0, 0, 1} };
	return T;

}

//Rotate along z
vector<vector <double>> rz(double theta) {
	vector<vector <double>> R = { { cos(theta * pi/180),-sin(theta * pi/180), 0, 0 }, {sin(theta * pi/180), cos(theta * pi/180) , 0, 0 } , { 0, 0, 1, 0},{ 0, 0, 0, 1 } };
	return R;
}

//Rotate along y
vector<vector <double>> ry(double theta) {
	vector<vector <double>> R = { { cos(theta * pi / 180), 0 , sin(theta * pi / 180), 0 },{ 0 ,1, 0, 0 } ,{ -sin(theta * pi/180), 0, cos(theta * pi/180), 0 },{ 0, 0, 0, 1 } };
	return R;
}

//Multiply 2 vector of vectors
vector <vector <double>> mult(vector <vector <double>> A, vector <vector <double>> B) {
	vector <vector <double>> result;

	//initialize rows
	for (int i = 0; i < A.size(); i++) {
		vector <double> row;
		result.push_back(row);
	}

	double c = 0;


	//matrix multiplication
	for (int i = 0; i < A.size(); i++) {
		for (int j = 0; j < B[0].size(); j++) {
			c = 0;
			for (int k = 0; k < B.size(); k++) {
				c += A[i][k] * B[k][j];
			}
			result[i].push_back(c);
		}
		
	}

	//return result
	return result;

}

//extract function to fill in L in point
vector <vector <double>> extract(vector <vector <double>> u, double l) {
	u[0][2] = l;
	u[1][2] = l;
	u[2][2] = l;
	u[3][2] = l;

	return u;
}


//print matrix
void printMatrix(vector < vector <double>> p) {

	for (int i = 0; i < p.size(); i++) {
		for (int j = 0; j < p[0].size(); j++)
			cout << p[i][j] << "\t";
		cout << "\n";
	}


}

//transpose function
vector<vector<double>> transpose(vector <vector<double>> orig) {
	vector <vector <double>> t;

	//initialize matrix
	for (int j = 0; j < orig[0].size(); j++) {
		vector <double> row;
		t.push_back(row);
		for (int i = 0; i < orig.size(); i++) {
			t[j].push_back(0);
		}
	}

	//perform transpose
	for (int j = 0; j < orig[0].size(); j++) {
		for (int i = 0; i < orig.size(); i++) {
			t[j][i] = orig[i][j];

		}
	}

	return t;

}

//Function to extract last translation coordinates
vector <double> extractTranslate(double l) {

	vector <vector <double>> M = mult(M_prev, tz(l));
	vector <double> result = {M[0][M.size()-1], M[1][M.size()-1], M[2][ M.size()-1]};
	
	return result;
}

//Generation of Hierarchichal cuboid
vector <vector <double>> cuboid(vector <vector <double>> u, int i, double l_prev, double l, double theta) {
	
	//declare matriices
	vector < vector <double> > M;
	vector < vector <double> > T;
	vector < vector <double> > R;
	vector <vector <double>> ul, ul_t;
	vector <vector <double>> result;

	//extract u as per l
	ul = extract(u, l);

	//computation for T
	T = tz(l_prev);

	//computation for R
	if (i == 0)
		R = rz(theta);
	else
		R = ry(theta);

	//computation for M
	M = mult(T, R);

	//transpose ul
	ul_t = transpose(ul);
	
	//return result
	if(i == 0)
		result =	transpose(mult(M, ul_t));
	else
		result =  transpose(mult(mult(M_prev, M), ul_t));

	//compute M_prev;
	if (i == 0)
		M_prev = M;
	else
		M_prev = mult(M_prev, M);

	//return result
	return result;

}


//Function to print out initial comments
void initial() {
	cout << "#Inventor V2.0 ascii \n";

}

//Function to print out Separator
void separator() {
	cout << "Separator {" << "\n";

}

//Function to print out coordinate3
void coordinate3(vector<vector <double>> u) {
	cout << "Coordinate3 {" << "\n";
	cout << "point[" << "\n";
	for (int i = 0; i < u.size(); i++) {
		for (int j = 0; j < u[0].size() - 1; j++)
			cout << u[i][j] << " ";
		cout << "," << "\n";
	}
	cout << "] \n";
	cout << "} \n";

}


//Function to print out indexed line set for cuboid
void indexedLineSet() {
	cout << "IndexedLineSet {" << "\n";
	cout << "coordIndex[" << "\n";
	cout << "0, 1, 2, 0, -1, \n";
	cout << "0, 2, 3, 0, -1, \n";
	cout << "7, 6, 5, 7, -1, \n";
	cout << "7, 5, 4, 7, -1, \n";
	cout << "0, 3, 7, 0, -1, \n";
	cout << "0, 7, 4, 0, -1, \n";
	cout << "1, 5, 6, 1, -1, \n";
	cout << "1, 6, 2, 1, -1, \n";
	cout << "0, 4, 5, 0, -1, \n";
	cout << "0, 5, 1, 0, -1,\n";
	cout << "3, 2, 6, 3, -1,\n";
	cout << "3, 6, 7, 3, -1 \n";
	cout << "] \n";
	cout << "} \n";

}


//end Separator Function
void endSeparator() {

	cout << "} \n";
}

//light model function
void lightModel(string s) {

	cout << "LightModel { \n";
	cout << "model " << s << "\n";
	cout << "} \n";
}

//function to print sphere
void material(vector <double> r) {

	cout << "Material { \n";
	cout << "diffuseColor " << r[0] << " " << r[1] << " " << r[2] << "\n";
	cout << "} \n";
}

//function to print radius
void sphere(double r) {
	cout << "Sphere { \n";
	cout << "radius" << " " << r << "\n";
	cout << "} \n";
}

//function to print transform - translation here
void transform(vector <double> r) {

	cout << "Transform { \n";
	cout << "translation " << r[0] << " " << r[1] << " " << r[2] << "\n";
	cout << "} \n";
}

//***************MAIN FUNCTION**************

int main(int argc, char* argv[])
{
	Parameters param;

	//Function call to assign Command Line Arguments : if arguments are more than 1 then read them and fill in default values, else directly assign default values
	if (argc > 1)
		argumentAssign(argv, argc, param);
	else
		param.setValues(default_theta1, default_theta2, default_theta3, default_l1, default_l2, default_l3);

	//declare generic vector coordinates
	vector <vector <double>> u;

	//Print out in .iv format - CUBOID 0
	initial();
	cout << "\n";
	separator();

	//print coordinates
	coordinate3(u0);

	//indexed line set for cuboid
	indexedLineSet();

	endSeparator();

	//CUBOID 1
	cout << "\n";
	separator();

	//print coordinates
	u = cuboid(u_L, 0, default_l0, param.getL1(), param.getTheta1());
	coordinate3(u);

	//indexed line set for cuboid
	indexedLineSet();

	endSeparator();

	//CUBOID 2
	cout << "\n";
	separator();

	//print coordinates
	u = cuboid(u_L, 1, param.getL1(), param.getL2(), param.getTheta2());
	coordinate3(u);

	//indexed line set for cuboid
	indexedLineSet();

	endSeparator();

	//CUBOID 3
	cout << "\n";
	separator();

	//print coordinates
	u = cuboid(u_L, 2, param.getL2(), param.getL3(), param.getTheta3());
	coordinate3(u);

	//indexed line set for cuboid
	indexedLineSet();

	endSeparator();

	//****Display Spheres***

	//Sphere - 1
	separator();
	lightModel("PHONG");
	material(rconst0);
	sphere(0.2);
	endSeparator();

	//Sphere - 2
	separator();
	lightModel("PHONG");
	material(rconst0);
	vector <double> r3 = extractTranslate(param.getL3());
	transform(r3);
	sphere(0.2);
	endSeparator();


	return 0;

}
