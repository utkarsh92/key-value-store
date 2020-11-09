#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include <bits/stdc++.h>
#include "endec.hpp"

#define F_COUNT 10
#define F_PATH "storage/"
using namespace std;

string getFilePath(string key)
{
	string path;
	int hash = 0;

	for (char c : key)
		hash += c;

	cout << "(HASH " << hash << ")" << endl;
	hash %= F_COUNT;
	path = F_PATH + to_string(hash) + ".txt";
	return path;
}

// int addBlock(char *blk)
// {
// 	string path = F_PATH + to_string(getFileIndex(getKey(blk))) + ".txt";

// 	cout << "writing to " << path << endl;

// 	int fd = open(path.c_str(), O_CREAT | O_RDWR | O_APPEND, 0777);
// 	if (fd < 0)
// 		return 0;

// 	write(fd, blk, 513);
// 	write(fd, "\n", 1);

// 	return 1;
// }

void writeToFile(string path, string data)
{
	fstream file;
	file.open(path, ios::out | ios::trunc);
	if (file.is_open())
	{
		file << data;
	}
	else
	{
		cout << "file not found write" << endl;
	}

	file.close();
}

void delFileKey(string key)
{
	string path = getFilePath(key);
	fstream file;

	cout << "deleting data from " << path << endl;

	file.open(path, ios::in);
	if (file.is_open())
	{
		string data, line, temp_k;
		bool found = false;

		while (getline(file, line))
		{
			if (!found)
			{
				stringstream stream(line);
				getline(stream, temp_k, padd);

				if (temp_k == key)
				{
					//skip this line
					found = true;
				}
				else
				{
					data += line + "\n";
				}
			}
			else
			{
				data += line + "\n";
			}
		}

		file.close();

		if (!found)
		{
			cout << "unknown key!" << endl;
		}
		else
		{
			cout << "found key!" << endl;
			writeToFile(path, data);
		}
	}
	else
	{
		cout << "file not found del" << endl;
		file.close();
	}
}

void putFileKey(string key, string val)
{
	string path = getFilePath(key);
	fstream file;

	cout << "putting data to " << path << endl;

	file.open(path, ios::in);
	if (file.is_open())
	{
		string data, line, temp_k, temp_v;
		bool found = false;

		while (getline(file, line))
		{
			if (!found)
			{
				stringstream stream(line);
				getline(stream, temp_k, padd);

				if (temp_k == key)
				{
					getline(stream, temp_v);
					line = temp_k + padd + val;
					found = true;
				}
			}

			data += line + "\n";
		}

		file.close();

		if (!found)
		{
			cout << "new key!" << endl;
			data += key + padd + val + "\n";
		}
		else
		{
			cout << "found key!" << endl;
		}

		writeToFile(path, data);
	}
	else
	{
		cout << "file not found put" << endl;
		file.close();
		cout << "new file key!" << endl;
		string data = key + padd + val + "\n";
		writeToFile(path, data);
	}
}

string getFileValue(string key)
{
	string path = getFilePath(key);
	// string path = "test.txt";
	fstream file;

	cout << "getting data from " << path << endl;

	file.open(path, ios::in);
	if (file.is_open())
	{
		string line, temp_k, temp_v;
		temp_v = "\0";

		while (getline(file, line))
		{
			stringstream stream(line);
			getline(stream, temp_k, padd);

			if (temp_k == key)
			{
				getline(stream, temp_v);
				break;
			}
		}

		file.close();

		if (temp_v.length())
		{
			return temp_v;
		}
		else
		{
			cout << "not in file" << endl;
			return "\0";
		}
	}

	cout << "error reading file" << endl;
	return "\0";
}

void readData()
{
	fstream file;
	file.open("test.txt", ios::in);
	if (file.is_open())
	{
		string file_content;
		string line;
		while (getline(file, line))
		{
			string key, val;
			stringstream stream(line);
			getline(stream, key, padd);

			if (key == "rit")
			{
				getline(stream, val);

				cout << "key: " << key << endl;
				cout << "val: " << val << endl;

				line = key + padd + "yay";
			}

			file_content += line + "\n";
		}

		// cout << file_content;
		file.close();
		writeToFile("test.txt", file_content);
	}
}

int main()
{
	// int fd = open("storage.txt", O_CREAT | O_RDWR | O_TRUNC, 0777);
	// write(fd, "test\0lol", 8);

	// cout << getFile("asdasd") << endl;
	putFileKey("eshan", "mittal");
	putFileKey("utkarsh", "sharma");
	putFileKey("bcd", "sharma");
	putFileKey("ace", "sharma");
	cout << getFileValue("eshan") << endl;
	cout << getFileValue("bcd") << endl;
	cout << getFileValue("utkarsh") << endl;
	cout << getFileValue("lol") << endl;
	delFileKey("bcd");
	delFileKey("utkarsh");
	putFileKey("utkarsh", "owo");
	return 0;

	char s = '1';
	char key[256] = "utkarsh";
	char val[256] = "wow";

	char *blk = encode(s, key, val);

	for (int i = 0; i < 513; i++)
		printf("%c", blk[i]);
	printf("\n");

	cout << "status: " << getStatus(blk) << endl;
	cout << "key: " << getKey(blk) << endl;
	cout << "value: " << getValue(blk) << endl
		 << endl;

	// addBlock(blk);
	return 0;
}