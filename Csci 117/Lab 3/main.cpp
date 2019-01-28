#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream> 
#include <math.h>
#include "symbol_table.h"
#include <sstream>
#include <stdio.h>
#include <ctype.h>
using namespace std;

void Declarations();
void Declaration(string word);
void Statements();
void Statement(string word);

int Exp(), Term(), Exp2(int), Term2(int), Fact(), Fact2(int), Num();
string prog,s1;		// String for reading 1-line input program
int indexx = 0;		// Global index for program string
ifstream test_file;
const int Tsize = 50;
symbol_table stable[Tsize];
int ind = 0;
fstream fin;

int main(int argc, const char **argv)
{
	string path1 = argv[1];
	fin.open(path1.c_str());
	fin >> s1;

	if (s1 == "program")
	{
		Declarations();
		Statements();
	}
	else
	{
		cout << "Lexical Error Program needs to begin with Program" << endl;
		exit(1);
	}
	int ind2=0;
	fin.close();
	return 0;
}
void Declarations()
{
		string word;
		fin>>word;
	getline(fin,prog);
	if (word == "begin")
		return;
	else if (word == "int" || word == "double")
		Declaration(prog);
	else
		cout << "Lexical Error has to be begin or a definition of double/Int" << endl;
	Declarations();
}

void Declaration(string word)
{

	int temp1 = 0;
	char pop;
	while ((prog[temp1] != ';') && (temp1<prog.size()))
	{
		while ((prog[temp1] != ',') && (prog[temp1] != ';'))
		{
			if(prog[temp1]!=' ')
				pop = prog[temp1];
			temp1++;
		}
		stable[ind].type = word;
		if (!isalpha(pop))
		{
			cout << "Lexical Error id has to be a letter" << endl;
			break;
		}
		stable[ind].id = pop;
		ind++;
		temp1++;
	}
}

void Statements()
{
	string word;
	fin>>word;

	if (word == "end")
		exit(1);
	else 
		Statement(word);
	
	Statements();
}
void print_st() {
	int parse = 0,ind2 = 0,ind1 = 0,pvalue = 0,parse2 = 0;//iterators
	char print_char;
	string tprog, vprog;

	getline(fin,prog);

	while (prog[parse] != ';'&& parse<prog.size())//Extracts the id
	{
		print_char = prog[parse];
		parse++;
	}
	if (!isalpha(print_char))//checks if the id is not a letter
	{
			tprog = prog;//sets a temp string to hold prog

			while (tprog[parse2] != ';'&& parse2<prog.size()) {//grabs the equation 
				vprog += tprog[parse2];//stores the equation we want to print
				parse2++;
			}
			prog = vprog;//set prog to the equation we need to print
			indexx=0;
			cout << Exp() << endl;//Prints the answer to the equation
			return;
	}

	while (ind2<Tsize)//Prints the table
	{
		if (stable[ind2].id == print_char)
			cout << print_char<<" "<<stable[ind2].value << endl;
		ind2++;
	}
	return;
}
void Assign_St(string word) {
	char thold=word[0];
	fin>>word;
	getline(fin,prog);
	string tmp_prog1, tmp_prog2;//tmp strings
	int parse = 0,parse0=1,ind1 = 0,ind2=0;//iteratos
	bool sflag = false;//used as a flag to see if there is a semi colon
	while (parse0 < prog.size())//this while loop reads to the end of the line and checks for a ';'
	{
		if (prog[parse0] == ';')
			sflag = true;//sets bool to true if there is a semi colon
		parse0++;
	}
	if (sflag == false) {
		cout << "Sytanx Error no semi colon" << endl;//there is no semi colon terminate and create an error
		return;
	}

	if (word == "=")//if word is an = the assignment statement has been written syntactically correct
	{
		indexx = 0;
		bool l_flag = false,r_flag=false;
		char atmp = prog[1];//grab the id 
		if (isalpha(atmp))//if the id is a letter 
		{
			int ind3 = 0, tval = 0, itmp = 0;
			while (ind3 < Tsize)
			{
				if (stable[ind3].id == prog[1])//find the id on the right side of the =
				{
					tval = stable[ind3].value;//store its value
					r_flag = true;
				}
				if (stable[ind3].id == thold) {//find the id on the left side of the -
					itmp = ind3;//store it's index
					l_flag = true;//we found the id we were looking for
				}
				ind3++;
			}
			if (l_flag == false) {
				cout << "Sematics Error undefined variable on left side of the = " << endl;
				return;
			}
			if (r_flag == false) {
				cout << "Sematics Error undefined variable on right side of the = " << endl;
				return;
			}
			stable[itmp].value = tval;//set the id on the left side of the = to the id on the right side of the id
		}
		else {
			indexx = 0;//reset the index before we calculate the answer
			int tmps = Exp();//calculate the equation
			bool flag = false;
			while (ind2 < Tsize)
			{
				if (stable[ind2].id == thold)//find the id that is on the left side of the =
				{
					stable[ind2].value = tmps;// set it equal to the equation on the right side of the =
					flag = true;//set flag to true to indicate we found the id
				}
				ind2++;
			}
			if (flag == false)//if flag==false we did not find the id so we have a semantics error
				cout << "Semantics Error undefined variable" << endl;
		}
	}
	else
		cout << "Syntax Error need only on id on left side of = " << endl;// the assignment statement was not syntactically correct
	return;
}
void Statement(string word)
{
	if (word == "print")
	{
	
		print_st();
	}
	else
		Assign_St(word);
}
int Exp()
{ return Exp2(Term()); 
}

int Term()
{ 
	prog=prog;
	return Term2(Fact()); 
}

int Exp2(int inp)
{   
  int result = inp;
  if (indexx < prog.length()) //if not the end of program string
  { char a = prog.at(indexx++); //get one chr from program string
    while (a==' ' && (indexx < prog.length())) //if space, skip and read until a char
      a = prog.at(indexx++);
    if (a == '+')
      result = Exp2(result + Term());  //handles T+T, where T represents consecutive * or /
    else if (a == '-')                 //analogous for - operation
      result = Exp2(result - Term());
  }
  return result;
}

int Term2(int inp)
{   
  int result = inp;
  if (indexx < prog.length())   //if not the end of program string
  { char a = prog.at(indexx++); //get one chr from program string
    while (a==' ' && (indexx < prog.length())) //if space, skip and read until a char
      a = prog.at(indexx++);
    if (a == '*')
      result = Term2(result * Fact()); //handles consecutive * operators
    else if (a == '/')
      result = Term2(result / Fact()); //handles consecutive / operators
    else if (a == '+' || a == '-' || a == ')') //if + or -, get back one position
      indexx--;
  }
  return result;
}

int Fact()
{ return Fact2(Num());
}

int Fact2(int inp)
{ 
  int result = inp;
  if (indexx < prog.length())   //if not the end of program string
  { char a = prog.at(indexx++); //get one chr from program string
    while (a==' ' && (indexx < prog.length())) //if space, skip and read until a char
      a = prog.at(indexx++);
    if (a == '^')
      result = Fact2(pow(result, Fact()));
    else //this includes ')'
      indexx--; 
  }
  return result;
}

int Num()
{ 
  if (indexx < prog.length())
  { char a = prog.at(indexx++);
    while (a==' ' && (indexx < prog.length())) //if space, skip and read until a char
      a = prog.at(indexx++);
    if (a == '(')
      return Exp();
    return atoi(&a);  //converts a char to a numeric number and return
  }
}
