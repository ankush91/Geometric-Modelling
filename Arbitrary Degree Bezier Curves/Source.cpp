#include <iostream>
#include <vector>
#include <fstream>
#include<cstdlib>
#include <string>
#include <limits>
#include<cmath>
	using namespace std;
	
//default command line paramaters
const double default_radius = 0.1;
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
int found(char* s[], string sub, int len){
	int pos = 0;
	bool flag = false;

	while (pos < (len-1)) {

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
	if (a != -1){
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

	if (found1 != ""){
		f = found1;
	}
	else
		f = default_filename;

	if (found2 != ""){
		u = atof(found2.c_str());
	}
	else
		u = default_du;

	if (found3 != ""){
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

	//Read in Control Points
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
double fact(double num){
	
	return (num == 1 || num == 0) ? 1 : fact(num - 1)*num;

}

//Combinatorics Function
double comb(double k, double i)
{
	double result = fact(k) / ( fact(i) * fact(k - i) );
	return result;

}

//Bezier Curve Function - Generation of Control Points
vector <double> bezier(vector <double> v, double du){

	double u = 0;
	vector <double> result;
	double k = v.size()-1;
	double sum = 0;
	double unity = 1;

	//While u is less than 1
	while (u < 1.0){
		sum = 0;
		for (double i = 0; i <= k; i++){
			sum = sum + (v[i] * comb(k, i) * pow((1.0 - u),(k - i)) * pow(u, i));
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


//Function to print out initial comments
void initial(vector<double> x, vector<double> y, vector<double> z){
	    cout << "#Inventor V2.0 ascii \n";
		cout << "# control points \n";
	for (int i = 0; i < x.size(); i++)
		cout <<"#" << " " << x[i] << " " << y[i] << " " << z[i] << " " << "\n";

}

//Function to print out Separator
void separator() {
	cout << "Separator {" << "\n";

}

//Function to print out LightModel
void lightmodel(string model){
	cout << "LightModel {" << "\n";
	cout << "model " << model << "\n";
	cout << "}" << "\n";
}

 //Function to print out material
void material(double x, double y, double z) {
	cout << "Material {" << "\n";
	cout << "diffuseColor " << x << " "<< y << " " << z << " " << "\n";
	cout << "}" << "\n";
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

//Function to print out Indexes of points
void indexedlineset(vector<double> ux, vector<double> uy, vector<double> uz) {

	cout << "IndexedLineSet {" << "\n";
	cout << "coordIndex [" << "\n";
	for (int i = 0; i < ux.size(); i++) {
		cout << i << ", ";
	}
	cout << -1 << ", \n";
	cout << "] \n";
	cout << "} \n";
}

//Function to print out transform
void  transform(double x, double y, double z){
	cout << "Transform {" << "\n";
	cout << "translation " << x << " " << y << " " << z << " " << "\n";
	cout << "}" << "\n";

}

//Function for sphere
void  sphere(double r) {
	cout << "Sphere {" << "\n";
	cout << "radius "<< r << "\n";
	cout << "}" << "\n";

}

//End Separator
void endSeparator(){
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
	vector<double> xp, yp, zp;
	int j = 0;

	//Fill in Coordinate Vectors
	for (int i = 0; i < points.size()-2; i=i+3){
		xp.push_back(points[i]);
		yp.push_back(points[i+1]);
		zp.push_back(points[i+2]);
		j++;
	}

	//Bezier for X component, Y Component and Z Component
	vector <double> ux = bezier(xp, param.getDu());
	vector <double> uy = bezier(yp, param.getDu());
	vector <double> uz = bezier(zp, param.getDu());

	//Print out in .iv format
	initial(xp, yp, zp);
	cout << "\n";
	separator();
	lightmodel("BASE_COLOR");
	material(1.0,1.0,0.2);
	coordinate3(ux, uy, uz);
	indexedlineset(ux, uy, uz);

	//print control points
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
