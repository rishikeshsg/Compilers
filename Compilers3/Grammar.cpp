#define TRUE 1
#define FALSE 0
#include<iostream>
#include<map>
#include<vector>
#include<string>
#include<fstream>
#include<algorithm>
using namespace std;

map <string, vector<string> >rules;
vector<string> nonTerminals;
int letter = 16;

void removeIndirectLeftRecursion();
void removeDirectLeftRecursion();

int main()
{
	ifstream infile;
	string rule;
	
	string left, right;

	vector<string>::iterator it;

	infile.open("Input.txt", ifstream::in);
	
	while(getline(infile, rule))
	{
		cout << rule << endl;
		int i;
		i = 0;
		while(rule[i] != ':')
		{
			left = left + rule[i];
			i++;
		}
		i++;
		while(i != rule.length())
		{
			right = right + rule[i];
			i++;
		}		
		cout << left << endl;
		cout << right << endl;				
		
		rules[left].push_back(right);

		it = find(nonTerminals.begin(), nonTerminals.end(), left);
		if(it == nonTerminals.end())
			nonTerminals.push_back(left);

		left="";
		right="";
		
	}
	infile.close();

	for(int i = 0; i < nonTerminals.size(); i++)
	{
		for(int j = 0; j < rules[nonTerminals[i]].size(); j++)
		{
			cout << nonTerminals[i] << "\t" << rules[nonTerminals[i]][j] << endl ; 
		}	
	}
	cout << nonTerminals.size() << endl; 
	cout << rules.size() << endl;
	cout << "---------------------------------" << endl ;

	removeIndirectLeftRecursion();

	for(int i = 0; i < nonTerminals.size(); i++)
	{
		for(int j = 0; j < rules[nonTerminals[i]].size(); j++)
		{
			cout << nonTerminals[i] << "\t" << rules[nonTerminals[i]][j] << endl ; 
		}	
	}
	cout << "---------------------------------" << endl ;


	removeDirectLeftRecursion();


	for(int i = 0; i < nonTerminals.size(); i++)
	{
		for(int j = 0; j < rules[nonTerminals[i]].size(); j++)
		{
			cout << nonTerminals[i] << "\t" << rules[nonTerminals[i]][j] << endl ; 
		}	
	}
	return 0;
}

void removeIndirectLeftRecursion()
{
	int pos;

	string str, temp, newrule;
	vector<string>::iterator it;

	for(int i = 0; i < nonTerminals.size(); i++)
	{
		for(int j = 0; j < i; j++)
		{
			for(int k = 0; k < rules[nonTerminals[i]].size(); k++)
			{
				pos = rules[nonTerminals[i]][k].find(nonTerminals[j]);
				if(pos == rules[nonTerminals[i]][k].npos)
					;
				else if(pos == 0)
				{
					str = rules[nonTerminals[i]][k];
					temp = str.substr(pos + 2);

					it = find(rules[nonTerminals[i]].begin(), rules[nonTerminals[i]].end(), str);
					rules[nonTerminals[i]].erase(it);
					
					for(int l = 0; l < rules[nonTerminals[j]].size(); l++)
					{
						newrule = rules[nonTerminals[j]][l] + " " + temp;
						rules[nonTerminals[i]].push_back(newrule); 
					}
				}
			}				
		}
	}
}

void removeDirectLeftRecursion()
{
	string s;
	s = letter;
	//cout << s;

	int i, j, k;
	int pos;
		
	string temp, str;

	vector<string>::iterator it;
	vector<string> x, y, z;
	bool found = FALSE;

	for(i = 0; i < nonTerminals.size(); i++)
	{
		s = letter;	
		//cout << "Non Terminal " << nonTerminals[i] << endl;	

		for(j = 0; j < rules[nonTerminals[i]].size(); j++)
		{
			//cout << "RHS " << rules[nonTerminals[i]][j] << endl;
			pos = rules[nonTerminals[i]][j].find(nonTerminals[i]);

			if(pos == rules[nonTerminals[i]][j].npos)
				cout << "Not Found" << endl;
			else if(pos == 0)
			{
				found = TRUE;
				cout << "Mil Gaya " << found << endl;
				temp = rules[nonTerminals[i]][j];		//Rule to be removed
				cout << temp << endl;
				str = temp.substr(pos + 2);
				x.push_back(str);
				y.push_back(temp);
				letter++;
			}
		}

		if(found == TRUE)
		{	
			for(k = 0; k < y.size(); k++)
			{
				it = find(rules[nonTerminals[i]].begin(), rules[nonTerminals[i]].end(), y[k]);
				rules[nonTerminals[i]].erase(it);
			}

			for(k = 0; k < rules[nonTerminals[i]].size(); k++)
			{
				rules[nonTerminals[i]][k] = rules[nonTerminals[i]][k] + " " + s;
			}
			for(k = 0; k < x.size(); k++)
			{
				rules[s].push_back(x[k] + " " + s);
			}
			rules[s].push_back("#");
		}
		x.clear();
		y.clear();
		found = FALSE;
	}
	for(int t = 16; t < letter; t++)
	{
		s = t;
		nonTerminals.push_back(s);		
	}
}
