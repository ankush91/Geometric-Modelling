#include <iostream>
#include <vector>
#include <fstream>
#include<cstdlib>
#include <string>
#include <limits>
#include<cmath>
using namespace std;

//default command line paramaters
const double default_radius = 1.0;
const double default_du = 0.09;
const string default_filename = "cpts_in.txt";
const double eps = std::numeric_limits<double>::epsilon();

//Class Paramaters 
class Parameters
{
	double du, radius;
	string filename;

public:
	void setValues(string, double, double);
	string getFileName();
	double getDu();
	double getRadius();


};

//Function to set values from command line
void Parameters::setValues(string f, double u, double r)
{
	du = u;
	radius = r;
	filename = f;
}

//Functions to retrieve Parameters 
string Parameters::getFileName()
{
	return filename;
}

double Parameters::getDu()
{
	return du;
}

double Parameters::getRadius()
{
	return radius;
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

	string f;
	double u, r;

	string found1 = extractArgument(args, "-f", len);
	string found2 = extractArgument(args, "-u", len);
	string found3 = extractArgument(args, "-r", len);

	if (found1 != "") {
		f = found1;
	}
	else
		f = default_filename;

	if (found2 != "") {
		u = atof(found2.c_str());
	}
	else
		u = default_du;

	if (found3 != "") {
		r = atof(found3.c_str());
	}
	else {
		r = default_radius;
	}

	param.setValues(f, u, r);

}

//Read in Points from file
vector <double> fileRead(string filename) {
	vector<double> points;

	const char* str = filename.c_str();
	ifstream file;
	string line;
	double x, y, z;

	file.open(str);

	//Check if File can be opened And/Or Corrupted
	if (file.fail()) {
		cout << "Error opening file \n";
		exit(-1);
	}

	//Read in Coordinates of Input Points & Tangent Vectors
	else {
		int i = 0;
		while (getline(file, line)) {
			sscanf(line.c_str(), "%lf %lf %lf", &x, &y, &z);
			points.push_back(x);
			points.push_back(y);
			points.push_back(z);
			i++;
		}
		file.close();

	}

	return points;
}

//Facotrial Function
double fact(double num) {

	return (num == 1 || num == 0) ? 1 : fact(num - 1)*num;

}

//Combinatorics Function
double comb(double k, double i)
{
	double result = fact(k) / (fact(i) * fact(k - i));
	return result;

}

//Bezier Curve Function - Generation of Points on Curve
vector <double> bezier(vector <double> v, double du) {

	double u = 0;
	vector <double> result;
	double k = v.size() - 1;
	double sum = 0;
	double unity = 1;

	//While u is less than 1
	while (u < 1.0) {
		sum = 0;
		for (double i = 0; i <= k; i++) {
			sum = sum + (v[i] * comb(k, i) * pow((1.0 - u), (k - i)) * pow(u, i));
		}

		result.push_back(sum);
		u = u + du;
	}

	//If u exceeds 1 or is equal to 1, set u as 1

	if (u >= unity) {
		u = 1;
		sum = 0;
		for (int i = 0; i <= k; i++) {
			sum = sum + (v[i] * comb(k, i)* pow((1 - u), (k - i)) * pow(u, i));
		}
		result.push_back(sum);
	}

	return result;

}

//Seperate Case if there are only 2 Input points
vector <double> twoPointCase(vector <double> p, double t0, double t1) {

	vector<double>points;
	double p1, p2;

	//P0
	points.push_back(p[0]);

	//Equation for P1
	p1 = p[0] + (t0 / 3);
	points.push_back(p1);

	//Equation for P2
	p2 = p[1] - (t1 / 3);
	points.push_back(p2);

	//P3
	points.push_back(p[1]);

	return points;
}

//Return Control Points for 1st Piece of Curve
vector <double> firstPiece(vector <double> p, double t0, double tn) {

	vector<double>points;
	double p1, p2, t1;

	//P0
	points.push_back(p[0]);

	//Equation for P1
	p1 = p[0] + (t0 / 3);
	points.push_back(p1);

	//Equation for t1
	t1 = 0.5 * (p[2] - p[0]);

	//Equation for P2
	p2 = p[1] - (t1 / 3);
	points.push_back(p2);

	//P3
	points.push_back(p[1]);

	return points;
}

