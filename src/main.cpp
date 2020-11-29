
#include <bits/stdc++.h>
#include <chrono> // For the clock.

//To get the current working directory.

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
        void findOdds();

    public:

        int n;
        int **path_vals;
        int pathLength;

        vector<int> circuit;
        vector<City> cities;

        int **graph;

        vector<int>* adjlist;

        TSP(string in, string out);
        ~TSP();
        int get_distance(struct City c1, struct City c2);
        void perfectMatching();
        void euler_tour(int start, vector<int> &path);
        void make_hamiltonian(vector<int> &path, int &pathCost);
        void findMST();
        int getMinIndex(int key[], bool mst[]);
        void printResult();
        void printPath();
        void printEuler();
        void printAdjList();
        void printCities();
        int get_size(){return n;};
        void fillMatrix();
        int findBestPath(int start);

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

	path_vals = new int*[n];

	for(int i = 0; i < n; i++)
    {
		path_vals[i] = new int[n];
	}

	adjlist = new vector<int>[n];

	for(int i = 0; i < cities.size(); i++)
    {
		struct City cur = cities[i];
	}
}


TSP::~TSP(){
	for(int i = 0; i < n; i++){
		delete [] graph[i];
		delete [] path_vals[i];
	}
	delete [] path_vals;
	delete [] graph;
	delete [] adjlist;
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
		}
	}
}

//Finds the perfect matching for the given graph.

void TSP::findMST() 
{
    int *key = new int[n];
    bool *included = new bool[n];
    int *parent = new int[n];

    for (int i = 0; i < n; i++) 
    {
        key[i] = numeric_limits<int>::max();
        included[i] = false;
    }

    key[0] = 0;
    parent[0] = -1;

    for (int i = 0; i < n - 1; i++) 
    {
        int k = getMinIndex(key, included);
        included[k] = true;

        for (int j = 0; j < n; j++) 
        {
            if (graph[k][j] && included[j] == false && graph[k][j] < key[j]) 
            {
                parent[j] = k;
                key[j] = graph[k][j];
            }
        }

    }

    for (int i = 0; i < n; i++) 
    {
        int j = parent[i];

        if (j != -1) 
        {
            adjlist[i].push_back(j);
            adjlist[j].push_back(i);
        }
    }

    // cout << "MST\n";
    // for(int i = 0; i < n; i++)
    // {
    //     for(int j = 0; j < adjlist[i].size(); j++)
    //     {
    //         cout << adjlist[i][j] << " ";
    //     }
    //     cout << "\n";
    // }
}


int TSP::getMinIndex(int key[], bool mst[]) 
{
    int min = numeric_limits<int>::max();
    int min_index;

    for (int i = 0; i < n; i++) 
    {
        if (mst[i] == false && key[i] < min) 
        {
            min = key[i];
            min_index = i;
        }
    }

    return min_index;
}


void TSP::findOdds() 
{
    for (int i = 0; i < n; i++) 
    {
        if ((adjlist[i].size() % 2) != 0) 
        {
            odds.push_back(i);
        }
    }
}

//Finds the perfect matching for the odd degree vertices.

void TSP::perfectMatching() 
{
    int closest, length;
    vector<int>::iterator tmp, first;

    findOdds();

    while (!odds.empty()) 
    {
        first = odds.begin();
        vector<int>::iterator it = odds.begin() + 1;
        vector<int>::iterator end = odds.end();
        length = numeric_limits<int>::max();
        
        for (; it != end; ++it) 
        {
            if (graph[*first][*it] < length) 
            {
                length = graph[*first][*it];
                closest = *it;
                tmp = it;
            }
        } 

        adjlist[*first].push_back(closest);
        adjlist[closest].push_back(*first);
        odds.erase(tmp);
        odds.erase(first);

    }
}

//Euler tour on the multigraph created and getting the path for next step.

