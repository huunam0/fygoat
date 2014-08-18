#include <time.h>
#include <string.h>
#include "shtml.h"
#include <stdarg.h>

#define BUFFER_SIZE 1024
static char host_name[BUFFER_SIZE];
static char user_name[BUFFER_SIZE];
static char password [BUFFER_SIZE];
static char db_name  [BUFFER_SIZE];
static int DEBUG=false;

int m[2][10];
int status=0;
int mid=0;
bool isFirstTime=true;
int minutes=0;

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

	fp = fopen("/etc/footygoat/footygoat.conf", "r");
	if(fp!=NULL)
    {
        while (fgets(buf, BUFFER_SIZE - 1, fp))
        {
            read_buf(buf,"F_HOST_NAME",host_name);
            read_buf(buf, "F_USER_NAME",user_name);
            read_buf(buf, "F_PASSWORD",password);
            read_buf(buf, "F_DB_NAME",db_name);
        }
		return true;
	//	fclose(fp);
    }
    else
    {
        write_log("Cannot open file 'footygoat.conf'");
        return false;
    }
}

void executesql(const char * sql){
    char cmd[1000];
    sprintf(cmd,"mysql %s -u%s -p%s -s -N -e \"%s;\" &",db_name,user_name,password,sql);
    system(cmd);
}
//set Event to timeline and update f_matches
void emit2Event(const int iIndex,const int iValue0,const int iValue1)
{
    char sql[300];
    if (DEBUG)
    {
        cout<<"Event "<<iIndex<<" value: "<<iValue0<<" - "<<iValue1<<endl;
        return;
    }
    sprintf(sql,"INSERT INTO f_timeline2 (event, home, away, match_id) VALUE (%d,%d,%d,%d); ",iIndex,iValue0,iValue1,mid);
    executesql(sql);
    string field;
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
    case 8:
        field="minutes";
        break;
    case 11:
        field="team";
        break;
    }
    if (!field.empty())
    {
        if (iIndex!=8)
        {
            sprintf(sql,"UPDATE f_matches SET h%s=%d,a%s=%d where match_id=%d",field.c_str(),iValue0,field.c_str(),iValue1,mid);
        }
        else
        {
            sprintf(sql,"UPDATE f_matches SET status=%d,minutes=%d where match_id=%d",iValue0,iValue1,mid);
        }
        executesql(sql);
    }

}
//Set special event
void setEvent2(const int iEvent,const int iValue0=0,const int iValue1=0) //special events
{
    char sql[300];
    sprintf(sql,"INSERT INTO f_timeline2 (event, home, away, match_id) VALUE (%d,%d,%d,%d);",iEvent,iValue0,iValue1,mid);
    executesql(sql);

}
//Save values to array and emit event
void set2Value(const int iIndex, const int iValue0=0, const int iValue1=0)
{
    if (DEBUG)
    {
        cout<<"EMIT #"<<iIndex<<": "<<iValue0<<"-"<<iValue1<<endl;
        return;
    }
    if ( (isFirstTime) ||  ((m[0][iIndex]!=iValue0)) || (m[1][iIndex]!=iValue1))
    {
        m[0][iIndex]=iValue0;
        m[1][iIndex]=iValue1;
        emit2Event(iIndex,iValue0,iValue1);
    }
}

