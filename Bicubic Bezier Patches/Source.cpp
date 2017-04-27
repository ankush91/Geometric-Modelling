#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <string>
#include <limits>
#include<cmath>
	using namespace std;

//default command line paramaters
const double default_radius = 0.1;
const double default_du_num = 11.0;
const double default_dv_num = 11.0;
const string default_filename = "patchPoints.txt";
const int default_op = 0;
const double eps = std::numeric_limits<double>::epsilon();

//Class Paramaters 
class Parameters
{
	double du, dv, radius;
	string filename;
	int op;

public:
	void setValues(string, double, double, double, int);
	string getFileName();
	double getDu();
	double getDv();
	double getRadius();
	int getOp();

};

//Function to set values from command line
void Parameters::setValues(string f, double u, double v,double r, int option)
{
	du = u;
	dv = v;
	radius = r;
	filename = f;
	op = option;
}

//Functions to retrieve Parameters 
string Parameters::getFileName()
{
	return filename;
}

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

	string f;
	double u, v, r;
	int op;

	string found1 = extractArgument(args, "-f", len);
	string found2 = extractArgument(args, "-u", len);
	string found3 = extractArgument(args, "-r", len);
	string found4 = extractArgument(args, "-v", len);
	int model = findPoly(args, len);

	//Found f in args
	if (found1 != "") {
		f = found1;
	}
	else
		//else assign default filename
		f = default_filename;

	//find u in args 
	if (found2 != "") {
		u = 1.0/(atof(found2.c_str())-1.0);
	}
	else
		//else compute default du
		u = 1/(default_du_num-1);

	//find v in args 
	if (found4 != "") {
		v = 1/(atof(found4.c_str())-1);
	}
	else
		//else compute default dv
		v = 1.0/(default_dv_num-1.0);

	//find r in args
	if (found3 != "") {
		r = atof(found3.c_str());
	}
	//else assign default radius
	else {
		r = default_radius;
	}

	//find Poylgon Approximation Type in Argument
	if (model > -1) {
		op = model;
	}
	//else assign default radius
	else {
		op = default_op;
	}

	param.setValues(f, u, v, r, op);

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
double fact(double num) {

	return (num == 1 || num == 0) ? 1 : fact(num - 1)*num;

}

//Combinatorics Function
double comb(double k, double i)
{
	double result = fact(k) / (fact(i) * fact(k - i));
	return result;

}

//Function to Compute Sum
double computeSum(vector <double> p, double u, double v, double k) {

	double bexp1, bexp2;
	double sum = 0.0;
	double limit = sqrt(p.size());

		for (double i = 0; i <= k; i++) {

		//bernstein expression for i
		bexp1 = comb(k, i) * pow((1.0 - u), (k - i)) * pow(u, i);

			for (double j = 0; j <= k; j++) {

				//bernstein expression for j
				bexp2 = comb(k, j) * pow((1.0 - v), (k - j)) * pow(v, j);

				//calculate sum
				sum = sum + (p[j + (i*limit)] * bexp1 * bexp2);

			}

	}

	return sum;

}

//Bezier Patch Function - Generation of Points
vector <double> bezier(vector <double> p, double du, double dv) {

	double u = 0;
    double v = 0;
	vector <double> result;
	double k = sqrt(p.size()) - 1;
	double sum = 0;
	double unity = 1;
	double bexp1, bexp2;

	//While u is less than 1
	while (u < 1.0) {

		v = 0;

		while (v < 1.0) {

			//initialize sum
			sum = 0;

			//Compute the Sum
			sum = computeSum(p, u, v, k);

			//increment v
			v = v + dv;
			result.push_back(sum);
		}

		//If v exceeds 1 or is equal to 1, set v as 1

		if (v >= unity) {
			v = 1;
			sum = 0;

			sum = computeSum(p, u, v, k);

			result.push_back(sum);
		}

		//increment u
		u = u + du;
	}

	//If u exceeds 1 or is equal to 1, set u as 1

	if (u >= unity) {
		u = 1;
		sum = 0;
		v = 0;

		while (v < 1.0) {

			//initialize sum
			sum = 0;

			//Compute the Sum
			sum = computeSum(p, u, v, k);

			//increment v
			v = v + dv;
			result.push_back(sum);
		}

		//If v exceeds 1 or is equal to 1, set v as 1

		if (v >= unity) {
			v = 1;
			sum = 0;

			sum = computeSum(p, u, v, k);

			result.push_back(sum);
		}

	}

	return result;

}


