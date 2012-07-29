#include <time.h>
#include <mysql/mysql.h>
#include <string.h>
#include "shtml.h"
#include <stdarg.h>

#define BUFFER_SIZE 1024
static char host_name[BUFFER_SIZE];
static char user_name[BUFFER_SIZE];
static char password [BUFFER_SIZE];
static char db_name  [BUFFER_SIZE];
static int port_number;
static int DEBUG=0;

static MYSQL *conn;

int m[2][8];
int status=0;
int mid=0;
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
		if (DEBUG) printf("%s\n",value);
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
	sprintf(buffer,"/var/log/footygoat/match.log");
	FILE *fp = fopen(buffer, "a+");
	if (fp==NULL)
    {
		 fprintf(stderr,"openfile error!\n");
		 system("pwd");
	}va_start(ap, fmt);
	vsprintf(buffer, fmt, ap);
	fprintf(fp,"%s\n",buffer);
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
        }
        /*
        cout<<"Host_Name:"<<host_name<<endl;
        cout<<"User_Name:"<<user_name<<endl;
        cout<<"Password:"<<password<<endl;
        cout<<"DB_Name:"<<db_name<<endl;
        cout<<"Port:"<<port_number<<endl;
        */
		return true;
	//	fclose(fp);
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
		sleep(20);
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
			sleep(20);
			return false;
		}
	}
	if (!executesql("set names utf8"))
        return false;
	return true;
}
void emitEvent(int iIndex, int iValue, int iTeam)
{
    char sql[300];
    if (DEBUG)
        cout<<"Event "<<iIndex<<" value: "<<iValue<<" with team "<<iTeam<<endl;
    sprintf(sql,"INSERT INTO f_timeline (`event`, `value`, `team`, `match`, `date`) VALUE (%d,%d,%d,%d,NOW())",iIndex,iValue,iTeam,mid);
    executesql(sql);
    string t,field;
    t=(iTeam==1?"a":"h");
    switch (iIndex)
    {
    case 0:
        field="goals";
        break;
    case 1:
        field="1goals";
        break;
    case 2:
        field="reds";
        break;
    case 3:
        field="yellows";
        break;
    case 4:
        field="shots";
        break;
    case 5:
        field="gshots";
        break;
    case 6:
        field="corner";
        break;
    case 7:
        field="possession";
        break;
    }
    if (!field.empty())
    {
        sprintf(sql,"UPDATE f_matches SET %s=%d where match_id=%d",(t+field).c_str(),iValue,mid);
        //cout<<sql<<endl;
        executesql(sql);
    }

}
void setValue(int iIndex, int iValue, int iTeam)
{
    if ( (isFirstTime) || ((m[iTeam][iIndex]<iValue)&&(iIndex<7)) || ((m[iTeam][iIndex]!=iValue)&&(iIndex==7)))
    {
        m[iTeam][iIndex]=iValue;
        emitEvent(iIndex,iValue,iTeam);
    }
}
bool getMatch(int id)
{
    shtml sh,t,n,nh,nh2;
    int i=0,v=0;
    char url[300];
    bool reCard = true;
    sprintf(url,"http://soccernet.espn.go.com/match/_/id/%d?cc=4716",id);
    //sprintf(url,"http://soccernet.espn.go.com/match?id=%d&cc=4716",id);
    if (DEBUG)
        cout<<"Loading from "<<url<<endl;
    sh.loadFromURL(url);
    if (sh.isEmpty()) return false;
    sh.removeBetween("<!--","-->",-1);
    t=sh.cutTagByName("div");
    while (!t.isEmpty())
    {
        if (t.containAttr("bg-elements"))
        {
            sh=t;
            break;
        }
        if (sh.isEmpty()) break;
        t=sh.cutTagByName("div");
    }
    sh.retainTagByName("div",2);
    sh.retainTagByName("div");
    sh.retainTagByName("div",2);
    sh.retainTagByName("div");
    t=sh.cutTagByName("div",2);
    t.retainTagByName("div");
    n = t.cutTagByName("div");
    n.retainTagByName("span");
    if (n.contain("Full-time"))
    {
        status=4;
    }
    else if (n.contain("Match"))
    {
        status=0;
        return true;
    }
    else if (n.contain("First Half"))
    {
        status=1;
    }
    else if (n.contain("Second Half"))
    {
        status=2;
    }
    else if (n.contain("Final score"))
    {
        status=5;
    }
    else
    {
        status=3;
    }
    if (DEBUG)
        cout<<"Status:"<<status<<endl;
    t.retainTagByName("p");
    //t.viewContent();
    sh.retainTagByName("div",2);
    t=sh.cutTagByName("div");
    if (t.contain("Scoring Summary")) {
        t.retainTagByName("tbody");
        n=t.cutTagByName("tr");
        int sc[2]={0,0};
        int sc1[2]={0,0};
        while (!n.isEmpty())
        {
            for (i=0;i<2;i++)
            {
                nh=n.cutTagByName("td");
                if (nh.contain("'"))
                {
                    nh.retainBetween("(","'");
                    if (nh.contain(" miss ")) break;
                    while (nh.contain(" "))
                        {
                            nh.deleteTo(" ");
                        }
                    v=nh.toInt();
                    sc[i]++;
                    if (v<46) sc1[i]++;
                    //cout<<"Team "<<i<<" goal at "<<v<<" minute th"<<endl;

                }
            }
            n=t.cutTagByName("tr");
        }
        for (i=0;i<2;i++)
        {
            setValue(0,sc[i],i);
            setValue(1,sc1[i],i);
        }

        t=sh.cutTagByName("div");
    }
    if (t.contain("Match Stats")) {
        t.retainTagByName("tbody");
        n=t.cutTagByName("tr");
        if (n.contain("Shots"))
        {
            for (i=0;i<2;i++)
            {
                nh=n.cutTagByName("td",i);
                v=nh.toInt();
                setValue(4,v,i);
                nh.retainBetween("(",")");
                v=nh.toInt();
                setValue(5,v,i);
            }
            n=t.cutTagByName("tr");
        }
        if (n.contain("Fouls"))
        {
            n=t.cutTagByName("tr");
        }
        if (n.contain("Corner Kicks"))
        {
            for (i=0;i<2;i++)
            {
                nh=n.cutTagByName("td",i);
                v=nh.toInt();
                setValue(6,v,i);
            }
            n=t.cutTagByName("tr");
        }
        if (n.contain("Offsides"))
        {
            n=t.cutTagByName("tr");
        }
        if (n.contain("Time of Possession"))
        {
            for (i=0;i<2;i++)
            {
                nh=n.cutTagByName("td",i);
                v=nh.toInt();
                setValue(7,v,i);
            }
            n=t.cutTagByName("tr");
        }
        if (n.contain("Yellow Cards"))
        {
            for (i=0;i<2;i++)
            {
                nh=n.cutTagByName("td",i);
                v=nh.toInt();
                if (v>0) reCard=false;
                setValue(3,v,i);
            }
            n=t.cutTagByName("tr");
        }
        if (n.contain("Red Cards"))
        {
            for (i=0;i<2;i++)
            {
                nh=n.cutTagByName("td",i);
                v=nh.toInt();
                if (reCard) if (v>0) reCard=false;
                setValue(2,v,i);
            }
            n=t.cutTagByName("tr");
        }

        t=sh.cutTagByName("div");
    }
    if (reCard)
        for (i=0; i<2; i++)
        {
            t.removeTagByName("div",3);
            //int card[3];
            v=t.count("soccer-icons-yellowcard");
            setValue(3,v,i);
            v=t.count("soccer-icons-redcard");
            setValue(2,v,i);
            //cout<<"Team "<<i<<" has "<<card[1]<<" yellow card(s) and "<<card[2]<<" red card(s)."<<endl;
            t=sh.cutTagByName("div");
        }
    isFirstTime=false;
    //cout<<"End of get data"<<endl;
    return true;
}
int main(int argc, char** argv)
{
    //int iMatch;
    if (argc>1)
    {
        if (argc>2) DEBUG = true;
        init_mysql_conf();
        init_mysql();

        mid = atoi(argv[1]);
        getMatch(mid);
        while (status<4)
        {
            getMatch(mid);
            sleep(3);
        }
    }
    else
    {
        cout<<"Usage: gmatch <match_id> "<<endl;
        cout<<"by Tran Huu Nam, huunam0@gmail.com, 2012"<<endl;
    }
}
