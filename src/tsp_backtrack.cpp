#include <bits/stdc++.h>
#include <chrono>

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

using namespace std;
using namespace std::chrono;


string get_current_dir() {
   char buff[FILENAME_MAX]; //create string buffer to hold path.
   GetCurrentDir( buff, FILENAME_MAX );
   string current_working_dir(buff);
   return current_working_dir;
}

class TSP
{
    private:

        struct City
        {
            int x;
            int y;
        };

        string iFile;
        string oFile;

        vector<int>odds;

        vector<int> *adjList;

    public:

        int n;

        vector<City> cities;

        int **graph;

        TSP(string in, string out);

        int get_distance(struct City c1, struct City c2);

		int cost_function(int start_index, int V);

		int tsp(int pos, int visited, vector<vector<int>>& state);

        int get_size(){return n;};

        void fillMatrix();

};


TSP::TSP(string in, string out)
{
	iFile = get_current_dir() + "/input/" + in;
	oFile = get_current_dir() + "/output/" + out;

	ifstream inStream;
	inStream.open(iFile.c_str(), ios::in);

	if(!inStream)
    {
		cerr << "Can't open input file " << iFile << endl;
		exit(1);
	}
	
	int c, x, y;
	int count = 0;
	
    while(!inStream.eof())
    {
		inStream >> c >> x >> y;
		count++;
		struct City newCity = {x,y};
		cities.push_back(newCity);
	}
	
    count--;
	cout << "cities created" << endl;
	inStream.close();

	n = count;
	graph = new int*[n];
	for(int i = 0; i < n; i++)
    {
		graph[i] = new int[n];
		for(int j = 0; j < n; j++)
        {
			graph[i][j] = 0;
		}
	}
	
}


int TSP::get_distance(struct TSP::City c1, struct TSP::City c2)
{
	int dx = pow((float)(c1.x - c2.x),2);
	int dy = pow((float)(c1.y - c2.y),2);
	return floor((float)(sqrt(dx+dy) + .5));
}


void TSP::fillMatrix()
{
	for(int i = 0; i < n; i++)
    {
		for(int j = 0; j < n; j++)
        {
			graph[i][j] = graph[j][i] = get_distance(cities[i],cities[j]);
			cout << graph[i][j] << " ";
		}
		cout << endl;
	}
}


int TSP:: cost_function(int start_index, int V)
{
	vector<vector<int>> state(n);
    for(auto& neighbors : state)
        neighbors = vector<int>((1 << n) - 1, INT_MAX);

    return tsp(0, 1, state);
}

int TSP:: tsp(int pos, int visited, vector<vector<int>>& state)
{
    if(visited == ((1 << n) - 1))
        return graph[pos][0]; // return to starting city

    if(state[pos][visited] != INT_MAX)
        return state[pos][visited];

    for(int i = 0; i < n; ++i)
    {
        // can't visit ourselves unless we're ending & skip if already visited
        if(i == pos || (visited & (1 << i)))
            continue;

        int distance = graph[pos][i] + tsp(i, visited | (1 << i), state);
        if(distance < state[pos][visited])
            state[pos][visited] = distance;
    }

    return state[pos][visited];
}

int main(int argc, char** argv) 
{
	if(argc < 2)
    {
		exit(-1);
	}

	string input, output;
	input = output = argv[1];
	output.append(".tour");

	TSP tsp(input, output);
	cout << "tsp created" << endl;
	int tsp_size = tsp.get_size();
	cout << "Size = " << tsp_size << endl;

	cout << "Fillmatrix started" << endl;
	tsp.fillMatrix();
	cout << "Filled Matrix" << endl;

	int best;

	auto start = high_resolution_clock::now(); 

	best = tsp.cost_function(0, tsp_size);

    auto stop = high_resolution_clock::now(); 

	cout << "best = " << best << endl;
    auto duration = duration_cast<microseconds>(stop - start); 
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl; 

	return 0;
}
  
  