//Function to print out initial comments
void initial(vector<double> x, vector<double> y, vector<double> z) {
	cout << "#Inventor V2.0 ascii \n";
	cout << "# control points \n";
	for (int i = 0; i < x.size(); i++)
		cout << "#" << " " << x[i] << " " << y[i] << " " << z[i] << " " << "\n";

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

//Compute the Tangent at s and return result
double convertNorm1(double g[4][4], double s, double t, double M[4][4]) {

	vector <double> mt = {t*t*t, t*t, t, 1};
	vector <double> dt = { 3*t*t, 2*t, 1, 0 };

	vector <double> ms = { s*s*s, s*s, s, 1 };
	vector <double> ds = { 3*s*s, 2*s, 1, 0 };

	//Declarations
	double rs1[1][4];
	double rs2[1][4];
	double rs3[1][4];
	double rs4 = 0;

	double Mtrans[4][4];

	//Initializations of Intermediate Results
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 4; j++) {
			rs1[i][j] = 0;
			rs2[i][j] = 0;
			rs3[i][j] = 0;
		}

	}

	//Compute Transpose for M
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Mtrans[i][j] = M[j][i];
		}
			
	}

	//Multiplication 1

	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 4; j++) {

			for (int k = 0; k < 4; k++) {

				rs1[i][j] += mt[k] * Mtrans[k][j];
			}

		}
	  }

	//Multiplication 2
		for (int i = 0; i < 1; i++) {
			for (int j = 0; j < 4; j++) {

				for (int k = 0; k < 4; k++) {

					rs2[i][j] += rs1[i][k] * g[k][j];
				}	
			}
		}
			
	//Multiplication 3
			for (int i = 0; i < 1; i++) {
				for (int j = 0; j < 4; j++) {

					for (int k = 0; k < 4; k++) {

						rs3[i][j] += rs2[i][k] * M[k][j];
					}
					
				}
			}

	//Multiplication 4
				for (int i = 0; i < 1; i++) {
					for (int j = 0; j < 1; j++) {

						for (int k = 0; k < 4; k++) {

							rs4 += rs3[i][k] * ds[k];
						}
					}
				}

	return rs4;
}

//Compute the Tangent at t and return result
double convertNorm2(double g[4][4], double s, double t, double M[4][4]) {

	vector <double> mt = { t*t*t, t*t, t, 1 };
	vector <double> dt = { 3 * t*t, 2 * t, 1, 0 };

	vector <double> ms = { s*s*s, s*s, s, 1 };
	vector <double> ds = { 3 * s*s, 2 * s, 1, 0 };

	//Declarations
	double rs1[1][4];
	double rs2[1][4];
	double rs3[1][4];
	double rs4 = 0;

	double Mtrans[4][4];

	//Initializations of Intermediate Results
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 4; j++) {
			rs1[i][j] = 0;
			rs2[i][j] = 0;
			rs3[i][j] = 0;
		}

	}

	//Compute Transpose for M
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Mtrans[i][j] = M[j][i];
		}

	}

	//Multiplication 1

	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 4; j++) {

			for (int k = 0; k < 4; k++) {

				rs1[i][j] += dt[k] * Mtrans[k][j];
			}
		}
	}

	//Multiplication 2
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 4; j++) {

			for (int k = 0; k < 4; k++) {

				rs2[i][j] += rs1[i][k] * g[k][j];
			}
		}
	}

	//Multiplication 3
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 4; j++) {

			for (int k = 0; k < 4; k++) {

				rs3[i][j] += rs2[i][k] * M[k][j];
			}
		}
	}

	//Multiplication 4
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 1; j++) {

			for (int k = 0; k < 4; k++) {

				rs4 += rs3[i][k] * ms[k];
			}
		}
	}

	return rs4;
}

