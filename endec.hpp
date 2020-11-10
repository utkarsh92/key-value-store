#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

using namespace std;
char padd = '#';

char *encode(char status, char key[256], char value[256])
{
	char s[1], k[256], v[256];
	char *code = (char *)malloc(513);
	code[0] = '\0';

	memset(s, padd, 1);
	memset(k, padd, 256);
	memset(v, padd, 256);

	strncpy(s, &status, 1);
	strncpy(k, key, strlen(key));
	strncpy(v, value, strlen(value));

	strncat(code, s, 1);
	strncat(code, k, 256);
	strncat(code, v, 256);

	return code;
}

char *encode2(string status, string key, string value)
{
	char s[1], k[256], v[256];
	char *code = (char *)malloc(513);
	code[0] = '\0';

	memset(s, padd, 1);
	memset(k, padd, 256);
	memset(v, padd, 256);

	strncpy(s, status.c_str(), 1);
	strncpy(k, key.c_str(), strlen(key.c_str()));
	strncpy(v, value.c_str(), strlen(value.c_str()));

	strncat(code, s, 1);
	strncat(code, k, 256);
	strncat(code, v, 256);

	return code;
}
string *decode(char *m)
{
	string *s = new string[3];
	string as = m;
	s[0] = as[0];
	size_t a = strchr(m + 1, padd) - (m + 1);
	if (a <= 256)
		s[1] = as.substr(1, a);
	else
		s[1] = as.substr(1, 256);
	a = strchr(m + 257, padd) - (m + 257);
	if (a <= 256)
		s[2] = as.substr(257, a);
	else
		s[2] = as.substr(257, 256);

	return s;
}

string getStatus(char *enc)
{
	string *s = decode(enc);

	return s[0];
}

string getKey(char *enc)
{
	string *s = decode(enc);

	return s[1];
}

string getValue(char *enc)
{
	string *s = decode(enc);

	return s[2];
}
/*
int main()
{
	// how to use

	char s = '1';
	char key[256] = "testkey";
	char val[256] = "testvalue";

	char *blk = encode(s, key, val);

	for (int i = 0; i < 513; i++)
		printf("%c", blk[i]);
	printf("\n");

	cout << "status: " << getStatus(blk) << endl;
	cout << "key: " << getKey(blk) << endl;
	cout << "value: " << getValue(blk) << endl;

	return (0);
}
*/