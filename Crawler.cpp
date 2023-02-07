#include "Crawler.h"

Crawler::Crawler()
{
	this->URLs.push(domain);
}

Crawler::~Crawler()
{
}

// Init with seed url (domain)
queue<string> Crawler::init_crawler()
{
	string source = getPageSource(domain);
	queue<string> initQ = extractLinks(source);
	// Write source to the file
	_mkdir(root_dir.c_str());
	writeWebSource(source);
	addVisitedUrl(0, domain);
	return initQ;
}

size_t Crawler::write_function(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	string &buffer = *(string*)userdata;
	buffer.append(ptr, size*nmemb);
	return size*nmemb;
}

string Crawler::getPageSource(string url)
{
	CURL *curl;
	CURLcode res;
	string buffer;
	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_function);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5000);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:47.0) Gecko/20100101 Firefox/47.0");
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

		res = curl_easy_perform(curl);
	}
	curl_easy_cleanup(curl);
	return buffer;
}

queue<string> Crawler::extractLinks(string source)
{
	char tag[1], attr[4], val[128];
	queue<string> linkQ;
	HTMLSTREAMPARSER *hsp;
	hsp = html_parser_init();

	html_parser_set_tag_to_lower(hsp, 1);
	html_parser_set_attr_to_lower(hsp, 1);
	html_parser_set_tag_buffer(hsp, tag, sizeof(tag));
	html_parser_set_attr_buffer(hsp, attr, sizeof(attr));
	html_parser_set_val_buffer(hsp, val, sizeof(val) - 1);
	size_t pageSize = size(source), p;
	for (p = 0; p < pageSize; p++)
	{
		html_parser_char_parse(hsp, source[p]);
		if (html_parser_cmp_tag(hsp, "a", 1))
			if (html_parser_cmp_attr(hsp, "href", 4))
				if (html_parser_is_in(hsp, HTML_VALUE_ENDED)) {
					html_parser_val(hsp)[html_parser_val_length(hsp)] = '\0';
					linkQ.push(html_parser_val(hsp));
				}
	}
	html_parser_cleanup(hsp);
	return linkQ;
}

string Crawler::getWebIDStr()
{
	int id = visited.size()-1;
	return to_string(id);
}

int Crawler::getWebID()
{
	int id = visited.size()-1;
	return id;
}

int Crawler::writeWebSource(string source)
{
	fstream weboutput;
	string filename = getWebIDStr();
	string fullpath = root_dir + filename;
	weboutput.open(fullpath, fstream::out);
	weboutput << source;
	weboutput.close();
	return 0;
}

int Crawler::addVisitedUrl(int webID, string url)
{
	WEBPAGE visitedWeb;
	visitedWeb.webID = webID;
	visitedWeb.url = url;
	visited.push_back(visitedWeb);
	return 0;
}

bool Crawler::isFindUrl(string url)
{
	vector<WEBPAGE>::iterator it;
	it = find_if(visited.begin(), visited.end(), FindUrl(url));
	if (it != visited.end()) // If it's already exist in visited
		return true;
	else
		return false;
}