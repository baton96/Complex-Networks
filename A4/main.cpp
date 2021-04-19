#include <algorithm>
#include <iostream>
#include <iterator>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <tuple>

using namespace std;

int n_rep = 1; // number of repetitions of the simulation
float p = 0.2; // initial fraction of infected nodes
int t_max = 1000; // maximum number of time steps of each simulation
int t_trans = 900; // number of steps of the transitory
float a = 0.5;

float random(){ // random flot in range
	return float(rand())/float((RAND_MAX));
}

int main(int argc, char** argv){
	vector<tuple<int, int>> edges;
	int vcount = 0;

	string name = argv[1];
	ifstream input(name);
	string line;
	int i, j;
	float x;
	while (getline(input, line)){
		if(line=="*Edges ") break;
	}
	while (getline(input, line)){
		input >> i >> j >> x;
		int i2 = i-1;
		int j2 = j-1;
		tuple<int, int> edge = {i2, j2};
		edges.push_back(edge);
		if(j2>vcount) vcount = j2;
	}

	name = name.substr(0, name.find(".net"));
	ofstream output(name + "_" + to_string(a) + ".txt");

	bool infected_original[vcount];
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

	for(float b=0;b<=1.01;b+=0.1){
		float total_avg_r = 0;
		for(int n=0;n<n_rep;n++){
			float total_avg_t = 0;

			bool *infected_old = new bool[vcount];
			bool *infected_new = new bool[vcount];
			memcpy(infected_old, infected_original, sizeof(bool) * vcount);
			memcpy(infected_new, infected_original, sizeof(bool) * vcount);

			for(int t=0;t<t_max;t++){
				for(int i=0;i<vcount;i++){
					if(infected_new[i]){
						if(random()<a){
							infected_new[i] = false;
						}
					}
					else{
						vector<int> v_neighbors = neighbors[i];
						for(auto neighbor = v_neighbors.begin(); neighbor!=v_neighbors.end(); neighbor++){
							if(infected_old[*neighbor] && random()<b){
								infected_new[i] = true;
								break;
							}
						}
					}
				}

				if(t >= t_trans){
					float sum_infected = 0;
					for(int i=0;i<vcount;i++){
						if(infected_new[i]){
							sum_infected += 1;
						}
					}
					total_avg_t += sum_infected/vcount;
				}
				swap(infected_old, infected_new);
			}
			total_avg_r += total_avg_t/(t_max-t_trans);
		}
		cout << b << " " << total_avg_r/n_rep << endl;
		output << b << " " << total_avg_r/n_rep << endl;
	}	
	output.close();	

	return 0;
}