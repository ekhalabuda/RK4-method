/*#include "numcpp.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

using namespace std;
int main() {

	return 1;
}*/
/*
void write(const vector<double>& x, const vector<double>& v) {
	string path = "values.txt";
	ofstream fout;
	fout.open(path);

	for (auto i : x)
		fout << i << ' ';
	fout << '\n';

	for (auto i : v)
		fout << i << ' ';

	fout.close();
}

#define System_problem
#ifdef System_problem

int main(int argc, char* argv[]) {
	
	double start = stod(argv[1]);
	double stop = stod(argv[2]);
	double step = stod(argv[3]);
	double precision = stod(argv[4]);
	double max_step = stod(argv[5]);
	double epsilon = stod(argv[6]);
    double V = stod(argv[7]);
    double R = stod(argv[8]);
    double L = stod(argv[9]);
	double iv = stod(argv[10]);
	bool dif_step = static_cast<bool>(stod(argv[11]));
	
    function<double(double, double)> current_problem = [ V, R, L](double x, double I){return (V - R*I)/L;};

	std::pair<std::vector<double>, std::vector<double>> p = numcpp::RK4(start, stop, iv, current_problem, step, precision, max_step, dif_step, epsilon);
	std::vector<double> x = p.first;
	std::vector<double> v = p.second;

	write(x, v);
	
	return 0;
}

#endif
*/
