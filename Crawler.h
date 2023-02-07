#ifndef _CRAWLER_H
#define _CRAWLER_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <queue>
#include <vector>
#include <fstream>
#include <direct.h>
#include <curl\curl.h>
#include "htmlstreamparser.h"

using namespace std;

class Crawler
{
private:
	//string domain = "http://www.iasbs.ac.ir/"; //Also seed url
	string domain = "http://www.google.com/"; //Also seed url
	string root_dir = "..\\Output\\";
	struct WEBPAGE
	{
		int webID; // Also file ID
		string url;
	};

	struct FindUrl
	{
		const string url;
		FindUrl(const string& url):url(url){}
		bool operator()(const WEBPAGE& w)const {
			return w.url == url;
		}
	};

	vector<WEBPAGE> visited;

public:
	Crawler();
	~Crawler();
	queue<string> URLs;
	queue<string> init_crawler();
	static size_t write_function(char *, size_t, size_t , void *);
	static string getPageSource(string );
	static queue<string> extractLinks(string );
	string getWebIDStr();
	int getWebID();
	int writeWebSource(string);
	int addVisitedUrl(int, string);
	bool isFindUrl(string);
};


#endif // !_CRAWLER_H
