#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <string>
#include <limits>
#include<math.h>
using namespace std;

//default command line paramaters
const double default_du_num = 19.0;
const double default_dv_num = 9.0;
const double default_r = 1.0;
const double default_t = 1.0;
const double default_A = 1.0;
const double default_B = 1.0;
const double default_C = 1.0;
const int default_op = 0;

const double eps = std::numeric_limits<double>::epsilon();

//defining value of PI explicitly
const double pi = 3.14159265358979323846;
const double epspi = 0.00159265358979323846;

//Class Paramaters 
class Parameters
{
	double du, dv, A, B, C, r, t;
	int op;

public:
	//define Functions of Class
	void setValues(double, double, double, double, double, double, double, int);
	double getDu();
	double getDv();
	double getr();
	double gett();
	double getA();
	double getB();
	double getC();
	int getOp();

};

//Function to set values from command line
void Parameters::setValues(double u, double v, double rc, double tc, double Ac, double Bc, double Cc,  int option)
{
	du = u;
	dv = v;
	op = option;
	r = rc;
	t = tc;
	A = Ac;
	B = Bc;
	C = Cc;

}

//Functions to retrieve Parameters 

int Parameters::getOp()
{
	return op;
}

double Parameters::getDu()
{
	return du;
}

double Parameters::getDv()
{

	return dv;
}

double Parameters::getr()
{

	return r;
}

double Parameters::gett()
{

	return t;
}

double Parameters::getA()
{

	return A;
}

double Parameters::getB()
{

	return B;
}

