#include <string>
#include <cstdlib> //for atoi()
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <math.h>
using namespace std;

int Exp(), Term(), Exp2(int), Term2(int), Fact();
string prog, s1; //string for reading 1-line input program
int indexx = 0; //global index for program string
ifstream fin;

int main(int argc, const char **argv)
{
	ifstream myfile;
	myfile.open("filetest.txt");
	if (!myfile.is_open()) {
		cout << " failed " << endl;
		return 0;
	}

	while (myfile)
	{
		getline(myfile,prog);
	}
	cout << "data: " << prog << endl;
	cout<< Exp() << endl;
	myfile.close();
	system("PAUSE");
	return 0;
}

int Exp()
{
	int tmp1 = Exp2(Term());
	return tmp1;
}

int Term()
{
	int tmp = Term2(Fact());
	return tmp;
}



int Exp2(int inp)
{
	int result = inp;
	if (indexx < prog.length())   //if not the end of program string
	{
		char a = prog.at(indexx++); //get one chr from program string
		while (a == ' ' && (indexx < prog.length()))
			a = prog.at(indexx++);

		if (a == '+')
			result = Exp2(result + Term());  //handles T+T
		else if (a == '-')
			result = Exp2(result - Term());  //handles T-T
	}
	return result;
}

int Term2(int inp)
{
	int result = inp;
	if (indexx < prog.length())   //if not the end of program string
	{
		char a = prog.at(indexx++); //get one chr from program string
		while (a == ' ' && (indexx < prog.length()))
			a = prog.at(indexx++);
			
		 if (a == '*') {
			result = Term2(result * Fact()); //handles consecutive * operators
		}
		 else if (a == ')') {//reached the end of the () decrement the index
			 indexx--;
		 }
		else if (a == '/')
		{
			result = Term2(result / Fact()); //handles consecutive / operators
		}
		else if (a == '+' || a == '-')     //if + or -, get back one position
			indexx--;
			//exp();
	}
	return result;
}
int Fact2(int inp) {
	int result = inp;
		if(prog.size()==indexx){//checks if you reached the end of the string
		return result;//returns the result
		}
		char a = prog.at(indexx++); //get one chr from program string
		while (a == ' ' && (indexx < prog.length()))
			a = prog.at(indexx++);
	if (a == '^')//checks if there is an exponenet function
	{
		result= Fact2(pow(result,Fact()));
	}
	else{
		indexx--;//no exp found decrement the index and return
		return result;
	}
}
int Num() {
	char a = prog.at(indexx++); //get one chr from program strin
	while (a == ' ' && (indexx < prog.length()))//increments a
		a = prog.at(indexx++);
	if (a == '(')
	{
		return Exp();//calls the rest of the code and treats the funciton in the () first before the rest of the program
	}
	else {
		return atoi(&a);//returns the number calculated within the ()
	}
}
int Fact()
{
	return Fact2(Num());
}








