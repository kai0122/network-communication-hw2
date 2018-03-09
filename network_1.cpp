// 0416303
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <random>
#include <math.h>
#include <algorithm>
#include <vector>
using namespace std;

int main(void)
{	
	const long long nrolls = 10000000;  // number of experiments
	const double mean = 0.5;  // mu number of poisson distribution
	srand (time(NULL));
	double poisson1 = pow(0.5 * mean, 1) * exp(-0.5 * mean) / 1;
	double poisson0 = 1 - poisson1;
	
	long long last = 0;
	long long callNum = 0;
	vector<int> timeLine;
	
	for (long long int i=1; i<nrolls+1; ++i) {
		double p = (double)(rand()%1001)/1000;
		if(p > poisson0){
			// has a call and cell is empty
			callNum = callNum + 1;
			timeLine.push_back(i-last);  // push the difference to vector
			last = i;  // update difference
		}
	}
	
	cout << "Question 1: request probability = " << (double)callNum/nrolls << endl;	
	cout << "\nX axis data: \n";
	
	ofstream myfile;
	myfile.open ("network_1.txt");
	
	auto iter = max_element(std::begin(timeLine), std::end(timeLine)); // c++11
	for(int num=1;num<*iter;num++){
		long long int count = 0;
		for(vector<int>::iterator it = timeLine.begin();it != timeLine.end();++it){
			if(num == *it)
				count++;
		}
		myfile << count << "\n";  // print out the count of each difference 
		cout << num-1 << ": " << count << endl;
	}
	myfile.close();
	
	system("pause");
	return 0;
} 