//Return Control Points for last Piece of Curve
vector <double> lastPiece(vector <double> p, double t0, double tn) {

	vector<double>points;
	double p1n, p2n, t0n;

	//P0n
	points.push_back(p[p.size()-2]);

	//Equation for P2n
	p2n = p[p.size()-1] - (tn / 3);

	//Equation for t2n
	t0n = 0.5 * (p[p.size()-1] - p[p.size()-3]);

	//Equation for P1n
	p1n =  p[p.size()-2] + (t0n / 3);
	
	//P1n..P3n
	points.push_back(p1n);
	points.push_back(p2n);
	points.push_back(p[p.size()-1]);

	return points;
}

//Concatenate Control Points for Pieces
vector <double> concatenate(vector <double> p0, vector <double> pi, vector <double> pn) {
	vector <double> p;

	//Concatenate Vectors using reserve and insert
	p.reserve(p0.size() + pi.size() + pn.size());
	p.insert(p.end(), p0.begin(), p0.end());
	p.insert(p.end(), pi.begin(), pi.end());
	p.insert(p.end(), pn.begin(), pn.end());

	return p;

}

//Generate Control Points for Each Piece Wise Curve
vector <double> generateControlPoints(vector <double> p, double t0, double tn){
	
	vector<double>points, piece_first, piece_intermediate, piece_last;
	double pi1, pi2, ti1, ti0;

	//Simplified Case if only 2 points or 1 curve
	if (p.size() == 2) {
		points = twoPointCase(p, t0, tn);
		return points;
	}

	//Case for More than 1 Piece Wise Curve
	else {
		//return Control Points for 1st Piece
		piece_first = firstPiece(p, t0, tn);

		//return Control Points for Intermediate Pieces
		for (int i = 1; i < p.size() - 2; i++) {

			//Tangent at Starting point of new piece
			ti0 = 0.5*(p[i + 1] - p[i-1]);

			//Pi
			piece_intermediate.push_back(p[i]);

			//Equation for Pi1
			pi1 = p[i] + (ti0 / 3);
			piece_intermediate.push_back(pi1);

			//Equation for ti1
			ti1 = 0.5 * (p[i+2] - p[i]);

			//Equation for Pi2
			pi2 = p[i+1] - (ti1 / 3);
			piece_intermediate.push_back(pi2);

			//Pi3
			piece_intermediate.push_back(p[i+1]);	

		}

		//return Control Points for Last Piece
		piece_last = lastPiece(p, t0, tn);

		//Return Single Vector of all Control Points
		points = concatenate(piece_first, piece_intermediate, piece_last);

		return points;

	}


}


//Function to print out initial Inventor file Format
void initial() {
	cout << "#Inventor V2.0 ascii \n";

}

//Function to print out Separator
void separator() {
	cout << "Separator {" << "\n";

}

//Function to print out LightModel
void lightmodel(string model) {
	cout << "LightModel {" << "\n";
	cout << "model " << model << "\n";
	cout << "}" << "\n";
}

//Function to print out material
void material(double x, double y, double z) {
	cout << "Material {" << "\n";
	cout << "diffuseColor " << x << " " << y << " " << z << " " << "\n";
	cout << "}" << "\n";
}

//coordinate 3 start
void coordinate3Start() {
	cout << "Coordinate3 {" << "\n";
	cout << "point[" << "\n";
}

//coordinate 3 end
void coordinate3End() {
	cout << "] \n";
	cout << "} \n";

}

//Function to print out coordinate3
void coordinate3(vector<double> ux, vector<double> uy, vector<double> uz, int iteration) {
	
	int start;
	if (iteration == 0) {
		start = 0;
	}
	else {
		start = 1;
	}

	for (int i = start; i < ux.size(); i++) {
		cout << ux[i] << " " << uy[i] << " " << uz[i] << "," << "\n";
	}
	
}

//indexedlineset Start
void indexedlinesetStart() {
	cout << "IndexedLineSet {" << "\n";
	cout << "coordIndex [" << "\n";
}