//Function to Compute and Print Normals
void normal(vector<double> x, vector<double> y, vector<double> z, double du, double dv) {

	//print initial statements
	cout << "Normal  \t { \n";
	cout << "vector \t [ \n";

	//Declare M
	double M[4][4] = { 
						{-1, 3, -3, 1}, 
						{3, -6, 3, 0}, 
						{-3, 3, 0, 0},
						{1, 0, 0, 0}
					};

	//Initializations
	int limit = sqrt(x.size());
	double Gx[4][4], Gy[4][4], Gz[4][4];
	double Nxs, Nys, Nzs;
	double Nxt, Nyt, Nzt;
	double u = 0;
	double v =0;
	vector <double> resultx;
	vector <double> resulty;
	vector <double> resultz;
	double unity = 1;

	//Convert x, y and z to 2D Matrix Form

	for (int i = 0; i < 4; i++) {

		for (int j = 0; j < 4; j++) {

			Gx[i][j] = x[i + j*limit];
			Gy[i][j] = y[i + j*limit];
			Gz[i][j] = z[i + j*limit];


		}
	}

	//While u is less than 1
	while (u < 1.0) {

		//initialize Nx, Ny and Nz
		Nxs = 0; Nys = 0; Nzs = 0;
		Nxt = 0; Nyt = 0; Nzt = 0;

		v = 0;

		while (v < 1.0) {

			//initialize Nx, Ny and Nz
			Nxs = 0; Nys = 0; Nzs = 0;
			Nxt = 0; Nyt = 0; Nzt = 0;

			//Compute Nx, Ny and Nz
			Nxs = convertNorm1(Gx, u, v, M);
			Nys = convertNorm1(Gy, u, v, M);
			Nzs = convertNorm1(Gz, u, v, M);

			Nxt = convertNorm2(Gx, u, v, M);
			Nyt = convertNorm2(Gy, u, v, M);
			Nzt = convertNorm2(Gz, u, v, M);

			//increment v
			v = v + dv;
			
			resultx.push_back((Nys*Nzt) - (Nyt*Nzs));
			resulty.push_back((Nzs*Nxt) - (Nzt*Nxs));
			resultz.push_back((Nxs*Nyt) - (Nxt*Nys));

		}

		//If v exceeds 1 or is equal to 1, set v as 1

		if (v >= unity) {
			v = 1;

			//initialize Nx, Ny and Nz
			Nxs = 0; Nys = 0; Nzs = 0;
			Nxt = 0; Nyt = 0; Nzt = 0;

			//Compute Nx, Ny and Nz
			//Compute Nx, Ny and Nz
			Nxs = convertNorm1(Gx, u, v, M);
			Nys = convertNorm1(Gy, u, v, M);
			Nzs = convertNorm1(Gz, u, v, M);

			Nxt = convertNorm2(Gx, u, v, M);
			Nyt = convertNorm2(Gy, u, v, M);
			Nzt = convertNorm2(Gz, u, v, M);

			resultx.push_back((Nys*Nzt) - (Nyt*Nzs));
			resulty.push_back((Nzs*Nxt) - (Nzt*Nxs));
			resultz.push_back((Nxs*Nyt) - (Nxt*Nys));
		}

		//increment u
		u = u + du;
	}

	//If u exceeds 1 or is equal to 1, set u as 1

	if (u >= unity) {
		u = 1;
		v = 0;

		//initialize Nx, Ny and Nz
		Nxs = 0; Nys = 0; Nzs = 0;
		Nxt = 0; Nyt = 0; Nzt = 0;

		while (v < 1.0) {

			//initialize Nx, Ny and Nz
			Nxs = 0; Nys = 0; Nzs = 0;
			Nxt = 0; Nyt = 0; Nzt = 0;

			//Compute Nx, Ny and Nz
			//Compute Nx, Ny and Nz
			Nxs = convertNorm1(Gx, u, v, M);
			Nys = convertNorm1(Gy, u, v, M);
			Nzs = convertNorm1(Gz, u, v, M);

			Nxt = convertNorm2(Gx, u, v, M);
			Nyt = convertNorm2(Gy, u, v, M);
			Nzt = convertNorm2(Gz, u, v, M);


			//increment v
			v = v + dv;
			
			resultx.push_back((Nys*Nzt) - (Nyt*Nzs));
			resulty.push_back((Nzs*Nxt) - (Nzt*Nxs));
			resultz.push_back((Nxs*Nyt) - (Nxt*Nys));
		}

		//If v exceeds 1 or is equal to 1, set v as 1

		if (v >= unity) {
			v = 1;
			
			//initialize Nx, Ny and Nz
			Nxs = 0; Nys = 0; Nzs = 0;
			Nxt = 0; Nyt = 0; Nzt = 0;

			//Compute Nx, Ny and Nz
			Nxs = convertNorm1(Gx, u, v, M);
			Nys = convertNorm1(Gy, u, v, M);
			Nzs = convertNorm1(Gz, u, v, M);

			Nxt = convertNorm2(Gx, u, v, M);
			Nyt = convertNorm2(Gy, u, v, M);
			Nzt = convertNorm2(Gz, u, v, M);

			resultx.push_back((Nys*Nzt) - (Nyt*Nzs));
			resulty.push_back((Nzs*Nxt) - (Nzt*Nxs));
			resultz.push_back((Nxs*Nyt) - (Nxt*Nys));
		}

	}

	for (int i = 0; i < resultx.size(); i++) {
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
	for (int i = 0; i < un -1; i++) {

		//Reset u after vn

		a = i*vn;
		b = a + 1;

		//Inner Loop for v_num
		for (int j = 0; j < vn -1; j++) {

			//Inrement vn as per i and j

			c = (i + 1)*vn + j;
			d = c + 1;

			cout << a << ", " << c << " ," << b << ", " << "-1" << ", " << "\n";
			cout << b << ", " << c << " ," << d << ", " << "-1" << ", " << "\n";

			a = a + 1;
			b = b + 1;
		}

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

//ShapeHints Function
void shapeHints(string s) {
	cout << "ShapeHints { \n";
	cout << "vertexOrdering \t" << s << "\n";
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
		param.setValues(default_filename, 1/(default_du_num-1), 1/(default_dv_num-1), default_radius, default_op);

	//File Operations - Read Control Points from File
	vector<double> points = fileRead(param.getFileName());
	double x, y, z;
	vector<double> xp, yp, zp;
	int j = 0;

	//Fill in Coordinate Vectors
	for (int i = 0; i < points.size() - 2; i = i + 3) {
		xp.push_back(points[i]);
		yp.push_back(points[i + 1]);
		zp.push_back(points[i + 2]);
		j++;
	}

	//Compute Bi-cubic Bezier Patch
	vector <double> ux = bezier(xp, param.getDu(), param.getDv());
	vector <double> uy = bezier(yp, param.getDu(), param.getDv());
	vector <double> uz = bezier(zp, param.getDu(), param.getDv());

	//Print out in .iv format
	initial(xp, yp, zp);
	cout << "\n";
	separator();

	shapeHints("COUNTERCLOCKWISE");

	coordinate3(ux, uy, uz);

	//For Smooth Shaded Tessellated Patch
	if (param.getOp() > 0) {
		normalBinding("PER_VERTEX_INDEXED");
		normal(xp, yp, zp, param.getDu(), param.getDv());
	}

	//Indexed Face Set 
	indexedfaceset(ux, uy, uz, (1/param.getDu())+1, (1/param.getDv())+1, param.getOp());

	endSeparator();

	//print control points
	for (int i = 0; i < xp.size(); i++) {
		separator();
		lightmodel("PHONG");
		material(1.0, 1.0, 1.0);
		transform(xp[i], yp[i], zp[i]);
		sphere(param.getRadius());
		endSeparator();
	}
	
	return 0;

}
