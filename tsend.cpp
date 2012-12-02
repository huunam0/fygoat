#include "twittersend.h"
#include <mysql/mysql.h>
#include <string.h>
#include <stdarg.h>

twitCurl twitterObj;
string t_name, t_pass, t_key, t_secret, t_token, t_tokensecret,replyMsg;
#define BUFFER_SIZE 1024
static char host_name[BUFFER_SIZE];
static char user_name[BUFFER_SIZE];
static char password [BUFFER_SIZE];
static char db_name  [BUFFER_SIZE];
static int port_number;
bool DEBUG = true;
static MYSQL *conn;

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
        printf("KEY:%s\n",t_key.c_str());
        printf("SECRET:%s\n",t_secret.c_str());
        printf("Token key:%s\n",t_token.c_str());
        printf("Token secret:%s\n",t_tokensecret.c_str());
		return true;
	//	fclose(fp);
    }
    else
    {
        write_log("Cannot open file 'footygoat.conf'");
        return false;
    }
}
bool executesql(const char * sql)
{
	if (mysql_real_query(conn,sql,strlen(sql)))
    {
		//sleep(20);
		write_log("Error in sql %s:%s",sql,mysql_error(conn));
		//conn=NULL;
		return false;
	}
	else
	    return true;
}
int init_mysql() {
    if(conn==NULL)
    {
		conn=mysql_init(NULL);		// init the database connection
		/* connect the database */
		const char timeout=30;
		mysql_options(conn,MYSQL_OPT_CONNECT_TIMEOUT,&timeout);

		if(!mysql_real_connect(conn,host_name,user_name,password,db_name,port_number,0,0))
        {
			write_log("Error init mysql: %s",mysql_error(conn));
			write_log("host=%s,user=%s,pas=%s,db=%s,port=%d",host_name,user_name,password,db_name,port_number);
			return false;
		}
	}
	if (!executesql("set names utf8"))
        return false;
	return true;
}
void printUsage()
{
    printf( "\nUsage:\ntwittersend -u username -p password\n" );
}

