#include "Crawler.h"

using namespace std;

int main(int argc, char *argv[])
{
	Crawler crawler = Crawler();
	queue<string> initQ;
	initQ = crawler.init_crawler();
	while (!initQ.empty())
	{
		string currentUrl = initQ.front();
		string currentPageSource;
		currentPageSource= crawler.getPageSource(currentUrl);
		if (currentPageSource != "")
		{
			crawler.addVisitedUrl(crawler.getWebID(), currentUrl);
			crawler.writeWebSource(currentPageSource);
		}
		queue<string> newUrl = crawler.extractLinks(currentPageSource);
		initQ.pop();
		for (size_t i = 0; i < newUrl.size(); i++)
		{
			string tempUrl = newUrl.front();
			// Check for duplicate & then add to front queue
			if(!crawler.isFindUrl(tempUrl))
				initQ.push(tempUrl);
			newUrl.pop();
		}
	}
	getchar();
}