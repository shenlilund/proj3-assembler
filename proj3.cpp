#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <stdlib.h>

using namespace std;

int INSTRSIZE = 12;
int INTSIZE = 4;

int main(int argc, char* argv[]) 
{
    if (argc < 2)
    {
        cout << "Filename required\n";
        return -1;
    }

    ifstream infile;	

	try
	{
		infile.open(argv[1]);
	}
	catch (int e)
	{
		cout << "Could not open file.\n"; 
		return -1;
	}

	vector<string> instructions;
	instructions.push_back("TRP");
	instructions.push_back("JMP");
	instructions.push_back("JMR");
	instructions.push_back("BNZ");
	instructions.push_back("BGT");
	instructions.push_back("BLT");
	instructions.push_back("BRZ");
	instructions.push_back("MOV");
	instructions.push_back("LDA");
	instructions.push_back("STR");
	instructions.push_back("LDR");
	instructions.push_back("STB");
	instructions.push_back("LDB");
	instructions.push_back("ADD");
	instructions.push_back("ADI");
	instructions.push_back("SUB");
	instructions.push_back("MUL");
	instructions.push_back("DIV");
	instructions.push_back("AND");
	instructions.push_back("CMP");
	// instructions.push_back("STR");
	// instructions.push_back("LDR");
	// instructions.push_back("STB");
	// instructions.push_back("LDB");
	instructions.push_back(".INT");
	instructions.push_back(".BYT");

	int reg[13];
	//8:PC, 9:SL, 10:SP, 11:FP, 12:SB
	char mem[3000];

	string line;
	string word;

	//assembler pass 1 ---------------------------------------------------------------------------------
	int address = 0;
	int dataAddress = 0;
	int beginData = 0;
	map<string,int> symbolTable;
	while (getline(infile, line))
	{
		vector<string> words;
		istringstream iss(line);
		while (iss >> word)
		{
			if (word[0] == ';')
				break;
			words.push_back(word);
		}

		if (!words.empty())
		{			
			bool label = true;
			for (int j = 0; j < instructions.size(); j++)
			{
				if (instructions[j] ==  words[0])
				{
					label = false;
				}
			}

			if (label == true && symbolTable.find(words[0]) == symbolTable.end())
			{
				if (words[1] == ".BYT" || words[1] == ".INT")
				{
					symbolTable[words[0]] = dataAddress;
				}
				else
				{
					symbolTable[words[0]] = address;
				}
			}
			// for (int i = 0; i < words.size(); i++)
			// {
			// 	cout << words[i] << endl;
			// }
			if (words[0] == ".BYT" || words[1] == ".BYT")
			{
				dataAddress++;
			}
			else if (words[0] == ".INT" || words[1] == ".INT")
			{
				dataAddress+=INTSIZE;
			}
			else
			{
				address+=INSTRSIZE;
				beginData = address;
			}
		}

		
	}
	// cout << beginData << endl;
	// cout << symbolTable["FIVE"] << endl;

	infile.close();
	infile.open(argv[1]);

	address = 0;
	dataAddress = 0;
	//assembler pass 2 -----------------------------------------------------------------------------------
	while (getline(infile, line))
	{
		vector<string> words;
		istringstream iss(line);
		bool first = true;
		while (iss >> word)
		{
				if (word[0] == ';')
					break;
				bool label = true;
				for (int i = 0; i < instructions.size(); i++)
				{
					if (instructions[i] == word)
					{
						label = false;
					}
				}
				if (label == false || first == false)
				{
					words.push_back(word);
					first = false;
				}
		}
		// for (int i = 0; i < words.size(); i++)
		// {
		// 	cout << words[i] << " ";
		// }	
		// cout << endl;
		if (!words.empty())
		{
			for (int j = 0; j < instructions.size(); j++)
			{
				if (instructions[j] ==  words[0])
				{
					int* ptr = static_cast<int*>(static_cast<void*>(&mem[address]));
					int* ptr1 = static_cast<int*>(static_cast<void*>(&mem[beginData+dataAddress]));
					if (words[0] == ".INT")
					{
						string temp;
						temp = words[1];
						int temp1 = atoi(temp.c_str());
						*ptr1 = temp1;	
					}
					else if (words[0] == ".BYT")
					{
							string temp;
							temp = words[1];
							char temp1 = temp[1];
							mem[beginData+dataAddress] = temp1;
					}
					else
					{
						//This gives the number of the instruction. Add is 13, ADI is 14, etc.
						*ptr = j;
					}
					// label
					if (words[0] == "JMP")
					{
						if (!(symbolTable.find(words[1]) == symbolTable.end()))
						{
							*(ptr+1) = symbolTable[words[1]];
						}
						else
						{
							cout << "Label "+ words[1] +" not found" << endl;
							return -1;
						}

					}
					// register
					else if (words[0] == "JMR")
					{
						// int temp;
						// temp = words[1][1] - '0';
						string tempStr = words[1].erase(0,1);
						int tempInt = atoi(tempStr.c_str());
						*(ptr+1) = tempInt;
					}
					// register, label
					else if (words[0] == "BNZ" || words[0] == "BGT" || words[0] == "BLT" || words[0] == "BRZ" || words[0] == "LDA")
					{
						// int temp;
						string tempStr = words[1].erase(0,1);
						int tempInt = atoi(tempStr.c_str());
						// temp = words[1][1] - '0';
						*(ptr+1) = tempInt;

						if (!(symbolTable.find(words[2]) == symbolTable.end()))
						{
							*(ptr+2) = symbolTable[words[2]];
						}	
						else
						{
							cout << "Label "+ words[2] +" not found" << endl;
							return -1;
						}
					}
					else if (words[0] == "LDR" || words[0] == "LDB" || words[0] == "STB" || words[0] == "STR")
					{
						//register indirect with register - register 
						string str = words[2];
						if (str[0] == 'R' && (str[1] == '0' || str[1] == '1' || str[1] == '2' || str[1] == '3' || str[1] == '4' || str[1] == '5' || str[1] == '6' || str[1] == '7'))
						{
							if (words[0] == "LDR")
							{
								*ptr = 22;
							}
							else if (words[0] == "LDB")
							{
								*ptr = 24;
							}
							else if (words[0] == "STB")
							{
								*ptr = 23;
							}
							else if (words[0] == "STR")
							{
								*ptr = 21;
							}
							string tempStr = words[1].erase(0,1);
							int tempInt = atoi(tempStr.c_str());
							*(ptr+1) = tempInt;
							string tempStr2 = words[2].erase(0,1);
							int tempInt2 = atoi(tempStr2.c_str());
							*(ptr+2) = tempInt2;
						}
						// register - label  
						else
						{
							string tempStr = words[1].erase(0,1);
							int tempInt = atoi(tempStr.c_str());
							*(ptr+1) = tempInt;

							if (!(symbolTable.find(words[2]) == symbolTable.end()))
							{
								*(ptr+2) = symbolTable[words[2]];
							}	
							else
							{
								cout << "Label "+ words[2] +" not found" << endl;
								return -1;
							}
						}
					}
					// register, register
					else if (words[0] == "MOV" || words[0] == "ADD" || words[0] == "SUB" || words[0] == "MUL" || words[0] == "DIV" || words[0] == "AND" || words[0] == "OR" || words[0] == "CMP")
					{
						string tempStr = words[1].erase(0,1);
						int tempInt = atoi(tempStr.c_str());
						string tempStr2 = words[2].erase(0,1);
						int tempInt2 = atoi(tempStr2.c_str());
						*(ptr+1) = tempInt;
						*(ptr+2) = tempInt2;
					}
					// register, immediate
					else if (words[0] == "ADI")
					{
						string tempStr = words[1].erase(0,1);
						int tempInt = atoi(tempStr.c_str());
						string temp1;
						temp1 = words[2].erase(0,1);
						int temp2 = atoi(temp1.c_str());
						*(ptr+1) = tempInt;
						*(ptr+2) = temp2;
					}
					else if (words[0] == "TRP")
					{
						string temp;
						temp = words[1];
						int temp1 = atoi(temp.c_str());
						*(ptr+1) = temp1;
					}
				}
			}
			if (words[0] == ".BYT")
			{
				dataAddress++;
			}
			else if (words[0] == ".INT")
			{
				dataAddress+=INTSIZE;
			}
			else
			{
				address+=INSTRSIZE;
			}
		}

	
	}

	//8:PC, 9:SL, 10:SP, 11:FP, 12:SB
	reg[9] = address + dataAddress;
	reg[10] = 2999;
	reg[11] = 2999;
	reg[12] = 2999;
	// cout << reg[9] << endl;
	// cout << mem[0] << endl;
	// int* ptr2 = static_cast<int*>(static_cast<void*>(&mem[12]));
	// cout << *ptr2 << endl;
	// cout << *(ptr2+1) << endl;
	// cout << *(ptr2+2) << endl;
	// cout << *(ptr2+4) << endl;

	// char* ptr2 = static_cast<char*>(static_cast<void*>(&mem[beginData]));
	// // cout << *ptr2 << endl;
	// int* ptr3 = static_cast<int*>(static_cast<void*>(ptr2));
	// cout << *(ptr3) << endl;
	// char* ptr5 = static_cast<char*>(static_cast<void*>(ptr3+1));
	// cout << *(ptr5) << endl;
	// int* ptr4 = static_cast<int*>(static_cast<void*>(ptr5+1));
	// cout << *ptr4 << endl;



	//virtual machine ----------------------------------------------------------------------------------
	bool running = true;
	int PC = 0;
	while (running)
	{
		int* ptrPC = static_cast<int*>(static_cast<void*>(&mem[PC]));
		char* ptrData = static_cast<char*>(static_cast<void*>(&mem[beginData]));
		int* intPtr;
		int* intPtr2;	
		int tempInt;
		char* charPtr;
		char* charPtr2;
		char c;
		switch(*ptrPC)
		{
			case 0:
				if (*(ptrPC+1) == 0)
				{
					running = false;
				}
				else if (*(ptrPC+1) == 1)
				{
					cout << reg[7];
					PC+=INSTRSIZE;
				}
				else if (*(ptrPC+1) == 3)
				{
					char* charPtr = static_cast<char*>(static_cast<void*>(&reg[7]));
					cout << *charPtr;
					PC+=INSTRSIZE;
				}
				else if (*(ptrPC+1) == 4)
				{
					c = getchar();
				}
				break;
			case 1:
				//branch to label (JMP)
				PC = *(ptrPC+1);
				break;
			case 4:
				if (reg[*(ptrPC+1)] > 0)
				{
					PC = *(ptrPC+2);
				}
				else
				{
					PC+=INSTRSIZE;
				}
				break;
			case 5:
				if (reg[*(ptrPC+1)] < 0)
				{
					PC = *(ptrPC+2);
				}
				else
				{
					PC+=INSTRSIZE;
				}
				break;
			case 6:
				if (reg[*(ptrPC+1)] == 0)
				{
					PC = *(ptrPC+2);
				}
				else
				{
					PC+=INSTRSIZE;
				}
				break;
			case 7:
				reg[*(ptrPC+1)] = reg[*(ptrPC+2)];
				PC+=INSTRSIZE;
				break;
			case 8:
				reg[*(ptrPC+1)] = *(ptrPC+2);
				PC+=INSTRSIZE;
				break;
			// case 9: //STR
			// 	intPtr = static_cast<int*>(static_cast<void*>(ptrData+*()))
			case 10:
				intPtr = static_cast<int*>(static_cast<void*>(ptrData+*(ptrPC+2)));
				reg[*(ptrPC+1)] = *(intPtr);
				PC+=INSTRSIZE;
				break;
			case 13:
				reg[*(ptrPC+1)] += reg[*(ptrPC+2)];
				PC+=INSTRSIZE;
				break;
			case 14:
				reg[*(ptrPC+1)] += *(ptrPC+2);
				PC+=INSTRSIZE;
				break;
			case 15:
				reg[*(ptrPC+1)] -= reg[*(ptrPC+2)];
				PC+=INSTRSIZE;
				break;
			case 16:
				reg[*(ptrPC+1)] *= reg[*(ptrPC+2)];
				PC+=INSTRSIZE;
				break;
			case 17:
				reg[*(ptrPC+1)] /= reg[*(ptrPC+2)];
				PC+=INSTRSIZE;
				break;
			case 21:
				tempInt = reg[*(ptrPC+2)];
				intPtr = static_cast<int*>(static_cast<void*>(ptrData+tempInt));
				intPtr2 = static_cast<int*>(static_cast<void*>(&reg[*(ptrPC+1)]));
				*intPtr = *intPtr2;
				PC+=INSTRSIZE;
				break;
			case 22:
				tempInt = reg[*(ptrPC+2)];
				intPtr = static_cast<int*>(static_cast<void*>(ptrData+tempInt));
				reg[*(ptrPC+1)] = *(intPtr);				 
				PC+=INSTRSIZE;
				break;
			case 23:
				tempInt = reg[*(ptrPC+2)];
				charPtr = static_cast<char*>(static_cast<void*>(ptrData+tempInt));
				charPtr2 = static_cast<char*>(static_cast<void*>(&reg[*(ptrPC+1)]));
				*charPtr = *charPtr2;
				PC+=INSTRSIZE;
				break;
			case 24:
				tempInt = reg[*(ptrPC+2)];
				charPtr = static_cast<char*>(static_cast<void*>(ptrData+tempInt));
				reg[*(ptrPC+1)] = *(charPtr);				 
				PC+=INSTRSIZE;
				break;
		}
	}
}