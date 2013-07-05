#include <time.h>
#include <string.h>
#include "ftrigger.h"
#include <stdarg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>

twitCurl twitterObj;
string t_name, t_pass, t_key, t_secret, t_token, t_tokensecret,replyMsg;
#define BUFFER_SIZE 1024
static char host_name[BUFFER_SIZE];
static char user_name[BUFFER_SIZE];
static char password [BUFFER_SIZE];
static char db_name  [BUFFER_SIZE];
static int port_number;
static bool DEBUG = false;


string hadsent=string("#");




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
void read_int(char * buf,const char * key,int * value)
{
	char buf2[BUFFER_SIZE];
	if (read_buf(buf,key,buf2))
		sscanf(buf2, "%d", value);

}

bool init_conf()
{
	FILE *fp=NULL;
	char buf[BUFFER_SIZE];

    char t_username[BUFFER_SIZE];
    char t_password [BUFFER_SIZE];
    char consumerkey [BUFFER_SIZE];
    char consumersecret [BUFFER_SIZE];
    char token[BUFFER_SIZE];
    char tokensecret[BUFFER_SIZE];


	t_username[0]=0;
	t_password[0]=0;
	consumerkey[0]=0;
	consumersecret[0]=0;
    host_name[0]=0;
	user_name[0]=0;
	password[0]=0;
	db_name[0]=0;
	port_number=3306;

	fp = fopen("/etc/footygoat/footygoat.conf", "r");
	if(fp!=NULL)
    {
        while (fgets(buf, BUFFER_SIZE - 1, fp))
        {
            //read_buf(buf, "F_T_UNAME",t_username);
            //read_buf(buf, "F_T_PASS",t_password);
            read_buf(buf,"F_HOST_NAME",host_name);
            read_buf(buf, "F_USER_NAME",user_name);
            read_buf(buf, "F_PASSWORD",password);
            read_buf(buf, "F_DB_NAME",db_name);
            read_int(buf , "F_PORT_NUMBER", &port_number);
            read_buf(buf, "F_T_C_KEY",consumerkey);
            read_buf(buf, "F_T_C_SECRET",consumersecret);
            read_buf(buf, "F_T_ATOKEN_KEY",token);
            read_buf(buf, "F_T_ATOKEN_SECRET",tokensecret);
        }
        t_name=std::string(t_username);
        t_pass=std::string(t_password);
        t_key=std::string(consumerkey);
        t_secret=std::string(consumersecret);
        t_token=std::string(token);
        t_tokensecret=std::string(tokensecret);
		printf("CK: %s\n",consumerkey);
		printf("CKS: %s\n",consumersecret);
		printf("TK: %s\n",token);
		printf("TKS: %s\n",tokensecret);
		return true;
	//	fclose(fp);
    }
    else
    {
        printf("Cannot open file 'footygoat.conf'");
        return false;
    }
}



bool initTwitter0()
{
    printf("Init twitter");
    twitterObj.getOAuth().setConsumerKey(t_key);
    twitterObj.getOAuth().setConsumerSecret(t_secret);
    std::string myOAuthAccessTokenKey(t_token);
    std::string myOAuthAccessTokenSecret(t_tokensecret);
    twitterObj.getOAuth().setOAuthTokenKey( myOAuthAccessTokenKey );
    twitterObj.getOAuth().setOAuthTokenSecret( myOAuthAccessTokenSecret );
    /* OAuth flow ends */

    /* Account credentials verification */
    if(twitterObj.accountVerifyCredGet())
    {

            twitterObj.getLastWebResponse( replyMsg );
            printf( "\ntwittersend:: twitCurl::accountVerifyCredGet web response:\n%s\n", replyMsg.c_str() );

        return true;
    }
    else
    {

            twitterObj.getLastCurlError(replyMsg);
            printf( "\ntwittersend:: twitCurl::accountVerifyCredGet error:\n%s\n", replyMsg.c_str() );

        return false;
    }
}


bool postTweet(std::string tmpStr)
{
    replyMsg = "";
	printf("postTweet: %s", tmpStr.c_str() );
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
    
        printf("Send message '%s' to user '%s'",message.c_str(),toUser.c_str());
    if (twitterObj.directMessageSend(toUser,message))
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






int main( int argc, char* argv[] )
{
    DEBUG=true;
    printf("I'm running in debug mod = %d",DEBUG);
    //init_conf();
    if (!initTwitter0())
    {
        printf("Init twitter fail");
        return -1;
    }
    int v=0;
	bool b = postTweet("This is test tweet. Ignore.");
	if (b) printf("OK"); else printf("Not OK");

    return 0;
}
