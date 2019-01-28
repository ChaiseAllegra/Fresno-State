#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream> 
#include <sstream>
#include <tr1/unordered_map>

using namespace std::tr1;
using namespace std;

int main(int argc, const char **argv)
{
	string s1,l1,l2,path1 = argv[1];
	fstream fin;
	ofstream outputFile;
	outputFile.open("output.txt");
	unordered_map<string, vector<string> > Dmap;
	fin.open(path1.c_str());
	getline(fin,l1);
	while(!fin.eof())
	{
		getline(fin,l2);
		if(Dmap.find(l2) != Dmap.end())
		Dmap[l2].push_back(l1);
		else
		{Dmap[l2];
		 Dmap[l2].push_back(l1);
		}
		getline(fin,l1);
	}
	for(unordered_map<string, vector<string> >::const_iterator l_it=Dmap.begin();l_it!=Dmap.end();++l_it)
	{
		if(l_it->second.size()>1)
		{
			for(int t=0; t<l_it->second.size();t++)
			{
				cout<<l_it->second[t]<<" ";
				outputFile << l_it->second[t]<< " ";
			}
			cout<<endl;
			outputFile<<endl;
			cout<<l_it->first<<endl;
			outputFile << l_it->first<< endl;
		}
		else
		{
		cout<<l_it->second[0]<<endl;
		outputFile << l_it->second[0]<< endl;
		cout<<l_it->first<<endl;
		outputFile << l_it->first<< endl;
		}
	}
	outputFile.close();
	fin.close();
    return 0;
 }
 /*670485961
	
 */