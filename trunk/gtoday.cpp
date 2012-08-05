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
static char f_home  [BUFFER_SIZE];
static int port_number;
static int DEBUG=1;

static MYSQL *conn;

string  teamname,group;
int iMinute;
int matchid;
string sToday;
struct Fmatch
{
    int mid;
    string league;
    string group;
    int hteam, ateam;
    int status;
    int hgoal, agoal;
};
const int mmax=150;
Fmatch matchs[mmax];
int mm;
int iNo=0;
bool isFirstTime = true;
int sleep_time = 10;
int day, month, year, day2, month2, year2;
string momment;
int stat0,stat1;
bool bEOM;

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
	sprintf(buffer,"/var/log/footygoat/today.log");
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
            read_buf(buf , "F_HOME", f_home);
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
void getTable(string sLeague)
{
    //cout<<"Get table "<<sLeague<<endl;
    char cmd[200];
    sprintf(cmd,"%sgtable %s &",f_home,sLeague.c_str());
    cout<<cmd<<endl;
    system(cmd);
}
void addLeague(string lid, string lname)
{
    char sql[500];
    sprintf(sql,"INSERT IGNORE INTO f_leagues (`league_id`,`league_name`) VALUE ('%s','%s')",lid.c_str(),lname.c_str());
    cout<<sql<<endl;
    executesql(sql);
    getTable(lid);
}
void addMatch(int mid, string league,string group,int hteam, int ateam,int status=0, int hscore=0, int ascore=0)
{
    char sql[500];
    sprintf(sql,"INSERT IGNORE INTO f_matches (match_id,league_id,`group`,hteam,ateam,status,hgoals,agoals,`order`,`match_date`) VALUE ('%d','%s','%s','%d','%d','%d','%d','%d',%d,'%d-%d-%d %s') ON DUPLICATE KEY UPDATE hteam=%d,ateam=%d",mid,league.c_str(),group.c_str(),hteam,ateam,status,hscore,ascore,iNo,year2,month2,day2,momment.c_str(),hteam,ateam);
    cout<<sql<<endl;
    executesql(sql);

}
void addMatch(int iIndex)
{
    char sql[500];
    sprintf(sql,"INSERT IGNORE INTO f_matches (match_id,league_id,`group`,hteam,ateam,status,hgoals,agoals,`order`,`match_date`) VALUE ('%d','%s','%s','%d','%d','%d','%d','%d',%d,'%d-%d-%d %s') ON DUPLICATE KEY UPDATE hteam=%d,ateam=%d",matchs[iIndex].mid,matchs[iIndex].league.c_str(),matchs[iIndex].group.c_str(),matchs[iIndex].hteam,matchs[iIndex].ateam,matchs[iIndex].status,matchs[iIndex].hgoal,matchs[iIndex].agoal,iIndex,year2,month2,day2,momment.c_str(),matchs[iIndex].hteam,matchs[iIndex].ateam);
    cout<<iIndex<<"::>"<<sql<<endl;
    executesql(sql);

}
void addTeam(int teamid, string tname, string league,string group)
{

}
int parseStatus(string status)
{
    shtml sh(status);
    momment="00:00";
    day2=day;
    month2=month;
    year2=year;
    if (sh.contain("AET"))
    {
        return 8;
    }
    else if (sh.contain("FT-Pens"))
    {
        return 9;
    }
    else if (sh.contain("FT"))
    {
        return 7;
    }
    else if (sh.contain("HT"))
    {
        return 2;
    }
    else if (sh.contain("2nd"))
    {
        return 3;
    }
    else if (sh.contain("1st"))
    {
        return 1;
    }
    else if (sh.contain(":"))
    {
        if (isFirstTime)
        {
            int v=status.find(":");
            momment=status.substr(v-2,5);
            if (sh.contain(","))
            {
                if (bEOM)
                {
                    day2=1;
                    month2++;
                    if (month2>12)
                    {
                        month2=1;
                        year2++;
                    }
                }
                else
                {
                    day2++;
                }


            }
        }

        return 0;
    }
    else if (sh.contain("'"))
    {
        sh.replace("'","");
        iMinute = sh.toInt();
        int v;
        if (iMinute<46)
        {
            v=1;
        }
        else if (iMinute<91)
        {
            v=3;
        }
        else
        {
            v=4;
        }
        //if (sh.contain("+")) v++;
        return v;
    }
    else
    {
        return 0;
    }
}
void parseDate(string sDate)
{
    //cout<<"Parse "<<sDate;
    shtml sh(sDate);
    shtml st(sh.cutBetween(",",","));
    if (st.contain("Jan"))
    {
        month=1;
    }
    else if (st.contain("Feb"))
    {
        month=2;
    }
    else if (st.contain("Mar"))
    {
        month=3;
    }
    else if (st.contain("Apr"))
    {
        month=4;
    }
    else if (st.contain("May"))
    {
        month=5;
    }
    else if (st.contain("Jun"))
    {
        month=6;
    }
    else if (st.contain("Jul"))
    {
        month=7;
    }
    else if (st.contain("Aug"))
    {
        month=8;
    }
    else if (st.contain("Sep"))
    {
        month=9;
    }
    else if (st.contain("Oct"))
    {
        month=10;
    }
    else if (st.contain("Nov"))
    {
        month=11;
    }
    else if (st.contain("Dec"))
    {
        month=12;
    }
    //cout<<", month:"<<month;
    st.deleteTo(" ",2);
    day = st.toInt();
    //cout<<", day:"<<day;
    sh.deleteTo(" ");
    year = sh.toInt();
    //cout<<", year:"<<year<<endl;
}
void getMatch(int mId)
{
    char cmd[200];
    sprintf(cmd,"%sgmatch %d &",f_home,mId);
    cout<<cmd<<endl;
    system(cmd);

}
void getToday(string sDay="")
{
    shtml m,t,n,nh;
    string status,hteam,ateam,score,league,gid,group,hname,aname, cday;
    int v, iStatus, hscore, ascore;

    //m.loadfromfile("scores2.htm");
    if (sDay.empty())
    {
        m.loadFromURL("http://soccernet.espn.go.com/scores?cc=4716");
    }
    else
    {
        m.loadFromURL((string("http://soccernet.espn.go.com/scores?date=")+sDay+string("&cc=4716&league=all")).c_str());
    }
    m.removeBetween("<!--","-->",-1);
    t=m.cutTagByName("div");
    iNo=0;
    stat0=stat1=0;
    while (!t.isEmpty())
    {
        if (t.containAttr("bg-elements"))
        {
            //sh.setContent(t.getContent());
            m=t;
            break;
        }
        if (m.isEmpty()) break;
        t=m.cutTagByName("div");
    }
    //sh.viewContent();
    cday=m.getBetween("id=\"currentDate\"","</ul>");
    t.setContent(cday);
    cday=t.getBetween(">","<");
    if (!isFirstTime)
    {
        //cout<<cday<<" ::: "<<sToday<<endl;
        isFirstTime=(cday!=sToday);
    }
    sToday=cday;
    if (isFirstTime)
    {
        t.deleteTo("<");
        t.retainBetween("\"","\"");
        bEOM = t.contain(" 1 ");
        parseDate(cday);
    }
    //cout<<"Today is "<<cday<<" End of month:"<<bEOM<<" - "<<day<<month<<year<<endl;

    m.retainTagByName("div",2);
    m.retainTagByName("div");
    m.retainTagByName("div",2);
    m.retainTagByName("div");
    m.retainTagByName("div");
    t=m.cutTagByName("div");
    while (!t.isEmpty())
    {
        if (t.containAttr("group-set"))
        {
            if (isFirstTime)
            {
                gid.clear();
                group.clear();
                n=t.getTagByName("a");
                nh.setContent(n.getAttr());
                nh.retainBetween("league","\"");
                if (nh.contain("/"))
                {
                    nh.retainBetween("/","/");
                }
                else
                {
                    nh.deleteTo("=");
                }
                //n  = t.getTagByOrder(1);
                //if (DEBUG)cout<<"League: "<<nh.getContent()<<" : "<<n.getText()<<endl;
                league=nh.getContent();
                addLeague(league,n.getText());
            }
            t.retainTagByName("table");
            n=t.cutTagByName("tr");
            while (!n.isEmpty())
            {

                if (n.containAttr("gamebox"))
                {
                    //cout<<n.getAttr()<<endl;
                    matchid = atoi(n.getBetweenAttr("id=\"","-").c_str());
                    nh=n.cutTagByName("td");
                    status=nh.getText();
                    iStatus=parseStatus(status);
                    if (iStatus==0) stat0++;
                    if (iStatus<7) stat1++;
                    if (isFirstTime)
                    {
                        nh=n.cutTagByName("td");
                        hname=nh.getText();
                        nh.setContent(nh.getAttr());
                        hteam=nh.getBetween("teamId-","\"");
                        nh=n.cutTagByName("td");
                        nh.replace("&nbsp;","",-1);
                        hscore = nh.toInt();
                        nh.deleteTo("-");
                        ascore = nh.toInt();
                        nh=n.cutTagByName("td");
                        aname=nh.getText();
                        nh.setContent(nh.getAttr());
                        ateam=nh.getBetween("teamId-","\"");
                        //cout<<iNo<<" Status "<<status<<"/"<<iStatus<<". Home:"<<hteam<<". Score:"<<hscore<<ascore<<". Away:"<<ateam<<endl;
                        matchs[iNo].mid=matchid;
                        matchs[iNo].league=league;
                        matchs[iNo].group=group;
                        matchs[iNo].hteam=atoi(hteam.c_str());
                        matchs[iNo].ateam=atoi(ateam.c_str());
                        matchs[iNo].hgoal=hscore;
                        matchs[iNo].agoal=ascore;
                        addMatch(iNo);
                        if (iStatus>0) getMatch(matchid);
                        //cout<<iNo<<" id="<<matchid<<", league="<<league<<", group="<<group<<", hteam="<<hteam<<", ateam="<<ateam<<", hscore="<<hscore<<", ascore="<<ascore<<", Satus="<<iStatus<<endl;
                    }
                    else
                    {
                        if ((iStatus>1) && (matchs[iNo].status==1))
                        {
                            getMatch(matchs[iNo].mid);
                        }
                    }

                    iNo++;
                }
                else if (isFirstTime)
                {
                    nh=n.getTagByName("a");
                    //gid=nh.getAttr();
                    group=nh.getText();
                    group=group.substr(group.length()-1);
                    nh.setContent(nh.getAttr());
                    gid=nh.getBetween("/groupId/","\"");
                    //cout<<"Group "<<gid<<":"<<group<<endl;
                }
                n=t.cutTagByName("tr");
            }
        }

        t=m.cutTagByName("div");
    }
    //m.viewContent();
}
void deleteTimeline()
{
    char sql[]="delete  from `f_timeline`;";
    executesql(sql);
}
int main(int argc, char** argv)
{
    string sDate;
    if (argc>1) sDate=string(argv[1]);
    init_mysql_conf();
    init_mysql();
    while (true)
    {
        //cout<<"Begin get list of match "<<endl;
        getToday(sDate);
        isFirstTime=false;
        sleep_time=(stat0==0?3600:10);
        cout<<"wait "<<sleep_time<<"seconds..."<<endl;
        if (stat1==0)
        {
            deleteTimeline();
        }
        sleep(sleep_time);
    }

    return 0;
}