bool initTwitter()
{
    //char tmpBuf[1024];
    twitterObj.setTwitterUsername(t_name);
    twitterObj.setTwitterPassword(t_pass);
    twitterObj.getOAuth().setConsumerKey(t_key);
    twitterObj.getOAuth().setConsumerSecret(t_secret);
    std::string myOAuthAccessTokenKey("");
    std::string myOAuthAccessTokenSecret("");
    std::ifstream oAuthTokenKeyIn;
    std::ifstream oAuthTokenSecretIn;
    char token_key[]="twittersend_token_key.txt";
    char token_secret[]="twittersend_token_secret.txt";
    oAuthTokenKeyIn.open(token_key);
    oAuthTokenSecretIn.open(token_secret);
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
        std::string authUrl;
        twitterObj.oAuthRequestToken( authUrl );
        twitterObj.oAuthHandlePIN( authUrl );


        /* Step 4: Exchange request token with access token */
        twitterObj.oAuthAccessToken();

        /* Step 5: Now, save this access token key and secret for future use without PIN */
        twitterObj.getOAuth().getOAuthTokenKey( myOAuthAccessTokenKey );
        twitterObj.getOAuth().getOAuthTokenSecret( myOAuthAccessTokenSecret );

        /* Step 6: Save these keys in a file or wherever */
        std::ofstream oAuthTokenKeyOut;
        std::ofstream oAuthTokenSecretOut;

        oAuthTokenKeyOut.open(token_key);
        oAuthTokenSecretOut.open(token_secret);

        oAuthTokenKeyOut.clear();
        oAuthTokenSecretOut.clear();

        oAuthTokenKeyOut << myOAuthAccessTokenKey.c_str();
        oAuthTokenSecretOut << myOAuthAccessTokenSecret.c_str();

        oAuthTokenKeyOut.close();
        oAuthTokenSecretOut.close();
        /* Step 2: Get request token key and secret */

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
bool initTwitter0()
{
    //char tmpBuf[1024];
    //twitterObj.setTwitterUsername(t_name);
    //twitterObj.setTwitterPassword(t_pass);
    twitterObj.getOAuth().setConsumerKey(t_key);
    twitterObj.getOAuth().setConsumerSecret(t_secret);
    std::string myOAuthAccessTokenKey(t_token);
    std::string myOAuthAccessTokenSecret(t_tokensecret);
    //std::ifstream oAuthTokenKeyIn;
    //std::ifstream oAuthTokenSecretIn;
    //char token_key[]="twittersend_token_key.txt";
    //char token_secret[]="twittersend_token_secret.txt";
    //oAuthTokenKeyIn.open(token_key);
    //oAuthTokenSecretIn.open(token_secret);
    //char tmpBuf[1024];

    //memset( tmpBuf, 0, 1024 );
    //oAuthTokenKeyIn >> tmpBuf;
    //myOAuthAccessTokenKey = tmpBuf;

    //memset( tmpBuf, 0, 1024 );
    //oAuthTokenSecretIn >> tmpBuf;
    //myOAuthAccessTokenSecret = tmpBuf;

    //oAuthTokenKeyIn.close();
    //oAuthTokenSecretIn.close();
    /* If we already have these keys, then no need to go through auth again */
   // if (DEBUG)
        //printf( "\nUsing:\nKey: %s\nSecret: %s\n\n", myOAuthAccessTokenKey.c_str(), myOAuthAccessTokenSecret.c_str() );
    twitterObj.getOAuth().setOAuthTokenKey( myOAuthAccessTokenKey );
    twitterObj.getOAuth().setOAuthTokenSecret( myOAuthAccessTokenSecret );
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
    if (DEBUG)
        write_log("Send message '%s' to user '%s'",message.c_str(),toUser.c_str());
    return twitterObj.directMessageSend(toUser,message);
}

void stra2cpy(char* &dst, char* src)
{
    if (dst!=NULL)
    {
        free (dst);
    }
    dst=(char*)malloc(strlen(src)+1);
    strcpy(dst,src);
}
void tweet_match(char *user_id, char *user_twitter, char *match_id, char *match_teams)
{
    char msg[140];
    sprintf(msg,"[F] match got triggers, #%s:%s",match_id,match_teams);
    if (sendDirectMessage(string(user_twitter),string(msg)))
    {
        char sql[500];
        sprintf(sql,"insert into f_sent (user_id,match_id,moment) value (%s,%s,NOW());",user_id,match_id);
        executesql(sql);
    }
    else
    {
        write_log("Cannot send direct message to %s:%s",user_id,user_twitter);
    }

}
void work()
{
    string data;
    FILE *stream;
    int MAX_BUFFER = 1000;
    char buffer[MAX_BUFFER];
    //char *wrd;
    char *user_id=NULL, *user_twit=NULL, *match_id=NULL, *match_team=NULL;
    stream = popen("wget -q -O - http://footygoat.com/gottriggers.php", "r");
    while ( fgets(buffer, MAX_BUFFER, stream) != NULL )
    {
        char *wrd=strtok(buffer,"#");
        if (strcmp(wrd,"user")==0)
        {
            wrd=strtok(NULL,"#");
            if (wrd!=NULL)
            {
                stra2cpy(user_id,wrd);
                wrd=strtok(NULL,"#");
                if (wrd!=NULL)
                {
                    stra2cpy(user_twit,wrd);
                }

            }
            cout<<"User id:"<<user_id<<", twit:"<<user_twit<<endl;
        }
		else if (strcmp(wrd,"match")==0)
        {
            wrd=strtok(NULL,"#");
            if (wrd!=NULL)
            {
                stra2cpy(match_id,wrd);
                wrd=strtok(NULL,"#");
                if (wrd!=NULL)
                {
                    stra2cpy(match_team,wrd);
                }

            }
            cout<<"Match id:"<<match_id<<", teams:"<<match_team<<endl;
            tweet_match(user_id,user_twit,match_id,match_team);
        }

    }

    pclose(stream);

}
void test_tsend()
{
    init_conf();
    if (!initTwitter0())
    {
        cout<<"Init fail"<<endl;
        return ;
    }
    write_log("Init twitter OK");
    sendDirectMessage(string("huunam0"),string("Please go to chamthi.net"));
}
int main( int argc, char* argv[] )
{
    init_conf();
    if (!initTwitter0())
    {
        write_log("Init twitter fail");
        return -1;
    }
    write_log("Init twitter OK");
    int v=0;
    while(true)
    {
        init_mysql();
        work();
        //if (v++>2)
        break;
    }
    //work();

    return 0;
}