void TSP::euler_tour(int start_index, vector<int> &path)
{
	vector<int> *newList = new vector<int>[n];
	for(int i = 0; i < n; i++)
    {
		newList[i].resize(adjlist[i].size());
		newList[i] = adjlist[i];
	}

	stack<int> stack;
	int pos = start_index;

	while(!stack.empty() || newList[pos].size() > 0)
    {
		if(newList[pos].empty())
        {
			path.push_back(pos);
			pos = stack.top();
			stack.pop();
		}
		else
        {
			stack.push(pos);
			int neighbor = newList[pos].back();
			newList[pos].pop_back();
			
            for(int i = 0; i < newList[neighbor].size(); i++)
            {
				if(newList[neighbor][i] == pos)
                {
					newList[neighbor].erase(newList[neighbor].begin()+i);
				}
			}

			pos = neighbor;
		}
	}
	path.push_back(pos);

}

//Finding the shortcuts in path created from euler tour.

void TSP::make_hamiltonian(vector<int> &path, int &pathCost)
{
	bool *visited = new bool[n];
	
    for(int i = 0; i < n; i++)
    {
		visited[i] = 0;
	}
	
	pathCost = 0;

	int source_vertex = path.front();
	
    vector<int>::iterator cur = path.begin();
	vector<int>::iterator iter = path.begin()+1;
	
    visited[source_vertex] = 1;

	bool addMore = true;

	while(iter != path.end())
    {
		if(!visited[*iter])
        {
			pathCost += graph[*cur][*iter];
			cur = iter;
			visited[*cur] = 1;
			iter = cur + 1;
		}	
		else
        {
			iter = path.erase(iter);
		}
	}

    pathCost += graph[*cur][source_vertex];
}

//Utility function for euler and hamiltonian 

int TSP::findBestPath(int start)
{
	vector<int> path;

	euler_tour(start, path);

	int length;

	make_hamiltonian(path, length);

	return length;
}


void TSP::printResult()
{
    ofstream outputStream;
    outputStream.open(oFile.c_str(), ios::out);
    outputStream << pathLength << endl;
    
    for (vector<int>::iterator it = circuit.begin(); it != circuit.end(); ++it) 
    {
        outputStream << *it << endl;
    }

    outputStream.close();
};

void TSP::printPath()
{
    cout << endl;
    
    for (vector<int>::iterator it = circuit.begin(); it != circuit.end()-1; ++it) 
    {
        cout << *it << " --> " << *(it+1) << " ";
        cout << graph[*it][*(it+1)] << endl;
    }

    cout << *(circuit.end()-1) << " --> " << circuit.front() << " ";
    cout << graph[*(circuit.end()-1)][circuit.front()];
    cout << "\n\nLength of final circuit : " << pathLength << endl << endl;
};

void TSP::printEuler() 
{
    for (vector<int>::iterator it = circuit.begin(); it != circuit.end(); ++it)
        cout << *it << endl;
}

void TSP::printAdjList() 
{
    for (int i = 0; i < n; i++) 
    {
        cout << i << ": "; 
        
        for (int j = 0; j < (int)adjlist[i].size(); j++) 
        {
            cout << adjlist[i][j] << " "; 
        }

        cout << endl;
    }
};

void TSP::printCities()
{
    cout << endl;
    int i = 0;

    for (vector<City>::iterator it = cities.begin(); it != cities.end(); ++it)
        cout << i++ << ":  " << it->x << " " << it->y << endl;
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

	cout << "Fillmatrix started" << endl;
	tsp.fillMatrix();
	cout << "Filled Matrix" << endl;

	auto start = high_resolution_clock::now(); 

	tsp.findMST();
	cout << "MST created" << endl;

	tsp.perfectMatching();
	cout << "Matching completed" << endl;

	int best = INT_MAX;
	int bestIndex;
	
    for (long t = 0; t < tsp_size; t++) {
		int result = tsp.findBestPath(t);
        cout << result << endl;
		tsp.path_vals[t][0] = t; 
		tsp.path_vals[t][1] = result; 

		if (tsp.path_vals[t][1] < best) {
			bestIndex = tsp.path_vals[t][0];
			best = tsp.path_vals[t][1];
		}
	}

    auto stop = high_resolution_clock::now(); 

	tsp.euler_tour(bestIndex,tsp.circuit);
	tsp.make_hamiltonian(tsp.circuit,tsp.pathLength);

	cout << "Final length: " << tsp.pathLength << endl;
    cout << "Path : \n";

	tsp.printPath();
	tsp.printResult();

    auto duration = duration_cast<microseconds>(stop - start); 
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl; 
}
