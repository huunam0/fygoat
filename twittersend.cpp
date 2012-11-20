#include "twittersend.h"
#define BUFFER_SIZE 1024

twitCurl twitterObj;
string t_name, t_pass, t_key, t_secret,replyMsg;

bool DEBUG = true;

void write_log(const char *fmt, ...)
{
	va_list         ap;
	char            buffer[4096];
	char times[20];
    struct tm *sTm;
    time_t now = time (0);
    sTm = gmtime (&now);
    strftime (times, sizeof(times), "%Y-%m-%d %H:%M:%S", sTm);

	sprintf(buffer,"/var/log/footygoat/tweet.log");
	FILE *fp = fopen(buffer, "a+");
	if (fp==NULL)
    {
		 fprintf(stderr,"openfile error!\n");
		 system("pwd");
	}va_start(ap, fmt);
	vsprintf(buffer, fmt, ap);
	fprintf(fp,"%s \t %s\n",times,buffer);
	//if (DEBUG) printf("%s\n",buffer);
	va_end(ap);
	fclose(fp);

}
int after_equal(char * c){
	int i=0;
	for(;c[i]!='\0'&&c[i]!='=';i++);
	return ++i;
}
void trim(char * c)
{
    char buf[BUFFER_SIZE];
    char * start,*end;
    strcpy(buf,c);
    start=buf;
    while(isspace(*start)) start++;
    end=start;
    while(!isspace(*end)) end++;
    *end='\0';
    strcpy(c,start);
}
bool read_buf(char * buf,const char * key,char * value)
{
   if (strncmp(buf,key, strlen(key)) == 0)
    {
		strcpy(value, buf + after_equal(buf));
		trim(value);
		return 1;
   }
   return 0;
}
bool init_conf()
{
	FILE *fp=NULL;
	char buf[BUFFER_SIZE];
    char user_name[BUFFER_SIZE];
    char password [BUFFER_SIZE];
    char consumerkey [BUFFER_SIZE];
    char consumersecret [BUFFER_SIZE];

	user_name[0]=0;
	password[0]=0;
	consumerkey[0]=0;
	consumersecret[0]=0;


	fp = fopen("/etc/footygoat/footygoat.conf", "r");
	if(fp!=NULL)
    {
        while (fgets(buf, BUFFER_SIZE - 1, fp))
        {
            read_buf(buf, "F_T_UNAME",user_name);
            read_buf(buf, "F_T_PASS",password);
            read_buf(buf, "F_T_C_KEY",consumerkey);
            read_buf(buf, "F_T_C_SECRET",consumersecret);
        }
        t_name=std::string(user_name);
        t_pass=std::string(password);
        t_key=std::string(consumerkey);
        t_secret=std::string(consumersecret);

		return true;
	//	fclose(fp);
    }
    else
    {
        write_log("Cannot open file 'footygoat.conf'");
        return false;
    }
}

void printUsage()
{
    printf( "\nUsage:\ntwittersend -u username -p password\n" );
}

