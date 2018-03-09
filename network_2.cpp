// 0416303
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <random>
#include <math.h>
#include <algorithm>
#include <vector>
using namespace std;

int maximum(int a, int b, int c){
	if(a >= max(b, c))
		return a;
	else if(b >= max(a, c))
		return b; 
	else
		return c;
}

int main(void)
{	
	// **************************************************************
	// *						Question One						*
	// **************************************************************
	
	const long long nrolls = 100*120;  // number of experiments
	long long addNum = 0;
	double blockProb = 0.0;
	int cells[12] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};  // 12 cells , 5 channel for each
	srand (time(NULL));
	
	
	for (long long int count=1; count<nrolls+1; ++count) {
		long long blockNum = 0;  // count numbers of requests that has been blocked
		long long in = 0;  // count numbers of requests
		
		for(int i=1;i<=12;i++){
			
			// STEP 1 : Find if someone leave
			
			double p = (double)(rand()%1001)/1000;
			double mu = (double)((i % 4)+1) * 0.2 * (5 - cells[i-1]) / 60;  // survice rate
			double poisson1 = (double) pow(0.5 * mu, 1) * exp(-0.5 * mu) / 1;
			double poisson0 = 1 - poisson1;
			
			if(cells[i-1] < 5 && p > poisson0){  // someone leave and has people in channel
				cells[i-1]++;  // add back useful channel number
			}
			
			// STEP 2 : Find if someone gets in
			
			p = (double)(rand()%1001)/1000;
			double lamda = (double)((i % 4)+1) * 1 / 60;
			poisson1 = (double) pow(0.5 * lamda, 1) * exp(-0.5 * lamda) / 1; // arrival rate
			poisson0 = 1 - poisson1;
			
			if(p > poisson0){  // someone gets in
				in++;
				if(cells[i-1] > 0){  // channel not full
					cells[i-1]--;
				}
				else if(cells[i-1] == 0){  // no channel left
					blockNum++;
				}	
			}
		}
		if(in != 0){
			blockProb += (double)blockNum/in;
			addNum++;
		}
	}
	cout << "Question 1 : blocking probability = " << blockProb/addNum << endl;

	// **************************************************************
	// *						Question Two						*
	// **************************************************************
	
	addNum = 0;
	blockProb = 0.0;
	for(int i=0;i<12;i++)  // 12 cells , 5 channel for each
		cells[i] = 5;
	
	int cellsBlock[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // 12 cells , calculate if any one may block a request
	int borrow[12][12];
	
	for(int i=0;i<12;i++){  // 12 cells , calculate who did I borrow a channel from
		for(int j=0;j<12;j++)
			borrow[i][j] = 0;
	}
	
	
	for (long long int count=1; count<nrolls+1; ++count) {
		long long blockNum = 0;  // count numbers of requests that has been blocked
		long long in = 0;  // count numbers of requests
		
		for(int i=1;i<=12;i++){
			
			// *********************** STEP 1 : Find if someone leave ************************
			
			double p = (double)(rand()%1001)/1000;
			double mu = (double)((i % 4)+1) * 0.2 * (5 - cells[i-1]) / 60;  // survice rate
			double poisson1 = (double) pow(0.5 * mu, 1) * exp(-0.5 * mu) / 1;
			double poisson0 = 1 - poisson1;
			
			if(cells[i-1] < 5 && p > poisson0){ // someone leave and has people in channel
				int borrowFrom = 0;
				for(int bor_i = 1;bor_i<=12;bor_i++){
					if(borrow[i-1][bor_i-1] > 0){  // if I have borrowed from BS bor_i
						borrow[i-1][bor_i-1]--;
						borrowFrom = bor_i;  // borrowed from BS bor_i
						break;
					}
				}
				if(borrowFrom == 0)
					cells[i-1]++;  // no borrowing from others , free myselves channel
				else
					cells[borrowFrom-1]++;  // free BS bor_i 's channel
			}
			
			// *********************** STEP 2 : Find if someone gets in ************************
			
			p = (double)(rand()%1001)/1000;
			double lamda = (double)((i % 4)+1) * 1 / 60;
			poisson1 = (double) pow(0.5 * lamda, 1) * exp(-0.5 * lamda) / 1;  // arrival rate
			poisson0 = 1 - poisson1;
			
			if(p > poisson0){  // someone gets in
				in++;
				if(cells[i-1] > 0){  // channel not full
					cells[i-1]--;
				}
				else if(cells[i-1] == 0){  // no channel left
					cellsBlock[i-1] = 1;  // need to borrow ( may block )
				}	
			}
		}
		
		// Borrow from others
		 
		for(int i=1;i<=12;i++){
			if(cellsBlock[i-1] == 1){
				cellsBlock[i-1] = 0;  // dealing it
				
				switch(i){
					case 1:{
						if(cells[3-1] > 0 && cells[3-1] > cells[2-1] && cells[3-1] > cells[8-1]){
							cells[3-1]--;
							borrow[i-1][3-1]++;  // borrow from BS3
						}
						else if(cells[2-1] > 0 && cells[8-1] > 0){
							if(cells[2-1] > cells[8-1]){
								cells[2-1]--;
								borrow[i-1][2-1]++;  // borrow from BS2	
							}
							else{
								cells[8-1]--;
								borrow[i-1][8-1]++;  // borrow from BS8
							}
						}
						else
							blockNum++;
						break;
					}
					case 2:{
						if(cells[3-1] > 0 && cells[3-1] > cells[1-1] && cells[3-1] > cells[10-1]){
							cells[3-1]--;
							borrow[i-1][3-1]++;  // borrow from BS3
						}
						else if(cells[1-1] > 0 && cells[10-1] > 0){
							if(cells[1-1] > cells[10-1]){
								cells[1-1]--;
								borrow[i-1][1-1]++;  // borrow from BS1	
							}
							else{
								cells[10-1]--;
								borrow[i-1][10-1]++;  // borrow from BS10
							}
						}
						else
							blockNum++;
						break;
					}
					case 3:{
						if(cells[1-1] > 0 && cells[10-1] > 0 && cells[4-1] > 0 && (maximum(cells[1-1], cells[10-1], cells[4-1]) >= maximum(cells[2-1], cells[8-1], cells[5-1]) || cells[2-1] == 0 || cells[8-1] == 0 || cells[5-1] == 0)){
							if(cells[1-1] > max(cells[10-1], cells[4-1])){
								cells[1-1]--;
								borrow[i-1][1-1]++;  // borrow from BS1	
							}
							else if(cells[10-1] > max(cells[1-1], cells[4-1])){
								cells[10-1]--;
								borrow[i-1][10-1]++;  // borrow from BS10	
							}
							else{
								cells[4-1]--;
								borrow[i-1][4-1]++;  // borrow from BS14
							}
						}
						else if(cells[2-1] > 0 && cells[8-1] > 0 && cells[5-1] > 0){
							if(cells[2-1] > max(cells[8-1], cells[5-1])){
								cells[2-1]--;
								borrow[i-1][2-1]++;  // borrow from BS2
							}
							else if(cells[5-1] > max(cells[2-1], cells[8-1])){
								cells[5-1]--;
								borrow[i-1][5-1]++;  // borrow from BS5
							}
							else{
								cells[8-1]--;
								borrow[i-1][8-1]++;  // borrow from BS8
							}
						}
						else
							blockNum++;
						break;
					}
					case 4:{
						if(cells[3-1] > 0 && cells[6-1] > 0 && cells[9-1] > 0 && (maximum(cells[3-1], cells[6-1], cells[9-1]) >= max(cells[8-1], cells[5-1]) || cells[8-1] == 0 || cells[5-1] == 0)){
							if(cells[3-1] > max(cells[6-1], cells[9-1])){
								cells[3-1]--;
								borrow[i-1][3-1]++;  // borrow from BS3	
							}
							else if(cells[6-1] > max(cells[3-1], cells[9-1])){
								cells[6-1]--;
								borrow[i-1][6-1]++;  // borrow from BS6
							}
							else{
								cells[9-1]--;
								borrow[i-1][9-1]++;  // borrow from BS9
							}
						}
						else if(cells[8-1] > 0 && cells[5-1] > 0){
							if(cells[8-1] > cells[5-1]){
								cells[8-1]--;
								borrow[i-1][8-1]++;  // borrow from BS8	
							}
							else{
								cells[5-1]--;
								borrow[i-1][5-1]++;  // borrow from BS5	
							}
						}
						else
							blockNum++;
						break;
					}
					case 5:{
						if(cells[3-1] > 0 && cells[6-1] > 0 && cells[12-1] > 0 && (maximum(cells[3-1], cells[6-1], cells[12-1]) >= max(cells[4-1], cells[10-1]) || cells[4-1] == 0 || cells[10-1] == 0)){
							if(cells[3-1] > max(cells[6-1], cells[12-1])){
								cells[3-1]--;
								borrow[i-1][3-1]++;  // borrow from BS3	
							}
							else if(cells[6-1] > max(cells[3-1], cells[12-1])){
								cells[6-1]--;
								borrow[i-1][6-1]++;  // borrow from BS6
							}
							else{
								cells[12-1]--;
								borrow[i-1][12-1]++;  // borrow from BS12
							}
						}
						else if(cells[4-1] > 0 && cells[10-1] > 0){
							if(cells[4-1] > cells[10-1]){
								cells[4-1]--;
								borrow[i-1][4-1]++;  // borrow from BS4
							}
							else{
								cells[10-1]--;
								borrow[i-1][10-1]++;  // borrow from BS10	
							}
						}
						else
							blockNum++;
						break;
					}
					case 6:{
						if(cells[4-1] > 0 && cells[4-1] > cells[5-1]){
							cells[4-1]--;
							borrow[i-1][4-1]++;  // borrow from BS4
						}
						else if(cells[5-1] > 0){
							cells[5-1]--;
							borrow[i-1][5-1]++;  // borrow from BS5
						}
						else
							blockNum++;
						break;
					}
					case 7:{
						if(cells[9-1] > 0 && cells[9-1] > cells[8-1]){
							cells[9-1]--;
							borrow[i-1][9-1]++;  // borrow from BS9
						}
						else if(cells[8-1] > 0){
							cells[8-1]--;
							borrow[i-1][8-1]++;  // borrow from BS8
						}
						else
							blockNum++;
						break;
					}
					case 8:{
						if(cells[1-1] > 0 && cells[4-1] > 0 && cells[7-1] > 0 && (maximum(cells[1-1], cells[4-1], cells[7-1]) >= max(cells[3-1], cells[9-1]) || cells[3-1] == 0 || cells[9-1] == 0)){
							if(cells[1-1] > max(cells[4-1], cells[7-1])){
								cells[1-1]--;
								borrow[i-1][1-1]++;  // borrow from BS1	
							}
							else if(cells[4-1] > max(cells[1-1], cells[7-1])){
								cells[4-1]--;
								borrow[i-1][4-1]++;  // borrow from BS4
							}
							else{
								cells[7-1]--;
								borrow[i-1][7-1]++;  // borrow from BS7
							}
						}
						else if(cells[3-1] > 0 && cells[9-1] > 0){
							if(cells[3-1] > cells[9-1]){
								cells[3-1]--;
								borrow[i-1][3-1]++;  // borrow from BS3
							}
							else{
								cells[9-1]--;
								borrow[i-1][9-1]++;  // borrow from BS9	
							}
						}
						else
							blockNum++;
						break;
					}
					case 9:{
						if(cells[7-1] > 0 && cells[4-1] > 0 && cells[7-1] > cells[8-1] && cells[4-1] > cells[8-1]){
							if(cells[7-1] > cells[4-1]){
								cells[7-1]--;
								borrow[i-1][7-1]++;  // borrow from BS7	
							}
							else{
								cells[4-1]--;
								borrow[i-1][4-1]++;  // borrow from BS4
							}
						}
						else if(cells[8-1] > 0){
							cells[8-1]--;
							borrow[i-1][8-1]++;  // borrow from BS8
						}
						else
							blockNum++;
						break;
					}
					case 10:{
						if(cells[2-1] > 0 && cells[5-1] > 0 && cells[11-1] > 0 && (maximum(cells[2-1], cells[5-1], cells[11-1]) >= max(cells[3-1], cells[12-1]) || cells[3-1] == 0 || cells[12-1] == 0)){
							if(cells[2-1] > max(cells[5-1], cells[11-1])){
								cells[2-1]--;
								borrow[i-1][2-1]++;  // borrow from BS2
							}
							else if(cells[5-1] > max(cells[2-1], cells[11-1])){
								cells[5-1]--;
								borrow[i-1][5-1]++;  // borrow from BS5
							}
							else{
								cells[11-1]--;
								borrow[i-1][11-1]++;  // borrow from BS11
							}
						}
						else if(cells[3-1] > 0 && cells[12-1] > 0){
							if(cells[3-1] > cells[12-1]){
								cells[3-1]--;
								borrow[i-1][3-1]++;  // borrow from BS3
							}
							else{
								cells[12-1]--;
								borrow[i-1][12-1]++;  // borrow from BS12	
							}
						}
						else
							blockNum++;
						break;
					}
					case 11:{
						if(cells[10-1] > 0 && cells[10-1] > cells[12-1]){
							cells[10-1]--;
							borrow[i-1][10-1]++;  // borrow from BS10
						}
						else if(cells[12-1] > 0){
							cells[12-1]--;
							borrow[i-1][12-1]++;  // borrow from BS12
						}
						else
							blockNum++;
						break;
					}
					case 12:{
						if(cells[5-1] > 0 && cells[11-1] > 0 && cells[5-1] > cells[10-1] && cells[11-1] > cells[10-1]){
							if(cells[5-1] > cells[11-1]){
								cells[5-1]--;
								borrow[i-1][5-1]++;  // borrow from BS5	
							}
							else{
								cells[11-1]--;
								borrow[i-1][11-1]++;  // borrow from BS11	
							}
						}
						else if(cells[10-1] > 0){
							cells[10-1]--;
							borrow[i-1][10-1]++;  // borrow from BS10
						}
						else
							blockNum++;
						break;
					}
				}
			}
		}
		if(in != 0){
			blockProb += (double)blockNum/in;
			addNum++;
		}
	}
	cout << "Question 2 : blocking probability = " << blockProb/addNum << endl;
	
	system("pause");
	return 0;
} 
