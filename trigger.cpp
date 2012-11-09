#include "needed.h"
#include <time.h>
#include <mysql/mysql.h>
#include <string.h>
#include <stdarg.h>

#define BUFFER_SIZE 1024
static char host_name[BUFFER_SIZE];
static char user_name[BUFFER_SIZE];
static char password [BUFFER_SIZE];
static char db_name  [BUFFER_SIZE];
static char triggerurl [BUFFER_SIZE];
static int port_number;
static int DEBUG=0;

static MYSQL *conn;

bool isFirstTime=true;

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
		//if (DEBUG) printf("%s\n",value);
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

	sprintf(buffer,"/var/log/footygoat/match.log");
	FILE *fp = fopen(buffer, "a+");
	if (fp==NULL)
    {
		 fprintf(stderr,"openfile error!\n");
		 system("pwd");
	}va_start(ap, fmt);
	vsprintf(buffer, fmt, ap);
	fprintf(fp,"%s \t %s\n",times,buffer);
	if (DEBUG) printf("%s\n",buffer);
	va_end(ap);
	fclose(fp);

}
bool init_mysql_conf()
{
	FILE *fp=NULL;
	char buf[BUFFER_SIZE];
	host_name[0]=0;
	user_name[0]=0;
	password[0]=0;
	db_name[0]=0;
	triggerurl[0]=0;
	port_number=3306;

	fp = fopen("/etc/footygoat/footygoat.conf", "r");
	if(fp!=NULL)
    {
        while (fgets(buf, BUFFER_SIZE - 1, fp))
        {
            read_buf(buf,"F_HOST_NAME",host_name);
            read_buf(buf, "F_USER_NAME",user_name);
            read_buf(buf, "F_PASSWORD",password);
            read_buf(buf, "F_DB_NAME",db_name);
            read_int(buf , "F_PORT_NUMBER", &port_number);
            read_buf(buf, "F_TRIGGER_URL",triggerurl);
        }
		return true;
		fclose(fp);
		cout<<triggerurl<<endl;
    }
    else
    {
        write_log("Cannot open file 'footygoat.conf'");
        return false;
    }
}
bool executesql(const char * sql){

	if (mysql_real_query(conn,sql,strlen(sql)))
    {
		//sleep(20);
		write_log("Error iin sql %s:%s",sql,mysql_error(conn));
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

void checktriggers()
{
    char output[100];
    char url[1000];
    sprintf(url,"wget -O - %s/gottriggers.php",triggerurl);
    FILE *p = popen(url, "r");
    if(p != NULL)
        while(fgets(output, sizeof(output), p) != NULL)
        {
            cout << output<<endl;
        }

}

int main(int argc, char** argv)
{
    checktriggers();
}
