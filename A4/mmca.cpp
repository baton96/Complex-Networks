#include <algorithm>
#include <iostream>
#include <iterator>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <tuple>

using namespace std;

int n_rep = 10; // number of repetitions of the simulation
float p = 0.2; // initial fraction of infected nodes
int t_max = 100; // maximum number of time steps of each simulation
int t_trans = 90; // number of steps of the transitory
float a = 0.75;

int vcount = 0;
//vector<int> *neighbors;
bool *infected_original;

float random(){ // random flot in range [0, 1]
	return float(rand())/float((RAND_MAX));
}

float mmca(float a, float b){
	float avg = 0;
	bool *infected_old = new bool(vcount);
	bool *infected_new = new bool(vcount);
	for(int i=0;i<vcount;i++){
		infected_old[i] = infected_original[i];
		infected_new[i] = infected_original[i];
	}
	
	for(int t=0;t<1000;t++){
		for(int i=0;i<vcount;i++){
			float q = 1;
			vector<int> v_neighbors = neighbors[i];
			for(auto neighbor = v_neighbors.begin(); neighbor!=v_neighbors.end(); neighbor++){
				q = q * (1 - b * infected_old[*neighbor]);
			}
			infected_new[i] = (1-infected_old[i])*(1-q) + infected_old[i]*(1-a);
		}
		float new_avg = 0;
		for(int i=0;i<vcount;i++){
			if(infected_new[i]){
				new_avg += 1;
			}
		}
		new_avg = new_avg/vcount;
		if(new_avg==avg) return avg;
		swap(infected_old, infected_new);
	}
	return 0;
}

int main(int argc, char** argv){
	
	vector<tuple<int, int>> edges;

	string name = argv[1];
	ifstream input(name);
	string line;
	int i, j;
	while (getline(input, line)){
		if(line=="*Edges") break;
	}
	while (getline(input, line)){
		input >> i >> j;
		int i2 = i-1;
		int j2 = j-1;
		tuple<int, int> edge = {i2, j2};
		edges.push_back(edge);
		if(j2>vcount) vcount = j2;
	}

	name = name.substr(0, name.find(".net"));
	ofstream output(name + "_" + to_string(a) + ".mmca");
	
	infected_original = new bool(vcount);
	for(int i=0;i<vcount;i++){
		if(random() < p) infected_original[i] = true;
		else infected_original[i] = false;
	}
	
	vector<int> neighbors[vcount];
	for (auto i = edges.begin(); i != edges.end(); ++i){
		tuple<int, int> edge = *i;
		int source = get<0>(edge);
		int target = get<1>(edge);
		neighbors[source].push_back(target);
		neighbors[target].push_back(source);
	}
	
	for(int i=0;i<=50;i++){
		output << mmca(0.5, (float)i/50.0) << endl;
	}
	output.close();	
	return 0;
}