double Parameters::getC()
{

	return C;
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

// Check if command line Polygon Option is passed
int findPoly(char* s[], int len) {
	int pos = 0, option;
	bool flag = false;
	string st;

	while (pos < len) {

		st = s[pos];

		if (st.compare("-F") == 0) {
			flag = true;
			option = 0;
			return 0;
		}

		if (st.compare("-S") == 0) {
			flag = true;
			option = 1;
			return 1;
		}

		pos++;
	}

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

	double u, v, r, s, A, B, C, t;
	int op;

	int model = findPoly(args, len);
	string found2 = extractArgument(args, "-u", len);
	string found3 = extractArgument(args, "-r", len);
	string found4 = extractArgument(args, "-v", len);
	string found5 = extractArgument(args, "-t", len);
	string found6 = extractArgument(args, "-A", len);
	string found7 = extractArgument(args, "-B", len);
	string found8 = extractArgument(args, "-C", len);

	//find u in args 
	if (found2 != "") {
		u = (2*pi) / (atof(found2.c_str()) - 1);
	}
	else
		//else compute default du
		u = (2*pi) / (default_du_num - 1);

	//find v in args 
	if (found4 != "") {
		
		v = pi/(atof(found4.c_str()) - 1);
	}
	else
		//else compute default dv
		v = pi / (default_dv_num - 1);

	//find r in args
	if (found3 != "") {
		r = atof(found3.c_str());
	}
	//else assign default_r
	else {
		r = default_r;
	}

	//find t in args
	if (found5 != "") {
		t = atof(found5.c_str());
	}
	//else assign default_t
	else {
		t = default_t;
	}

	//find A in args
	if (found6 != "") {
		A = atof(found6.c_str());
	}
	//else assign default_A
	else {
		A = default_A;
	}

	//find B in args
	if (found7 != "") {
		B = atof(found7.c_str());
	}
	//else assign default_B
	else {
		B = default_B;
	}

	//find C in args
	if (found8 != "") {
		C = atof(found8.c_str());
	}
	//else assign default_C
	else {
		C = default_C;
	}


	//find Poylgon Approximation Type in Argument
	if (model > -1) {
		op = model;
	}
	//else assign default radius
	else {
		op = default_op;
	}

	param.setValues(u, v, r, t, A, B, C, op);

}

//sign function
double sgn(double x) {
	if (x < 0.0)
		return -1;
	else if (x == 0.0)
		return 0;
	else if(x > 0.0)
		return 1;
}

//cos function
double c_func(double w, double m) {

	double result;

	result = sgn(cos(w))*pow(fabs(cos(w)), m);

	return result;
}

//sine function
double s_func(double w, double m) {

	double result;

	result = sgn(sin(w))*pow(fabs(sin(w)), m);

	return result;
}
 

//Function to Compute Product for Various Coordinates
double computeProd(int p, double u, double v, double r, double t, double A, double B, double C) {
	double prod = 0;

	switch (p) {
	case 0:
		prod = A * c_func(v, r) * c_func(u, t);
		break;
	case 1:
		prod = B * c_func(v, r) * s_func(u, t);
		break;
	case 2:
		prod = C * s_func(v, r);
		break;
	}

	return prod;

}

double precision6(double value) {

	double result = round(value * (double)1000000) / (double)1000000;
	return result;
}

//General Superellispoid Function - Generation of Points for (u,v)
vector <double> superellipsoid(int p, double du, double dv, double r, double t, double A, double B, double C) {

	double u = (-1 * pi);
	double v = (-1 * pi/2);
	vector <double> result;
	double prod = 0;
	double start = (-1 * pi/2);
	double end = (pi/2);
	double endu = (pi);

	//While u is less than pi
	while (u < (endu - epspi)) {

		v = start + dv;

		while (v < (end - epspi)) {

			//initialize Prod
			prod = 0;

			//Compute the Prod
			prod = computeProd(p, u, v, r, t, A, B, C);

			//increment v
			v = v + dv;
			prod = precision6(prod);
			result.push_back(prod);
		}

		//increment u
		u = u + du;
	}

	//If u exceeds pi or is equal to pi, set u as pi

	if (u >= (endu - epspi)) {
		u = endu;
		prod = 0;
		v = start + dv;

		while (v < (end - epspi)) {

			//initialize prod
			prod = 0;

			//Compute the Product
			prod = computeProd(p, u, v, r, t, A, B, C);

			//increment v
			v = v + dv;
			prod = precision6(prod);
			result.push_back(prod);
		}


	}

	//Interpolate v Endpoints Separately for Poles
	u = 0;
	prod = 0;
	v = start;
	prod = computeProd(p, u, v, r, t, A, B, C);
	prod = precision6(prod);
	result.push_back(prod);

	//Interpolate v Endpoints Separately for Poles
	u = 0;
	prod = 0;
	v = end;
	prod = computeProd(p, u, v, r, t, A, B, C);
	prod = precision6(prod);
	result.push_back(prod);

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
void coordinate3(vector<double> ux, vector<double> uy, vector<double> uz) {
	cout << "Coordinate3 {" << "\n";
	cout << "point[" << "\n";
	for (int i = 0; i < ux.size(); i++) {
		cout << ux[i] << " " << uy[i] << " " << uz[i] << "," << "\n";
	}
	cout << "] \n";
	cout << "} \n";

}

//Function for Normal Binding
void normalBinding(string s) {

	cout << "NormalBinding { \n";
	cout << "value \t" << s << "\n";
	cout << "} \n";
}

//Compute Normal for X coordinate
double computeNormx(double u, double v, double r, double t, double A, double B, double C) {
	double prod = 0;
	prod = (1/A) * c_func(v, 2-r) * c_func(u, 2-t);
	return prod;
}

//Compute Normal for Y coordinate
double computeNormy(double u, double v, double r, double t, double A, double B, double C) {
	double prod = 0;
	prod = (1/B) * c_func(v, 2-r) * s_func(u, 2-t);
	return prod;
}

//Compute Normal for Z coordinate
double computeNormz(double u, double v, double r, double t, double A, double B, double C) {
	double prod = 0;
	prod = (1/C) * s_func(v, 2-r);

	return prod;
	
}

//Function to Compute and Print Normals
void normal(double du, double dv, double r, double t, double A, double B, double C) {

	//print initial statements
	cout << "Normal  \t { \n";
	cout << "vector \t [ \n";

	double u = (-1 * pi);
	double v = (-1 * pi / 2);
	vector <double> resultx, resulty, resultz;
	double prodx = 0;
	double prody = 0;
	double prodz = 0;
	double start = (-1 * pi / 2);
	double end = (pi/2);
	double endu = (pi);

	//While u is less than pi
	while (u < (endu - epspi)) {

		v = start + dv;

		while (v < (end - epspi)) {

			//initialize Prod
			prodx = 0;
			prody = 0;
			prodz = 0;

			//Compute the Normals
			prodx = computeNormx(u, v, r, t, A, B, C);
			prody = computeNormy(u, v, r, t, A, B, C);
			prodz = computeNormz(u, v, r, t, A, B, C);

			//increment v
			v = v + dv;

			prodx = precision6(prodx);
			prody = precision6(prody);
			prodz = precision6(prodz);
			resultx.push_back(prodx);
			resulty.push_back(prody);
			resultz.push_back(prodz);

		}

		//increment u
		u = u + du;
	}

	//If u exceeds pi or is equal to pi, set u as pi

	if (u >= (endu - epspi)) {
		u = endu;
		v = start + dv;

		while (v < (end - epspi)) {

			//initialize prod
			prodx = 0;
			prody = 0;
			prodz = 0;

			//Compute the Normals
			prodx = computeNormx(u, v, r, t, A, B, C);
			prody = computeNormy(u, v, r, t, A, B, C);
			prodz = computeNormz(u, v, r, t, A, B, C);

			//increment v
			v = v + dv;

			prodx = precision6(prodx);
			prody = precision6(prody);
			prodz = precision6(prodz);
			resultx.push_back(prodx);
			resulty.push_back(prody);
			resultz.push_back(prodz);

		}
	}

	//Interpolate v Endpoints Separately for Poles - v START (-PI/2)
	u = 0;
	prodx = 0;
	prody = 0;
	prodz = 0;
	v = start;
	prodx = computeNormx(u, v, r, t, A, B, C);
	prody = computeNormy(u, v, r, t, A, B, C);
	prodz = computeNormz(u, v, r, t, A, B, C);
	
	prodx = precision6(prodx);
	prody = precision6(prody);
	prodz = precision6(prodz);
	resultx.push_back(prodx);
	resulty.push_back(prody);
	resultz.push_back(prodz);

	//Interpolate v Endpoints Separately for Poles - V END (PI/2)
	u = 0;
	prodx = 0;
	prody = 0;
	prodz = 0;
	v = end;
	prodx = computeNormx(u, v, r, t, A, B, C);
	prody = computeNormy(u, v, r, t, A, B, C);
	prodz = computeNormz(u, v, r, t, A, B, C);

	prodx = precision6(prodx);
	prody = precision6(prody);
	prodz = precision6(prodz);
	resultx.push_back(prodx);
	resulty.push_back(prody);
	resultz.push_back(prodz);

	for(int i = 0; i < resultx.size(); i++) {
		cout << resultx[i] << " " << resulty[i] << " " << resultz[i] << ", \n";
	}

	cout << "] \n";
	cout << "} \n";

}

//Function to Print Flat Shade Coordinate Indices
void flatshade(double un, double vn) {

	int a, b, c, d;
	a = 0;
	b = 1;

	//Outer loop for u_num
	for (int i = 0; i < un - 1; i++) {

		//Reset u after vn

		a = i*(vn-2);
		b = a + 1;

		//Inner Loop for v_num
		for (int j = 0; j < vn - 3; j++) {

			//Inrement vn as per i and j

			c = (i + 1)*(vn-2) + j;
			d = c + 1;

			cout << a << ", " << c << " ," << b << ", " << "-1" << ", " << "\n";
			cout << b << ", " << c << " ," << d << ", " << "-1" << ", " << "\n";

			a = a + 1;
			b = b + 1;
		}

	}

	//Poles Logic - POLE 1
	for (int i = 0; i < un - 1; i++) {

		a = i*(vn - 2);
		b = a + (vn - 2);

		c = (un)*(vn - 2);
		cout << a << ", " << c << " ," << b << ", " << "-1" << ", " << "\n";

	}

	//POLE 2
	for (int i = 0; i < un - 1; i++) {

		a = i*(vn - 2) + (vn-3);
		b = a + (vn - 2) ;

		c = (un)*(vn - 2) + 1;

		cout << a << ", " << b << " ," << c << ", " << "-1" << ", " << "\n";
	}
	
}

//Function to print out Indexes of points
void indexedfaceset(vector<double> ux, vector<double> uy, vector<double> uz, double un, double vn, int option) {

	cout << "IndexedFaceSet {" << "\n";
	cout << "coordIndex [" << "\n";

	flatshade(un, vn);

	cout << "] \n";
	cout << "} \n";
}

//ShapeHints Function
void shapeHints(string s) {
	cout << "ShapeHints { \n";
	cout << "vertexOrdering \t" << s << "\n";
	cout << "} \n";
}

//end Separator Function
void endSeparator() {

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
		param.setValues((2*pi) / (default_du_num - 1), (pi) / (default_dv_num - 1), default_r, default_t, default_A, default_B, default_C, default_op);

	//Compute general superellipsoid
	vector <double> ux = superellipsoid(0, param.getDu(), param.getDv(), param.getr(), param.gett(), param.getA(), param.getB(), param.getC());
	vector <double> uy = superellipsoid(1, param.getDu(), param.getDv(), param.getr(), param.gett(), param.getA(), param.getB(), param.getC());
	vector <double> uz = superellipsoid(2, param.getDu(), param.getDv(), param.getr(), param.gett(), param.getA(), param.getB(), param.getC());

	//Print out in .iv format
	initial();
	cout << "\n";
	separator();

	shapeHints("COUNTERCLOCKWISE");

	coordinate3(ux, uy, uz);

	//For Smooth Shaded Tessellated Patch
	if (param.getOp() > 0) {
		normalBinding("PER_VERTEX_INDEXED");
		normal(param.getDu(), param.getDv(), param.getr(), param.gett(), param.getA(), param.getB(), param.getC());
	}

	//Indexed Face Set 
	int num_v = ceil(ux.size() / ((2 * pi) / param.getDu() + 1) + 1);
	int num_u = ceil(ux.size() / (num_v-2));
	indexedfaceset(ux, uy, uz, num_u, num_v, param.getOp());

	endSeparator();

	return 0;

}