void parseStatus2(shtml stat)
{
    if (stat.contain("FT-Pens"))
    {
        status=9;
        minutes=90;
    }
    else if (stat.contain("FT"))
    {
        status=7;
        minutes=90;
    }
    else if (stat.contain("Postp"))//
    {
        status=11;
    }
    else if (stat.contain("Aban"))//
    {
        status=10;
    }
    else if (stat.contain("AET"))//
    {
        status=8;
    }
    else if (stat.contain("Susp"))//
    {
        status=6;
    }
    else if (stat.contain("2nd"))
    {
        status=3;
    }
    else if (stat.contain("HT"))
    {
        status=2;
        minutes=45;
    }
    else if (stat.contain("1st"))
    {
        status=1;
    }
    else if (stat.contain(":")||(stat.contain("nbsp")))//
    {
        status=0;
    }
    else
    {
        minutes=stat.toInt();
        if (minutes<46)
        {
            status=1;
        }
        else //if (minutes<91)
        {
            status=3;
        }
    }
}
void addTeam(const string teamid, const string tname, const string slug, const string league,const string group)
{
    char sql[1000];
    sprintf(sql,"INSERT IGNORE INTO f_teams (team_id,team_name,team_slug,team_league,team_group,team_date,team_updated) VALUE (%s,'%s','%s','%s','%s',NOW(),0) ON DUPLICATE KEY UPDATE team_name='%s',team_slug='%s',team_league='%s',team_group='%s',team_date=NOW(),team_updated=0;",teamid.c_str(),tname.c_str(),slug.c_str(),league.c_str(),group.c_str(),tname.c_str(),slug.c_str(),league.c_str(),group.c_str());
    executesql(sql);
}
//update home team, away team  of the match
void updateha(const int m_id,const string home_id,const string away_id)
{
    char sql[1000];
    sprintf(sql,"UPDATE f_matches set hteam=%s, ateam=%s where match_id=%d;",home_id.c_str(),away_id.c_str(),m_id);
    executesql(sql);

}
bool getMatch(const int id) //for major league
{
    shtml sh,t,n,nh,nh2;
    shtml tdh,tda;
    string hname, h_id, aname, a_id, slug,tmp;
    int v[2],w[2];
    int sc[2]={0,0};
    int sc1[2]={0,0};
    char url[300];
    sprintf(url,"http://www.espnfc.com/gamecast/statistics/id/%d/statistics.html",id);
    if (DEBUG)
        cout<<"Loading from "<<url<<endl;
    if(!sh.loadFromURL(url))
    {
        if (DEBUG) cout<<"Cannot load from "<<url<<endl;
        return false;
    }
    if (sh.isEmpty()) return false;
    /*Remove comments in html*/
    sh.removeBetween("<!--","-->",-1);
    /* Remove odd parts*/
    sh.retainTagByName("section");
    sh.retainTagByName("div");
    sh.retainTagByName("div");//main-content

    t=sh.cutTagByName("section");
    t.retainTagByName("section");
    t.retainTagByName("div",3);
    t.removeTagByName("div");
    if (isFirstTime) //get teams
    {
        n=t.cutTagByName("div");//home team
        h_id=n.getBetweenAttr("teamId-","\"");
        n.retainTagByName("p");
        n.retainTagByName("a");
        slug = n.getBetweenAttr("/","/",2);
        slug.erase(remove(slug.begin(),slug.end(),'\''),slug.end());
        n.replace("'","\\'",-1);
        hname=n.getContent();
        //n.setContent(slug);
        //slug=n.getBetween()
        addTeam(h_id,hname,slug,"","");
        n=t.cutTagByName("div");//score & time


        t.retainTagByName("div");//away team
        a_id=t.getBetweenAttr("teamId-","\"");
        t.retainTagByName("p");
        t.retainTagByName("a");
        slug = n.getBetweenAttr("/","/",2);
        slug.erase(remove(slug.begin(),slug.end(),'\''),slug.end());
        n.replace("'","\\'",-1);
        aname=t.getContent();
        addTeam(a_id,aname,slug,"","");
        updateha(id,h_id,a_id);
        //emit2Event(11,atoi(h_id.c_str()),atoi(a_id.c_str()));
    }
    else //only get time & status
    {
        n=t.cutTagByName("div",2);
    }
    n.retainTagByName("div");

    nh=n.cutTagByName("p");//score
    nh.trim();
    sc[0]=nh.toInt();
    if (nh.contain("v"))//has not start
    {
        status=0;
    }
    else
    {
        nh.deleteTo("-");
        nh.trim();
        sc[1]=nh.toInt();
        status=1;
    }
    nh=n.cutTagByName("p");
    nh.trim();
    if (status>0)
    parseStatus2(nh);

    if (status==0)
    {
        return false;
    }
    set2Value(8,status,minutes);
    if (status==1) //first half
    {
        sc1[0]=sc[0];
        sc1[1]=sc[1];
    }
    set2Value(0,sc[0],sc[1]);
    set2Value(1,sc1[0],sc1[1]);

    t=sh.cutTagByName("section");//omit
    t=sh.cutTagByName("section");
    if (t.containAttr("gc-stat-list"))
    {
        t=sh.cutTagByName("div");
        t=sh.cutTagByName("div");
        n=t.cutTagByName("section");//sumary score
        n=t.cutTagByName("section");//match stats
        n.retainTagByName("table");
        nh=n.cutTagByName("tr");
        while (!nh.isEmpty())
        {
            tdh=nh.cutTagByName("td");
            t=nh.cutTagByName("td");
            tda=nh.cutTagByName("td");
            v[0]=tdh.toInt();
            v[1]=tda.toInt();
            if (t.contain("Shots (on goal)"))
            {
                set2Value(4,v[0],v[1]);
                tmp=tdh.getBetween("(",")");
                w[0]=atoi(tmp.c_str());
                tmp=tda.getBetween("(",")");
                w[1]=atoi(tmp.c_str());
                set2Value(5,w[0],w[1]);
            }
            else if (t.contain("Corner kicks"))
            {
                set2Value(6,v[0],v[1]);
            }
            else if (t.contain("Time of Possession"))
            {
                set2Value(7,v[0],v[1]);
            }
            else if (t.contain("Yellow Cards"))
            {
                set2Value(3,v[0],v[1]);
            }
            else if (t.contain("Red Cards"))
            {
                set2Value(2,v[0],v[1]);
            }

            nh=n.cutTagByName("tr");
        }
    }
    else if (t.containAttr("mod-tabs"))
    {
        t.retainTagByName("div");
        t.retainTagByName("div"); //#matchstats
        n=t.cutTagByName("section");
        while (!n.isEmpty())
        {
            if (n.contain("Yellow Cards") && n.contain("Red Cards"))
            {
                nh=n.cutTagByName("div");
                v[0]=nh.count("soccer-icons-yellowcard");
                nh=n.cutTagByName("div");
                v[1]=nh.count("soccer-icons-yellowcard");
                set2Value(3,v[0],v[1]);
                nh=n.cutTagByName("div");
                w[0]=nh.count("soccer-icons-redcard");
                nh=n.cutTagByName("div");
                w[1]=nh.count("soccer-icons-redcard");
                set2Value(2,w[0],w[1]);
            }
            n=t.cutTagByName("section");
        }
    }
    else
    {
        v[0]=t.count("soccer-icons-yellowcard");
        w[0]=t.count("soccer-icons-redcard");
        t=sh.cutTagByName("section");
        v[1]=t.count("soccer-icons-yellowcard");
        w[1]=t.count("soccer-icons-redcard");
        set2Value(3,v[0],v[1]);
        set2Value(2,w[0],w[1]);
    }
/*
    else
    {
        t.retainTagByName("div");
        t.retainTagByName("div");
        n=t.cutTagByName("section");//score sumary
        //process - not enought infos
        n=t.cutTagByName("section");//omit
        n=t.cutTagByName("section");//omit
        n=t.cutTagByName("section");//yellow & red card(s)
        nh=n.cutTagByName("div"); // home-yellow
        //nh.viewContent();
        v[0]=nh.count("soccer-icons-yellowcard");
        nh=n.cutTagByName("div");
        v[1]=nh.count("soccer-icons-yellowcard");
        if ((v[0]>0) || (v[1]>0)) set2Value(3,v[0],v[1]);
        nh=n.cutTagByName("div"); // home-red
        v[0]=nh.count("soccer-icons-redcard");
        nh=n.cutTagByName("div");
        v[1]=nh.count("soccer-icons-redcard");
        if ((v[0]>0) || (v[1]>0)) set2Value(2,v[0],v[1]);
    }
*/
    isFirstTime=false;
    //cout<<"End of get data"<<endl;
    return true;
}
void getTable(const string sLeague)
{
    if (DEBUG) cout<<"Get table (X) "<<sLeague<<endl;
}
void deleteTimeline(const int maid)
{
    write_log("Empty timeline for %d",maid);
    char sql[200];
    sprintf(sql, "delete  from f_timeline2 where match_id='%d';",maid);
    executesql(sql);
}
void killSame(const int sid)
{
    char cmd[200];
    //write_log_call("Get Match %d ",mId);
    sprintf(cmd,"kill `ps -ef | grep 'gmatch %d' | grep -v grep | awk '{print $2}'` &",sid);
    //sprintf(cmd,"kill `ps -ef | grep 'gmatch \+%d' | grep -v grep | awk '{print $2}'` &",sid);
    //cout<<cmd<<endl;
    //system(cmd);
}
int main(int argc, char** argv)
{
    if (argc>1)
    {
        int kt=0;
        for (int i=2; i<argc; i++)
        {
            if (strcmp(argv[i],"debug")==0)
            {
                DEBUG=true;
                break;
            }
        }
        init_mysql_conf();
        mid = atoi(argv[1]);
        killSame(mid);
        deleteTimeline(mid);
        getMatch(mid);
        if (status>0) {
            setEvent2(10);
            while (status<7)
            {

                if (!getMatch(mid)) break;
                sleep(5);
                kt++;
                if (kt>5400)
                {
                    write_log("Overtime match %d:%d times",mid,kt);
                    break;
                }
            }
        }
        if (status>=7)
        {
            deleteTimeline(mid);
            //setEvent2(12,status);
        }
    }
    else
    {
        cout<<"Usage: gmatch <match_id> "<<endl;
        cout<<"by Tran Huu Nam, huunam0@gmail.com, 2012"<<endl;
    }
}
