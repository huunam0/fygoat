#include "twittersend.h"
#include <string.h>
#include <stdarg.h>
string t_key=string("YK8QsxScQyuKpqXufreNIQ");
string t_secret=string("C2ai7LVvKjhIQIqYDbryilQLswhcylh54jknCBuXfQ");
string t_accesstokenkey=string("406312188-H9QfJrIETdelkLXbCTIGDxzcy4pJtf5ng6zSVXZe");
string t_accesstokensecret=string("tTPyrV0768NVtHJ7dnDLMl9sqoB7sDqGiutehLADspY");
string replyMsg;
int main( int argc, char* argv[] )
{
	twitCurl twitterObj;
	twitterObj.getOAuth().setConsumerKey(t_key);
	twitterObj.getOAuth().setConsumerSecret(t_secret);
	twitterObj.getOAuth().setOAuthTokenKey(t_accesstokenkey );
	twitterObj.getOAuth().setOAuthTokenSecret( t_accesstokensecret );
	if(twitterObj.accountVerifyCredGet())
	{
		twitterObj.getLastWebResponse(replyMsg);
		printf("OK:%s\n",replyMsg.c_str());
	} 
	else
	{
		twitterObj.getLastCurlError(replyMsg);
		printf("NOK:%s\n",replyMsg.c_str());
	}
	return 0;
}