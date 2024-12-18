#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;
//using Clock = std::chrono::system_clock;

int z = 0;
int l = 0;
float pt = 0;
vector<vector<int> > N;
vector<vector<int> > T;
vector<vector<int> > tabu;
auto start = chrono::high_resolution_clock::now();

void read_input(string inputfile)
{
    ifstream infile;
    infile.open(inputfile);
    if(!infile.is_open())
    {
        cout << "Error opening input file\n";
    }
    infile >> pt >> z >> l;
    //pt = pt/2;
    pt -= 0.01;

    for(int i = 0; i < z; i++)
    {
        vector<int> temp;
        for(int j = 0; j < z; j++)
        {
            int n;
            infile >> n;
            temp.push_back(n);
        }
        N.push_back(temp);
    }

    for(int i = 0; i < l; i++)
    {
        vector<int> temp;
        for(int j = 0; j < l; j++)
        {
            int t;
            infile >> t;
            temp.push_back(t);
        }
        T.push_back(temp);
    }
    infile.close();
}

long objective(vector<int> arr)
{
    long obj {0};
    //cout << "In objective function\n";

    for(int i = 0; i < arr.size(); i++)
    {
        //cout << "In loop 1\n";
        for(int j = 0; j < arr.size(); j++)
        {
            //cout << "In loop 2\n";
            if(arr[i] && arr[j])
            {
                obj += N[arr[i]-1][arr[j]-1]*T[i][j];
            }
            //obj += N[arr[i]-1][arr[j]-1]*T[i][j];
        }
    }
    return obj;
}


vector<int> neighbor_gen(vector<int> v)
{
//    vector<pair<long, vector<int>>> neighbors;
    vector<int> neighbor = v;
    for(int i = 0; i < v.size(); i++)
    {
        for(int j = i+1; j < v.size(); j++)
        {
            auto t = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count();
            if(t >= pt*60*1000000)
    	    {
    		return neighbor;
    	    }
            vector<int> temp = v;
            if(temp[i]!=temp[j])
            {
                swap(temp[i], temp[j]);
//                neighbors.push_back(make_pair(objective(temp), temp));
                if(objective(temp) <= objective(neighbor))
                {
                    neighbor = temp;
                }
            }
        }
    }
    return neighbor;
}

vector<int> hill_climb(vector<int> v)
{
    auto t = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count();
    if(t >= pt*60*1000000)
    {
    	return v;
    }

    if(tabu.size() >= 3)
    {
        tabu.erase(tabu.begin());
        tabu.push_back(v);
    }
    else
    {
        tabu.push_back(v);
    }

    long v_cost = objective(v);

    vector<int> neighbor = neighbor_gen(v);
    long n_cost = objective(neighbor);

    if(neighbor == v)
    {
        return v;
    }

    if(n_cost < v_cost)
    {
//        cout << "hill climb(neighbor)\n\n";
        return hill_climb(neighbor);
    }
    if(n_cost == v_cost)
    {
        if(find(tabu.begin(), tabu.end(), neighbor) == tabu.end())
        {
//            cout << "hill climb(neighbor)\n\n";
            return hill_climb(neighbor);
        }
        else
        {
//            cout << "returning v\n\n";
            return v;
        }
    }
    return v;
}

vector<int> sample_gen()
{
    vector<int> v(l, 0);
    int zone = 1;
    int rd {0};
    while(zone <= z)
    {
        rd = rand() % l;
        //cout << rd << endl;
        if(v[rd] == 0)
        {
            v[rd] = zone;
            zone++;
        }
    }
    return v;
}

vector<int> hcrr()
{

    vector<int> opt = sample_gen();
    vector<int> temp;
    long curr_opt = objective(opt);
    long temp_opt;
    int i = 0;

    while(true)
    {
        auto t = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count();
        if(t >= pt*60*1000000)
        {
        	break;
        }

        tabu.clear();
        temp = hill_climb(sample_gen());
        temp_opt = objective(temp);

        if(curr_opt > temp_opt)
        {
//            cout << "Updating opt\n";
            opt = temp;
            curr_opt = temp_opt;
        }
        i++;
    }
//    cout << "No. of hill climbs:" << i << endl;

    return opt;

}

void print_file(string output_file, int best[])
{
    ofstream ofile;
    ofile.open(output_file);

    for(int i = 1; i < z+1; i++)
    {
        ofile << best[i] << " " ;
    }
    ofile << endl;

    ofile.close();

}



int main(int argc, char *argv[])
{
    start = chrono::high_resolution_clock::now();
    //cout << start << endl;
    srand(time(0));

    if(argc != 3)
    {
        cout<<"Enter 2 arguments.\n";
    }

    read_input(argv[1]);


    vector<int> opt = hcrr();

//    vector<pair<int, int>> best;
    int best[z+1];

    for(int i = 0; i < opt.size(); i++)
    {
        if(opt[i])
        {
//            best.push_back(make_pair(opt[i], i));
            best[opt[i]] = i+1;
        }
    }

     print_file(argv[2], best);

//    sort(best.begin(), best.end());

//    cout << "Optimal arrangement : ";
//    for(int i = 1; i < z+1; i++)
//    {
//        cout << best[i] << " ";
//    }
//    cout << endl;
//    cout << "Total time : " << objective(opt) << endl;

//    auto end = chrono::high_resolution_clock::now();
//
//    chrono::duration<double, micro> duration = end - start;
//	cout << "Execution time: " << duration.count()/1000000 << " seconds" << endl;

    return 0;
}