//indexedlineset End
void indexedlinesetEnd() {
	cout << "] \n";
	cout << "} \n";
}

	int start_next =0;
//Function to print out Indexes of points
void indexedlineset(double limit, int iteration) {
	
	int offset;
	int start;
	if (iteration == 0) {
		start = 0;
	}
	else {
		start = start_next;
	}
	
	int end = (start) + limit;
	for (int i = start; i < end; i++) {
		cout << i << ", ";
	}
	cout << -1 << ", \n";
	
	start_next = end - 1;
}

//Function to print out transform
void  transform(double x, double y, double z) {
	cout << "Transform {" << "\n";
	cout << "translation " << x << " " << y << " " << z << " " << "\n";
	cout << "}" << "\n";

}

//Function for sphere
void  sphere(double r) {
	cout << "Sphere {" << "\n";
	cout << "radius " << r << "\n";
	cout << "}" << "\n";

}

//End Separator
void endSeparator() {
	cout << "} \n";

}

//***************MAIN FUNCTION**************

int main(int argc, char* argv[])
{
	Parameters param;

	//Function call to assign Command Line Arguments : if arguments are more than 1 then read them and fill in defualt values, else directly assign default values
	if (argc > 1)
		argumentAssign(argv, argc, param);
	else
		param.setValues(default_filename, default_du, default_radius);

	//File Operations - Read Control Points from File
	vector<double> points = fileRead(param.getFileName());
	double x, y, z;

	//Declare Vectors for inputs points and tangents
	vector<double> xp, yp, zp;
	double t0x, t0y, t0z;
	double t1x, t1y, t1z;
	vector<double> controlx, controly, controlz;

	int j = 0;

	//Read in Tangent Vector 0
	t0x = points[0]; t0y = points[1]; t0z = points[2];

	//Read in Tangent Vector 1
	t1x = points[3]; t1y = points[4]; t1z = points[5];

	//Fill in Inputs Points Coordinates
	for (int i = 6; i < points.size() - 2; i = i + 3) {
		xp.push_back(points[i]);
		yp.push_back(points[i + 1]);
		zp.push_back(points[i + 2]);
		j++;

	}

	//Generate 2 Control Points between each 2 Coordinate Vectors -- Repeat for X, Y and Z
	controlx = generateControlPoints(xp, t0x, t1x);
	controly = generateControlPoints(yp, t0y, t1y);
	controlz = generateControlPoints(zp, t0z, t1z);

	vector <double> tempx(4), tempy(4), tempz(4);

	//Print out in .iv format & Compute Piece-Wise Curves
	initial();
	cout << "\n";
	separator();
	lightmodel("BASE_COLOR");
	material(1, 1, 0.2);

	//size vector
	vector <double> size_set;

	coordinate3Start();

	//Bezier for X component, Y Component and Z Component for Each Piece
	for (int i = 0; i < controlx.size() - 3; i += 4) {

		tempx[0] = controlx[i]; tempx[1] = controlx[i + 1]; tempx[2] = controlx[i + 2]; tempx[3] = controlx[i + 3];
		tempy[0] = controly[i]; tempy[1] = controly[i + 1]; tempy[2] = controly[i + 2]; tempy[3] = controly[i + 3];
		tempz[0] = controlz[i]; tempz[1] = controlz[i + 1]; tempz[2] = controlz[i + 2]; tempz[3] = controlz[i + 3];

		vector <double> ux = bezier(tempx, param.getDu());
		vector <double> uy = bezier(tempy, param.getDu());
		vector <double> uz = bezier(tempz, param.getDu());

		coordinate3(ux, uy, uz, i);

		size_set.push_back(ux.size());
	}
	
	coordinate3End();

	//Indexed Line Set
	indexedlinesetStart();

	for (int i = 0; i < size_set.size(); i++) {
		indexedlineset(size_set[i], i);
	}
	
	indexedlinesetEnd();

	//print Input Points
	for (int i = 0; i < xp.size(); i++) {
		separator();
		lightmodel("PHONG");
		material(1.0, 1.0, 1.0);
		transform(xp[i], yp[i], zp[i]);
		sphere(param.getRadius());
		endSeparator();
	}
	cout << "\n";
	endSeparator();

	return 0;

}