bool initTwitter()
{
    twitterObj.setTwitterUsername(t_name);
    twitterObj.setTwitterPassword(t_pass);
    twitterObj.getOAuth().setConsumerKey(t_key);
    twitterObj.getOAuth().setConsumerSecret(t_secret);
    std::string myOAuthAccessTokenKey("");
    std::string myOAuthAccessTokenSecret("");
    std::ifstream oAuthTokenKeyIn;
    std::ifstream oAuthTokenSecretIn;

    oAuthTokenKeyIn.open( "twittersend_token_key.txt" );
    oAuthTokenSecretIn.open( "twittersend_token_secret.txt" );
    char tmpBuf[1024];

    memset( tmpBuf, 0, 1024 );
    oAuthTokenKeyIn >> tmpBuf;
    myOAuthAccessTokenKey = tmpBuf;

    memset( tmpBuf, 0, 1024 );
    oAuthTokenSecretIn >> tmpBuf;
    myOAuthAccessTokenSecret = tmpBuf;

    oAuthTokenKeyIn.close();
    oAuthTokenSecretIn.close();

    if( myOAuthAccessTokenKey.size() && myOAuthAccessTokenSecret.size() )
    {
        /* If we already have these keys, then no need to go through auth again */
        if (DEBUG)
            printf( "\nUsing:\nKey: %s\nSecret: %s\n\n", myOAuthAccessTokenKey.c_str(), myOAuthAccessTokenSecret.c_str() );

        twitterObj.getOAuth().setOAuthTokenKey( myOAuthAccessTokenKey );
        twitterObj.getOAuth().setOAuthTokenSecret( myOAuthAccessTokenSecret );
    }
    else
    {
        /* Step 2: Get request token key and secret */
        std::string authUrl;
        twitterObj.oAuthRequestToken( authUrl );

        /* Step 3: Get PIN  */
        memset( tmpBuf, 0, 1024 );
            /* Else, pass auth url to twitCurl and get it via twitCurl PIN handling */
        twitterObj.oAuthHandlePIN( authUrl );


        /* Step 4: Exchange request token with access token */
        twitterObj.oAuthAccessToken();

        /* Step 5: Now, save this access token key and secret for future use without PIN */
        twitterObj.getOAuth().getOAuthTokenKey( myOAuthAccessTokenKey );
        twitterObj.getOAuth().getOAuthTokenSecret( myOAuthAccessTokenSecret );

        /* Step 6: Save these keys in a file or wherever */
        std::ofstream oAuthTokenKeyOut;
        std::ofstream oAuthTokenSecretOut;

        oAuthTokenKeyOut.open( "twittersend_token_key.txt" );
        oAuthTokenSecretOut.open( "twittersend_token_secret.txt" );

        oAuthTokenKeyOut.clear();
        oAuthTokenSecretOut.clear();

        oAuthTokenKeyOut << myOAuthAccessTokenKey.c_str();
        oAuthTokenSecretOut << myOAuthAccessTokenSecret.c_str();

        oAuthTokenKeyOut.close();
        oAuthTokenSecretOut.close();
    }
    /* OAuth flow ends */

    /* Account credentials verification */
    if(twitterObj.accountVerifyCredGet())
    {
        if (DEBUG)
        {
            twitterObj.getLastWebResponse( replyMsg );
            printf( "\ntwittersend:: twitCurl::accountVerifyCredGet web response:\n%s\n", replyMsg.c_str() );
        }
        return true;
    }
    else
    {
        if (DEBUG)
        {
            twitterObj.getLastCurlError(replyMsg);
            printf( "\ntwittersend:: twitCurl::accountVerifyCredGet error:\n%s\n", replyMsg.c_str() );
        }
        return false;
    }
}
bool postTweet(std::string tmpStr)
{
    replyMsg = "";
    if( twitterObj.statusUpdate( tmpStr ) )
    {
        twitterObj.getLastWebResponse( replyMsg );
        printf( "\ntwittersend:: twitCurl::statusUpdate web response:\n%s\n", replyMsg.c_str() );
        return true;
    }
    else
    {
        twitterObj.getLastCurlError( replyMsg );
        printf( "\ntwittersend:: twitCurl::statusUpdate error:\n%s\n", replyMsg.c_str() );
        return  false;
    }
}
bool sendDirectMessage(string toUser,string message)
{
    return twitterObj.directMessageSend(toUser,message);
}

void test_popen()
{
  // setup
  string data;
  FILE *stream;
  int MAX_BUFFER = 1000;
  char buffer[MAX_BUFFER];

  // do it
  stream = popen("wget -O - http://footygoat.com/gottriggers.php", "r");
  while ( fgets(buffer, MAX_BUFFER, stream) != NULL )
    cout<<buffer<<endl;
    //data.append(buffer);
  pclose(stream);

  // exit
  //return trim(data);
}
void test_tsend()
{
    init_conf();
    if (!initTwitter())
    {
        cout<<"Init fail"<<endl;
        return ;
    }
    sendDirectMessage(string("huunam0"),string("Please go to chamthi.net"));
}
int main( int argc, char* argv[] )
{
    test_popen();
    return 0;
}
