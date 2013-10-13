#include <time.h>
#include <mysql/mysql.h>
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
#define LOCKFILE "/var/run/ftrigger.pid"
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
static MYSQL *conn;
string hadsent=string("#");
//For daemon start:
static bool STOP=false;
int sleep_time = 5;

void call_for_exit(int s)
{
   STOP=true;
   printf("Stopping ftrigger...\n");
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
void write_log(const char *fmt, ...)
{
	va_list         ap;
	char            buffer[4096];
	char times[20];
    struct tm *sTm;
    time_t now = time (0);
    sTm = gmtime (&now);
    strftime (times, sizeof(times), "%Y-%m-%d %H:%M:%S", sTm);

	sprintf(buffer,"/var/log/footygoat/trigger.log");
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
int init_mysql(bool bForce = false) {
    if((conn==NULL) || (bForce))
    {
		conn=mysql_init(NULL);		// init the database connection
		/* connect the database */
		const char timeout=30;
		mysql_options(conn,MYSQL_OPT_CONNECT_TIMEOUT,&timeout);

		if(!mysql_real_connect(conn,host_name,user_name,password,db_name,port_number,0,0))
        {
			write_log("Error init mysql %d: %s",bForce?1:0,mysql_error(conn));
			//write_log("host=%s,user=%s,pas=%s,db=%s,port=%d",host_name,user_name,password,db_name,port_number);
			return false;
		}
	}
	if (!executesql("set names utf8"))
    {
		if(!bForce)
        {
			write_log("Re-initialize mysql ");
			return init_mysql(true);
		}
		else
        {
            init_conf();
            return false;
        }

    }
	return true;
}


bool initTwitter0(bool debug=DEBUG)
{
    write_log("Init twitter");
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
        if (debug)
        {
            twitterObj.getLastWebResponse( replyMsg );
            write_log( "\ntwittersend:: twitCurl::accountVerifyCredGet web response:\n%s\n", replyMsg.c_str() );
        }
        return true;
    }
    else
    {
        if (debug)
        {
            twitterObj.getLastCurlError(replyMsg);
            write_log( "\ntwittersend:: twitCurl::accountVerifyCredGet error:\n%s\n", replyMsg.c_str() );
        }
        return false;
    }
}


bool postTweet(std::string tmpStr,bool debug=DEBUG)
{
    replyMsg = "";
	if (debug) write_log("postTweet: %s", tmpStr.c_str() );
    if( twitterObj.statusUpdate( tmpStr ) )
    {
        if (debug)
        {
            twitterObj.getLastWebResponse( replyMsg );
            write_log( "\ntwittersend:: twitCurl::statusUpdate web response:\n%s\n", replyMsg.c_str() );
        }
        return true;
    }
    else
    {
        if (debug)
        {
            twitterObj.getLastCurlError( replyMsg );
            write_log( "\ntwittersend:: twitCurl::statusUpdate error:\n%s\n", replyMsg.c_str() );
        }
        return  false;
    }
}
bool sendDirectMessage(string toUser,string message, bool debug=DEBUG)
{
    if (DEBUG)
        printf("Send message '%s' to user '%s'",message.c_str(),toUser.c_str());
    if (twitterObj.directMessageSend(toUser,message))
    {
        if (debug)
        {
            twitterObj.getLastWebResponse( replyMsg );
            write_log( "\ntwittersend:: twitCurl::statusUpdate web response:\n%s\n", replyMsg.c_str() );
        }
        return true;
    }
    else
    {
        if (debug)
        {
            twitterObj.getLastCurlError( replyMsg );
            write_log( "\ntwittersend:: twitCurl::statusUpdate error:\n%s\n", replyMsg.c_str() );
        }
        return  false;
    }
}
void reverify(int s)
{
    if (!initTwitter0(true))
    {
        write_log("Cannot initialize Twitter");
        if (!initTwitter0())
        {
            write_log("Cannot initialize Twitter again, it must stop");
            STOP=true;
        }
    }
    sendDirectMessage(string("huunamtran"),string("Ban co khoe khong?"),true);
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
int post_blog(char *match_id)
{
    char cmd[200];
    FILE *stream;
    int MAX_BUFFER = 1000;
    int p_id=0;
    char buffer[MAX_BUFFER];
    sprintf(cmd,"wget -q -O - http://localhost/livefootystats/postblog.php?m=%s",match_id);
    //system(cmd);
    stream = popen(cmd, "r");
    while ( fgets(buffer, MAX_BUFFER, stream) != NULL )
    {
        if (strlen(buffer)>1)
        {
            char sql[500];
            sprintf(sql,"update wp_posts set  post_status = 'publish' where ID=%s",buffer);
            executesql(sql);
            write_log("Add blog entry %s",buffer);
            p_id = atoi(buffer);
        }
        break;
    }
    pclose(stream);
    return p_id;
}
void postfb(int p_id)
{
    char cmd[500];
    char a_t[500];
    char p_i[]="308602152532791";
    FILE *stream;
    stream = popen("wget -q -O - http://www.footygoat.com/getoption.php?key=fb-page-act", "r");
    fgets(a_t, 500, stream);
    pclose(stream);
    //char lnk[]="http://baogia.info/bao-gia-vang/";
    sprintf(cmd,"curl -X POST -F 'link=http://www.footygoat.com/?p=%d' -F 'access_token=%s' https://graph.facebook.com/%s/feed",p_id,a_t,p_i);
    system(cmd);
}
void tweet_match(char *user_id, char *user_twitter, char *match_id, char *match_teams)
{
    /**/
    string tmp= string(user_id)+string("-")+string(match_id)+string("#");
    if (hadsent.find(string("#")+tmp)!=string::npos) return;
    hadsent+=tmp;
    write_log("DM:to %s with match %s and tmp=%s",user_id,match_id,tmp.c_str());
    /**/
    char msg[250],tweet[250];
    sprintf(msg,"ALERT %s www.footygoat.com",match_teams);
    if (sendDirectMessage(string(user_twitter),string(msg)))
    {
        char sql[500];
        sprintf(sql,"insert into f_sent (user_id,match_id,moment) value (%s,%s,NOW());",user_id,match_id);
        executesql(sql);
        if ((strcmp(user_twitter,"FootyGoat")==0))
        {
            int p_id=post_blog(match_id);
            sprintf(tweet,"%s/?p=%d",msg,p_id);
            postTweet(string(tweet));
            postfb(p_id);
        }
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
    stream = popen("wget -q -O - http://localhost/livefootystats/gottriggers.php", "r");
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
            //cout<<"User id:"<<user_id<<", twit:"<<user_twit<<endl;
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
            //cout<<"Match id:"<<match_id<<", teams:"<<match_team<<endl;
            tweet_match(user_id,user_twit,match_id,match_team);
        }

    }

    pclose(stream);

}
//For daemon:
int lockfile(int fd)
{
	struct flock fl;
	fl.l_type = F_WRLCK;
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	fl.l_len = 0;
	return (fcntl(fd,F_SETLK,&fl));
}
bool already_running()
{
	int fd;
	char buf[16];
	fd = open(LOCKFILE, O_RDWR|O_CREAT, LOCKMODE);
	if (fd < 0)
    {
		write_log("can't open %s: %s", LOCKFILE, strerror(errno));
		return true;
	}
	if (lockfile(fd) < 0)
    {
		if (errno == EACCES || errno == EAGAIN)
		{
			close(fd);
			return true;
		}
		write_log("can't lock %s: %s", LOCKFILE, strerror(errno));
		return true;
	}
	ftruncate(fd, 0);
	sprintf(buf,"%d", getpid());
	write(fd,buf,strlen(buf)+1);
	return false;
}
bool daemon_init(void)
 {
     setsid();
     umask(0); /* clear file mode creation mask */
     close(0); /* close stdin */
     close(1); /* close stdout */
     close(2); /* close stderr */
     return(true);
}
int main( int argc, char* argv[] )
{
    if (argc>1) if (strcmp(argv[1],"debug")==0) DEBUG=true;
    write_log("I'm running in debug mod = %d",DEBUG);
    init_conf();
    if (!initTwitter0())
    {
        write_log("Init twitter fail");
        return -1;
    }
    int v=0;
	if (!DEBUG) daemon_init();
    if (already_running())
    {
        write_log("Ftrigger is already running!");
        return 1;
    }
	signal(SIGQUIT,call_for_exit);
	signal(SIGKILL,call_for_exit);
	signal(SIGTERM,call_for_exit);
	signal(SIGHUP,reverify);
    while(!STOP)
    {
        init_mysql();
        work();
    	sleep(sleep_time);
	}
    //work();

    return 0;
}
