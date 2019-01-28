#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <sstream>
using namespace std;

class Col{
public: 
		int mark=0;
		int key=0;
		int next=-1;
		string Lval="none";
		//int column_num=0;
};
//Nov.02(Th)–Lab2
Col array[10];
bool flag1=false,flag2=false,first_l2=true,first_l1=true;//define all the bools we use
int prevL1=0,prevL2=0,last_L1=0,last_L2=0,free_head=11;//define all the integers we use
int L1,L2,curr=0,z=0,d=0;//curr is conceptually equal to free
		
void insert_L1(string x){
		if(first_l1==true){//if it is the first insert 
						L1=curr;//set the L1 head to the current spot
						prevL1=curr;//set the previous L1 insert pointer to the current spot
						first_l1=false;//it is no longer our first input
					}
					else{
							array[prevL1].next=curr;//if it is not our first L1 insert set the previous L1 insert next to our current position in the array 
					}
						//cout<<"currr"<<curr<<endl;
						z=x.at(10)-'0';//convert the string to a char to an int
						array[curr].key=z;// set the key 
						array[curr].Lval="L1";//set the L1 identifier
						prevL1=curr;//set the previous L1 array slot to the previous L1 insert
						//prevL2=curr;
						curr++;//update the incrementer
						free_head=curr+1;//Update the free list head 
						last_L1=curr;//Update the last input in the L1 to the current spot in the arary 
}
void insert_L2(string x){//Same as insert L1 but just for L2 
					if(first_l2==true){
						L2=curr;
						prevL2=curr;
						first_l2=false;
					}
					else{
							array[prevL2].next=curr;//+1;
					}
						z=x.at(10)-'0';
						array[curr].key=z;
						array[curr].Lval="L2";
						prevL2=curr;
						//prevL1=curr;
						curr++;
						free_head=curr+1;
						last_L2=curr;
}
int main()
{
	stringstream ss;
	L1=-1;//initialize the head pointers of L1 and L2
	L2=-1;
	string x;
	for(int i=0; i<10;i++){
		if(i+1>=10)
		{
			array[i].next=-1;//Set the last spot to point to the first spot in memeory 
		}
		else{
			array[i].next=i+1;//initialize the next pointers of the array
		}
	}
	while(x!="end")
	{
		cout<<"type in you menu command with no spaces "<<endl;
		cin>>x;//user input
		if(x.at(0)=='i'&&x.at(5)=='t')//if the user typed insert
		{
			//cout<<"x "<<x<<" "<<x.at(8)<<endl;
			if(x.at(8)=='1')//if we are inserting into L1
				{
					//cout<<"inserting l1"<<endl;
					insert_L1(x);
					array[last_L1].next=-1;//set the L2 inserted's next 
				}
		 	if(x.at(8)=='2')//if we are inserting into L2
			{
					insert_L2(x);
					array[last_L2].next=-1;
			}
		}
		if(x.at(0)=='d'&&x.at(5)=='e')//if the user wants to delete
		{
			//cout<<x<<endl;
			//cout<<"l1"<<L1<<"l2"<<L2<<endl;
			string find="te";//intitalize the string
			d=x.at(10)-'0';//grab the key we want to delete
			//cout<<"delete val "<<d<<endl;
			find.at(0)=x.at(7);//these lines define the string find wich will be equal to L1||L2
			find.at(1)=x.at(8);
			//cout<<"find "<<find<<endl;
			int count1=0,count2=0,inc1=L1,inc2=L2;//define the incrementers to start at the head of L1 and L2
			if(find=="L1")
			{	
				if(d==array[L1].key)//if the key is the head of the list
				{
							L1=array[L1].next;//Change the head of the list once we delete it 
				}
				else{//if the key is not the head of the list
									inc1=L1;//set the incrementer to the head of the L1 list
					while(array[inc1].next!=-1)//While we dont reach the end
					{
						if(array[array[inc1].next].key==d)//if we find the key in the current ones next
						{
							array[inc1].next=array[array[inc1].next].next;//set the current's next equal to the next's next
							break;
						}
						inc1=array[inc1].next;//set the incrementer to the current one's next 
						count1++;//increment
						
					}
				}

			}
			if(find=="L2")//exact same code as find L1 but for L2
			{	
				if(d==array[L2].key)
				{
							//cout<<"head "<<endl;
							L2=array[L2].next;
							array[inc1].next=-1;
				}
				else{
									inc2=L2;
					while(array[inc2].next!=-1)
					{
						if(array[array[inc2].next].key==d)
						{
							array[inc2].next=array[array[inc2].next].next;
							break;
						}
						inc2=array[inc2].next;
											
					}
				}
			}
		}
		if(x=="garbage-collect();"||x=="garbage-collect()")
		{//cout<<"you typed garbage-collect "<<endl;
			int q=L1,y=L2;//set variables to head L1 and Head L2
			if(q!=-1)//if the head L1 is not equal to -1
			{
				array[q].mark=1;//set the head mark to 1
			}
			if(y!=-1)//same as L1
			{
				array[y].mark=1;
			}
			while(array[q].next!=-1)//as long as you havent reached the end of the list for L1
			{
				q=array[q].next;//tmp variable set to the next array index
				array[q].mark=1;//set the next ones mark to 1
			}
			while(array[y].next!=-1)//Same as L1 
			{
				y=array[y].next;
				array[y].mark=1;
			}
			for(int i=0;i<10;i++)//Set the free_head pointer to point to the first avaliable free spot
			{
				if(array[i].mark==0)
				{
					free_head=i;
					break;
				}
			}
			int tmp=free_head+1;//set the tmp
			int next_tmp =free_head;
			while(tmp<10)//find all the marks that are zero or unreachable
			{
				if(array[tmp].mark==0)
				{
					array[next_tmp].key=0;//set their key to zero
					array[next_tmp].next=tmp;//set the next to the current spot
					next_tmp=tmp;//set next to the current
				}
				tmp++;
			}
		}
		if(x=="print_memory;"||x=="print_memory")//print the memory 
		{
			for(int i=0;i<10;i++)
			{
				if(array[i].next==-1)
				{
					cout<<"Key "<<array[i].key<<endl;
					cout<<"Next "<<array[i].next<<endl;
					cout<<"Mark "<<array[i].mark<<endl;
					cout<<"      "<<endl;
				}
				else{
				cout<<"Key "<<array[i].key<<endl;
				cout<<"Next "<<array[i].next+1<<endl;
				cout<<"Mark "<<array[i].mark<<endl;
				cout<<"      "<<endl;
				}
			}
			//cout<<"L1 "<<L1+1<<"L2 "<<L2+1<<"Free"<<curr+1<<" free head"<<free_head<<endl;
			cout<<"L1 "<<L1+1<<"L2 "<<L2+1<<" free head"<<free_head<<endl;//add ones to the array slot so the print out is like the example table
		}
	}
	return 0;
}
/*
insert (L1, 3);
insert(L1, 1);
insert (L2, 4);
 insert  (L1, 5);
 insert (L2, 2);
 insert (L2, 9);
 insert (L2, 8);
 insert (L1, 4);


 delete (L1, 4);   print_memory;
 delete (L2, 8);   print_memory;
 delete (L1, 1);   print_memory;
 delete (L2, 4);   print_memory;
 delete (L1, 5);   print_memory;

 garbage-collect();
*/