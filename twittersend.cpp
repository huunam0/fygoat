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
    char consumerkey [BUFFER_SIZE];
    char consumersecret [BUFFER_SIZE];
    char token[BUFFER_SIZE];
    char tokensecret[BUFFER_SIZE];

	user_name[0]=0;
	password[0]=0;
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
            //read_buf(buf, "F_T_UNAME",user_name);
            //read_buf(buf, "F_T_PASS",password);
            read_buf(buf,"F_HOST_NAME",host_name);
            read_buf(buf, "F_USER_NAME",user_name);
            read_buf(buf, "F_PASSWORD",password);
            read_buf(buf, "F_DB_NAME",db_name);
            read_int(buf , "F_PORT_NUMBER", &port_number);
            read_buf(buf, "F_T_C_KEY",consumerkey);
            read_buf(buf, "F_T_C_SECRET",consumersecret);
            read_buf(buf, "F_T_ATOKEN",token);
            read_buf(buf, "F_T_ATOKENS",tokensecret);
        }
        t_name=std::string(user_name);
        t_pass=std::string(password);
        t_key=std::string(consumerkey);
        t_secret=std::string(consumersecret);
        t_token=std::string(token);
        t_tokensecret=std::string(tokensecret);
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
    if (DEBUG) cout<<sql<<endl;
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
			//sleep(20);
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
    //twitterObj.setTwitterUsername(t_name);
    //twitterObj.setTwitterPassword(t_pass);
    twitterObj.getOAuth().setConsumerKey(t_key);
    twitterObj.getOAuth().setConsumerSecret(t_secret);
    std::string myOAuthAccessTokenKey(t_token);
    std::string myOAuthAccessTokenSecret(t_tokensecret);
    if( myOAuthAccessTokenKey.size() && myOAuthAccessTokenSecret.size() )
    {
        twitterObj.getOAuth().setOAuthTokenKey( myOAuthAccessTokenKey );
        twitterObj.getOAuth().setOAuthTokenSecret( myOAuthAccessTokenSecret );
    }
    else
    {
        /* Step 2: Get request token key and secret */
        return false;
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
    if (!initTwitter())
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
    if (!initTwitter())
    {
        write_log("Init twitter fail");
        return -1;
    }
    write_log("Init twitter OK");
    while(true)
    {
        init_mysql();
        work();
        break;
    }
    //work();

    return 0;
}
