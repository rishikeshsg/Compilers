#define TRUE 1
#define FALSE 0
#include<iostream>
#include<map>
#include<vector>
#include<string>
#include<fstream>
#include<algorithm>
#include<set>
#include<sstream>
using namespace std;

map<string, vector<string> >rules;
vector<string> nonTerminals;
set<string> terminals;
map<string, set<string> >firstSet;
map<string, set<string> >followSet;
int letter = 16;

void removeIndirectLeftRecursion();
void removeDirectLeftRecursion();
void leftFactoring();
void findFirstSets();
bool produceEpsilon(string);
bool isNonTerminal(string);
void findFollowSets();

vector<string> split(string, char, vector<string>&);

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
	cout << nonTerminals.size() << "---------------------------------" << endl ;


	removeDirectLeftRecursion();


	for(int i = 0; i < nonTerminals.size(); i++)
	{
		for(int j = 0; j < rules[nonTerminals[i]].size(); j++)
		{
			cout << nonTerminals[i] << "\t" << rules[nonTerminals[i]][j] << endl ; 
		}	
	}
	cout << "Gone " << endl;
	cout << "---------------------------------" << endl ;

	//leftFactoring();
	findFirstSets();
	findFollowSets();
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
	int pos, count = 0;
		
	string temp, str;

	vector<string>::iterator it;
	vector<string> x, y, z;
	bool found = FALSE;

	for(i = 0; i < nonTerminals.size(); i++)
	{
		s = letter;	
		//cout << "Non Terminal " << nonTerminals[i] << endl;	
		count = 0;

		for(j = 0; j < rules[nonTerminals[i]].size(); j++)
		{
			//cout << "RHS " << rules[nonTerminals[i]][j] << endl;
			pos = rules[nonTerminals[i]][j].find(nonTerminals[i]);

			if(pos == rules[nonTerminals[i]][j].npos)
				cout << "Not Found" << endl;
			else if(pos == 0)
			{
				found = TRUE;
				//cout << "Mil Gaya " << found << endl;
				temp = rules[nonTerminals[i]][j];		//Rule to be removed
				cout << temp << endl;
				str = temp.substr(pos + 2);
				x.push_back(str);
				y.push_back(temp);
				if(count == 0)
					letter++;
				count++;
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

	//cout << "Aditya " << letter << endl;
	for(int t = 16; t < letter; t++)
	{
		s = t;
		nonTerminals.push_back(s);		
	}
}

void leftFactoring()
{	
	int i, j, k, l;
	int count;
	vector<vector<int> >lengths;
	vector<int> x;

	//cout << "Reached";
	for(i = 0; i < nonTerminals.size(); i++)
	{
		//cout << i << endl;
		for(j = 0; j < rules[nonTerminals[i]].size(); j++)
				x.push_back(0);

		for(j = 0; j < rules[nonTerminals[i]].size(); j++)
			lengths.push_back(x);		

		for(j = 0; j < rules[nonTerminals[i]].size(); j++)
		{
			for(k = j + 1; k < rules[nonTerminals[i]].size(); k++)
			{
				if(rules[nonTerminals[i]][j].length() < rules[nonTerminals[i]][k].length())
				{
					for(l = 0; l < rules[nonTerminals[i]][j].length(); l++)
					{
						if(rules[nonTerminals[i]][j][l] == rules[nonTerminals[i]][k][l])
							count++;
					}
				}
				else
				{
					for(l = 0; l < rules[nonTerminals[i]][k].length(); l++)
					{
						if(rules[nonTerminals[i]][j][l] == rules[nonTerminals[i]][k][l])
							count++;
					}
				}				
				lengths[j][k] = count / 2;
				lengths[k][j] = count / 2;
				count = 0;			
			}
																												
		}
	}

	for(i = 0; i < lengths.size(); i++)
	{
		for(j = 0; j < lengths[i].size(); j++)
		{
			cout << lengths[i][j] << " ";
		}
		cout << endl;
	}	
}

void findFirstSets()
{
	int i, j, k;
	bool result;
	vector<string>::iterator pos;
	vector<string> t;
	set<string>::iterator it;
	map<string, set<string> >::iterator xy;

	for(i = 0; i < nonTerminals.size(); i++)
	{
		for(k = 0; k < rules[nonTerminals[i]].size(); k++)
		{
			split(rules[nonTerminals[i]][k], ' ', t);				
		
			for(j = 0; j < t.size(); j++)
			{
			//cout << "Split " << t[j] << endl;
				pos = find(nonTerminals.begin(), nonTerminals.end(), t[j]);
				if(pos == nonTerminals.end())
				{
					firstSet[t[j]].insert(t[j]);
					terminals.insert(t[j]);
				}
			}
		}
		result = produceEpsilon(nonTerminals[i]);
		if(result)
			firstSet[nonTerminals[i]].insert("#");
		cout << nonTerminals[i] << "S-->" << result << endl;
	
		t.clear();
	}

	t.clear();

	vector<string>::iterator iter, iter_v;

	//cout << "Before while()" << endl;

	for(xy = firstSet.begin(); xy != firstSet.end(); xy++)
	{	
		cout << xy->first << "--> ";
		for(it = xy->second.begin(); it != xy->second.end(); it++)
		{
			cout << *it << "\t";
		}
		cout << endl;
	}

	
	result = TRUE;
	
	while(result)
	{
		result = FALSE;

		//cout << "New Iteration" << endl;
		for(iter = nonTerminals.begin(); iter != nonTerminals.end(); iter++)
		{
			for(iter_v = rules[*iter].begin(); iter_v != rules[*iter].end(); iter_v++)
			{
				//cout << *iter << ":" << *iter_v << "Aditya" << endl;
				vector<string> temp;
				vector<string>::iterator it_temp;
				split(*iter_v, ' ', temp);
				for(it_temp = temp.begin(); it_temp != temp.end(); it_temp++)
				{
					//cout << "Anuj" << *it_temp << endl;
					if(isNonTerminal(*it_temp))
					{
						if(!produceEpsilon(*it_temp))
						{
							//cout << "Ghusa" << endl;
							set<string>::iterator it_s;
							if(firstSet.find(*it_temp) != firstSet.end())
							{						
								for(it_s = firstSet[*it_temp].begin(); it_s != firstSet[*it_temp].end(); it_s++)
								{
									if(firstSet[*iter].find(*it_s) == firstSet[*iter].end())
										result = TRUE;
									firstSet[*iter].insert(*it_s);
								}
								cout << "END" << endl;
							}
							break;
						}
						else
						{
							set<string>::iterator it_s;
							if(firstSet.find(*it_temp) != firstSet.end())
							{						
								for(it_s = firstSet[*it_temp].begin(); it_s != firstSet[*it_temp].end(); it_s++)
								{
									if(firstSet[*iter].find(*it_s) == firstSet[*iter].end())
										result = TRUE;
									firstSet[*iter].insert(*it_s);
								}
								cout << "END else" << endl;
							}
							
						}
						//cout << "gaya" << endl ;
					}
					else
					{
						if(*it_temp != "")
						{
							if(firstSet[*iter].find(*it_temp) == firstSet[*iter].end())
							{
								cout << "Insert " << *it_temp << " in first " << *iter << endl; 
								result = TRUE;
							}
							firstSet[*iter].insert(*it_temp);
							break;
						}
					}
				}				
			}
		}
		/*for(xy = firstSet.begin(); xy != firstSet.end(); xy++)
		{	
			cout << xy->first << "--> ";
			for(it = xy->second.begin(); it != xy->second.end(); it++)
			{
				cout << *it << "\t";
			}
			cout << endl;
		}*/
	
	}

	cout << "Terminals" << endl;
	for(it = terminals.begin(); it != terminals.end(); it++)
		cout << *it << endl;


	cout << "-------------------------------------" << endl;
	cout << "First Sets" << endl;

	for(xy = firstSet.begin(); xy != firstSet.end(); xy++)
	{	
		cout << xy->first << "--> ";
		for(it = xy->second.begin(); it != xy->second.end(); it++)
		{
			cout << *it << "\t";
		}
		cout << endl;
	}
}


vector<string> split(string s, char delim, vector<string> &elems)
{
        stringstream ss(s);
        string item;
        while(getline(ss, item, delim))
	{
                elems.push_back(item);
        }
        return elems;
}

bool produceEpsilon(string str)
{
	//cout << "Called-->" << str << endl;
	int i, j;
	vector<string> x;
	vector<string>::iterator it;
	bool yes;
	
	for(i = 0; i < rules[str].size(); i++)
	{
		//cout << "Rules " << rules[str][i] << endl;
		if(rules[str][i] == "#"){//cout << "dir #\n";
			return TRUE;	}	
	}

	cout << "Size" << rules[str].size();

	for(i = 0; i < rules[str].size(); i++)
	{
		split(rules[str][i], ' ', x);

		//cout << "Length " << rules[str][i] << " " << rules[str][i].length() << endl;
		int has;

		for(j = 0; j < x.size(); j++)
		{
			has = 0;
			//cout << "x[j] -->" << x[j] << endl;
		
			yes = isNonTerminal(x[j]);
			if(yes)
			{
				if(!produceEpsilon(x[j])){ cout << "not produce\n";
					return FALSE;}
				else
					has = 1;
			}
			else
				break;
		}
		x.clear();
		if(has)
			return TRUE;
		
	}	
	return FALSE;

	
}



bool isNonTerminal(string s)
{
	vector<string>::iterator it;
	for(it = nonTerminals.begin(); it != nonTerminals.end(); it++)
	{
		if(*it == s)
			return TRUE;
	}
	return FALSE;
}

void findFollowSets(){
	set<string>::iterator it;
	int i,j;
	vector<string>::iterator x;
	vector<string>::iterator itnt;

	followSet[nonTerminals[0]].insert("$");

	for(itnt = nonTerminals.begin(); itnt != nonTerminals.end(); itnt++)
	{
		for(x = rules[*itnt].begin(); x != rules[*itnt].end(); x++)
		{
			vector<string> temp;
			int k;
			split(*x, ' ', temp);
			for(k = 0; k < temp.size() - 1; k++)
			{
				set<string>::iterator it_set;
				for(it_set = firstSet[temp[k + 1]].begin(); it_set != firstSet[temp[k+1]].end(); it_set++)
				{
					if(*it_set != "#")
						followSet[temp[k]].insert(*it_set);
				}
			}
		}
	}	


	map<string ,set<string> >::iterator xy;
	set<string>::iterator z;

	cout << endl;
	for(xy = followSet.begin(); xy != followSet.end(); xy++)
	{
		cout << xy->first << "--> " ;
		for(z = xy->second.begin(); z != xy->second.end(); z++)
			cout << *z << " ";
		cout << endl;
		
	}

	bool result = TRUE;

	while(result)
	{
		result = FALSE;
		for(itnt = nonTerminals.begin(); itnt != nonTerminals.end(); itnt++)
		{
			for(x = rules[*itnt].begin(); x != rules[*itnt].end(); x++)
			{
				vector<string> temp;
				split(*x, ' ', temp);
				for(i = 0; i < temp.size() - 1; i++)
				{
					for(j = i + 1; j < temp.size(); j++)
					{
						if(firstSet[temp[j]].find("#") == firstSet[temp[j]].end())
							break;
					}
					if(j == temp.size())
					{
						set<string>::iterator it_s;
						if(followSet.find(*itnt) != followSet.end())
						{
							for(it_s = followSet[*itnt].begin(); it_s != followSet[*itnt].end(); it_s++)
							{
								if(followSet[temp[i]].find(*it_s) == followSet[temp[i]].end())
									result = TRUE;
								followSet[temp[i]].insert(*it_s);
							}	
						}
					}
				}
				set<string>::iterator s_it;
				if(followSet.find(*itnt) != followSet.end())
				{
					for(s_it = followSet[*itnt].begin(); s_it != followSet[*itnt].end(); s_it++)
					{	
						if(followSet[temp[i]].find(*s_it) == followSet[temp[i]].end())
							result = TRUE;
						followSet[temp[i]].insert(*s_it);
					}
				}
			}			
		}
	}

	cout << "-------------------------------------" << endl;
	cout << "Follow Sets" << endl;
	
	for(xy = followSet.begin(); xy != followSet.end(); xy++)
	{
		cout << xy->first << "--> " ;
		for(z = xy->second.begin(); z != xy->second.end(); z++)
			cout << *z << " ";
		cout << endl;
		
	}
}
