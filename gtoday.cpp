#include <time.h>
#include <mysql/mysql.h>
#include <string.h>
#include "shtml.h"
#include <stdarg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>

#define BUFFER_SIZE 1024
static char host_name[BUFFER_SIZE];
static char user_name[BUFFER_SIZE];
static char password [BUFFER_SIZE];
static char db_name  [BUFFER_SIZE];
static char f_home  [BUFFER_SIZE];
static int port_number;
static bool DEBUG=false;
static bool RELOAD=false;
#define LOCKFILE "/var/run/footygoat.pid"
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
static MYSQL *conn;

string  teamname,group;
int iMinute;
int matchid;
string sToday;
string sDate;
struct Fmatch
{
    int mid;
    string league;
    string group;
    int hteam, ateam;
    int status;
    int hgoal, agoal;
};
const int mmax=300;
Fmatch matchs[mmax];
int mm;
int iNo=0;
bool isFirstTime = true, isNewDay=false;
bool isTimeLineFull=true;
int sleep_time = 8;
int day, month, year, day2, month2, year2;
string momment,match_date;//moment of a match, date of match
int stat0,stat1;
bool bEOM;//end of month
char currentdate[20];
bool liveTable=false;
//For daemon start:
static bool STOP=false;
void call_for_exit(int s)
{
   STOP=true;
   printf("Stopping footygoat...\n");
}
//Doc file cau hinh
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

	sprintf(buffer,"/var/log/footygoat/today.log");
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
void write_log_call(const char *fmt, ...)
{
	va_list         ap;
	char            buffer[4096];
	char times[20];
    struct tm *sTm;
    time_t now = time (0);
    sTm = gmtime (&now);
    strftime (times, sizeof(times), "%Y-%m-%d %H:%M:%S", sTm);

	sprintf(buffer,"/var/log/footygoat/calls.log");
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
//Khoi tao mysql
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
	if (mysql_real_query(conn,sql,strlen(sql)))
    {
		write_log("Error in sql %s:%s",sql,mysql_error(conn));
		sleep(20);
		conn=NULL;
		return false;
	}
	else
	    return true;
}

bool init_mysql(bool bForce = false) {
    if((conn==NULL) || (bForce) )
    {
		conn=mysql_init(NULL);		// init the database connection
		/* connect the database */
		const char timeout=50;
		mysql_options(conn,MYSQL_OPT_CONNECT_TIMEOUT,&timeout);

		if(!mysql_real_connect(conn,host_name,user_name,password,db_name,port_number,0,0))
        {
			write_log("Error init mysql with host=%s,user=%s,pass=%s,db=%s: %s",host_name,user_name,password,db_name,mysql_error(conn));
			sleep(20);
			return false;
		}
	}
	if (!executesql("set names utf8"))
    {
		if(!bForce)
        {
			return init_mysql(true);
		}
		else
            return false;
    }

	return true;
}
//Xu li chinh
void getTable(string sLeague, bool isLive = false)
{
    //if (DEBUG) cout<<"Get table "<<sLeague<<endl;
    if (sLeague.empty()) return;
    char cmd[500];
    write_log_call("Get table %s ",sLeague.c_str());
    sprintf(cmd,"%sg%stable %s &",f_home,(isLive?"live":""),sLeague.c_str());
    system(cmd);
}
void getTable(string sLeague)
{
    //if (DEBUG) cout<<"Get table "<<sLeague<<endl;
    if (sLeague.empty()) return;
    char cmd[500];
    write_log_call("Get table %s ",sLeague.c_str());
    sprintf(cmd,"%sgtable %s &",f_home,sLeague.c_str());
    system(cmd);
}
//from june 2014, add slug
void getTable(const string sLeague_id, const string sLeague_slug)
{
    //if (DEBUG) cout<<"Get table "<<sLeague<<endl;
    if (sLeague_id.empty()) return;
    char cmd[500];
    write_log_call("Get table %s ",sLeague_id.c_str());
    sprintf(cmd,"%sgtable %s %s &",f_home,sLeague_id.c_str(), sLeague_slug.c_str());
    system(cmd);
}
void addLeague(string lid, string lname,string lslug)
{
    char sql[1000];
    sprintf(sql,"INSERT IGNORE INTO f_leagues2 (`league_id`,`league_slug`,`league_name`) VALUE ('%s','%s','%s')",lid.c_str(),lslug.c_str(),lname.c_str());
    //if (DEBUG) cout<<"Add league "<<lid<<" / "<<lname<<endl;
    executesql(sql);
    getTable(lid,lslug);
}
void addMatch(int mid, string league,string group,int hteam, int ateam,int status=0, int hscore=0, int ascore=0)
{
    char sql[1000];
    sprintf(sql,"INSERT INTO f_matches (match_id,league_id,`group`,hteam,ateam,status,hgoals,agoals,`order`,`match_date`,`viewdate`) VALUE ('%d','%s','%s','%d','%d','%d','%d','%d',%d,'%d-%d-%d %s','%s') ON DUPLICATE KEY UPDATE hteam=%d,ateam=%d,viewdate='%s',`order`=%d",mid,league.c_str(),group.c_str(),hteam,ateam,status,hscore,ascore,iNo,year2,month2,day2,momment.c_str(),currentdate,hteam,ateam,currentdate,iNo);
    ///cout<<sql<<endl;
    executesql(sql);

}
void addMatch(int mid, string league,string group,int status=0)
{
    char sql[1000];
    sprintf(sql,"INSERT INTO f_matches (match_id,league_id,`group`,status,`order`,`match_date`,`viewdate`) VALUE ('%d','%s','%s','%d',%d,'%s','%s') ON DUPLICATE KEY UPDATE viewdate='%s',`order`=%d",mid,league.c_str(),group.c_str(),status,iNo,match_date.c_str(),currentdate,currentdate,iNo);
    //cout<<sql<<endl;
    executesql(sql);

}

void addTeam(string teamid, string tname, string league,string group)
{
    char sql[1000];
    sprintf(sql,"INSERT INTO f_teams (team_id,team_name,team_league,team_group,team_date,team_updated) VALUE (%s,'%s','%s','%s',NOW(),0) ON DUPLICATE KEY UPDATE team_name='%s',team_league='%s',team_group='%s',team_date=NOW(),team_updated=0;",teamid.c_str(),tname.c_str(),league.c_str(),group.c_str(),tname.c_str(),league.c_str(),group.c_str());
    //write_log("Add new team %d %s",teamid,tname.c_str());
    executesql(sql);
}

int parseStatus(string status)
{
    shtml sh(status);
    momment="07:00";
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
    else if (sh.contain("Postp"))
    {
        return 11;
    }
    else if (sh.contain("Susp"))
    {
        return 6;
    }
    else if (sh.contain("Aban"))
    {
        return 10;
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
    else
    {
        month=0;
    }
    //cout<<", month:"<<month;
    st.deleteTo(" ",2);
    day = st.toInt();
    //cout<<", day:"<<day;
    sh.deleteTo(" ");
    year = sh.toInt();
    //cout<<", year:"<<year<<endl;
    sprintf(currentdate,"%d-%s%d-%s%d",year,(month<=9?"0":""),month,day<=9?"0":"",day);
}
void parseDate8(string sDate)
{
    //cout<<"Parse "<<sDate;
    string p;
    p=sDate.substr(0,4);
    year = atoi(p.c_str());
    p=sDate.substr(2,2);
    month=atoi(p.c_str());
    p=sDate.substr(4,2);
    day=atoi(p.c_str());
    //cout<<", day:"<<day;
    sprintf(currentdate,"%d-%s%d-%s%d",year,(month<=9?"0":""),month,day<=9?"0":"",day);
}
void getMatch(int mId)
{
    if (mId<=0) return;
    char cmd[500];
    write_log_call("Get Match %d ",mId);
    sprintf(cmd,"%sgmatch %d &",f_home,mId);
    //cout<<cmd<<endl;
    system(cmd);
    RELOAD=true;
}
void setEvent(int iEvent,int iValue=0)
{
    char sql[500];
    //sprintf(sql,"INSERT IGNORE INTO f_timeline (`event`, `value`, `team`, `match`, `date`) VALUE (%d,%d,%d,%d,NOW())",iEvent,iValue,0,mid);
    sprintf(sql,"INSERT INTO f_timeline (`event`, `value`, `team`, `match`, `date`) VALUE (%d,%d,%d,%d,NOW()) ON DUPLICATE KEY UPDATE `value`=%d,`date`=NOW();",iEvent,iValue,0,0,iValue);
    executesql(sql);

}
void setEvent2(int iEvent,int iValue0=0,int iValue1=0)
{
    char sql[500];
    //sprintf(sql,"INSERT IGNORE INTO f_timeline (`event`, `value`, `team`, `match`, `date`) VALUE (%d,%d,%d,%d,NOW())",iEvent,iValue,0,mid);
    sprintf(sql,"INSERT INTO f_timeline2 (`event`, `home`, `away`, `match`, `date`) VALUE (%d,%d,%d,%d,NOW()) ON DUPLICATE KEY UPDATE `home`=%d,`away`=%d,`date`=NOW();",iEvent,iValue0,iValue1,0,iValue0,iValue1);
    executesql(sql);

}
void deleteTimeline()
{
    write_log_call("Empty timeline...");
    char sql[]="delete  from `f_timeline2` where `event` < 100;";
    executesql(sql);
    char sql2[]="ALTER TABLE f_timeline2 AUTO_INCREMENT = 2;";
    executesql(sql2);
}
void setCurrentDate(string refDate="")
{
    char sql[500];
    sprintf(sql,"update f_params set p_value='%s' where p_name='currentdate';",currentdate);
    executesql(sql);
    write_log_call(" New day %s -> %s",currentdate,refDate.c_str());
}
void getToday(string sDay="")
{
    shtml m,t,n,nh;
    string status,hteam,ateam,score,league,league_slug,league_name,gid,group,hname,aname, cday;
    int v, iStatus, hscore, ascore;
    bool noId,bRet;
    int homeid,awayid;

    //m.loadfromfile("scores2.htm");
    stat1=100;
    iNo=0;
    if (sDay.empty())
    {
        bRet=m.loadFromURL("http://www.espnfc.com/scores");
    }
    else
    {
        bRet=m.loadFromURL((string("http://www.espnfc.com/scores?date=")+sDay).c_str());
    }
    if (!bRet)
    {
        isFirstTime=true;
        return;
    }
    m.removeBetween("<!--","-->",-1);
    t=m.cutTagByName("div");

    stat0=stat1=0;
    while (!t.isEmpty())
    {
        if (t.containAttr("site"))
        {
            //sh.setContent(t.getContent());
            m=t;
            break;
        }
        if (m.isEmpty()) break;
        t=m.cutTagByName("div");
    }
    //sh.viewContent();
    m.retainTagByName("div");//id=site-content
    m.retainTagByName("div",3);//id=main
    m.retainTagByName("div");//id=main-content
    m.retainTagByName("div",2);
    m.retainTagByName("div");
    m.retainTagByName("div");//id=score-page
    m.retainTagByName("div");//id=scores-page-content
    //m.viewContent();//debug
    t=m.cutTagByName("div");
    n=t.cutTagByName("li");
    while (!n.isEmpty())
    {
        if (n.containAttr("selected"))
        {
            nh=n;
            break;
        }
        if (t.isEmpty()) break;
        n=t.cutTagByName("li");
    }
    //nh.viewContent();//for debug
    cday=nh.getBetween("date=","\"");
    //t.setContent(cday);
    //cday=t.getBetween(">","<");
    if (DEBUG) cout<<cday<<" ::: "<<sToday<<endl;
    //return;//debug
    if (!isFirstTime)
    {
        if (cday != sToday)
        {
            isFirstTime = true;
            sToday=cday;
        }
        //if (isFirstTime)
    }

    if (isFirstTime)
    {
        //
        //t.deleteTo("<");
        //t.retainBetween("\"","\"");
        //bEOM = t.contain(" 1 ");
        parseDate8(cday);
        //deleteTimeline();
        if ((day==0)||(month==0)||(year==0))
        {
            write_log("errorindate %s",cday.c_str());
            return;
        }
        setCurrentDate(cday);
    }
    //cout<<"Today is "<<cday<<" End of month:"<<bEOM<<" - "<<day<<month<<year<<endl;
    m.retainTagByName("div",2);
    //m.viewContent();return ;//debug
    isNewDay=false;
    t=m.cutTagByName("div"); //each league
    while (!t.isEmpty())
    {

        if (t.containAttr("score-league"))
        {
            //if (DEBUG) cout<<"Begin get league "<<endl;
            if (isFirstTime) //get league id, name, slug
            {
                gid.clear();
                group.clear();
                n=t.cutTagByName("a");
                liveTable=n.containAttr("score-table-link");
                nh.setContent(n.getAttr());
                nh.retainBetween("href=\"","\"");
                //nh.viewContent();
                int shift = 0;
                if (nh.contain("http://www.espnfc.com/"))
                    shift=2;
                league_slug=nh.getBetween("/","/",shift+1);
                league=nh.getBetween("/","/",shift+2); //id
                //league=nh.getContent();
                if (liveTable)
                {
                    n=t.cutTagByName("a");
                }
                n.replace("'","''");
                league_name=n.getContent();
            }

            n=t.cutTagByName("div");//each group //if any
            while (!n.isEmpty())
            {
                //if (DEBUG) cout<<"Begin get match "<<endl;
                if (n.containAttr("score-group"))
                {
                    //cout<<n.getAttr()<<endl;
                    group=n.getBetween("<p>Group ","</p>");
                    nh=n.cutTagByName("div");
                    while (!nh.isEmpty()) //each match
                    {
                        if (nh.containAttr("score-box"))
                        {
                            nh.retainTagByName("div");
                            gid=nh.getBetween("data-gameId=\"","\"");
                            matchid=atoi(gid.c_str());
                            if (matchid!=matchs[iNo].mid) isNewDay=true;
                            if (nh.containAttr("score complete full"))
                            {
                                iStatus=7;
                                //+Postp
                            }
                            else if (nh.containAttr("score upcoming full"))
                            {
                                iStatus=0;
                            }
                            if (iStatus==0) stat0++;
                            if ((iStatus<7)&&((iStatus>=0))) stat1++;
                            if (isFirstTime) //add teams
                            {
                                nh.retainTagByName("div");
                                nh.retainTagByName("div",3);
                                nh.replace("T"," ");
                                //nh.viewContent();
                                match_date=nh.getBetween("data-time=\"",".");
                                //match_date.replace(9,1," ");
                                matchs[iNo].status=iStatus;
                                matchs[iNo].mid=matchid;
                                matchs[iNo].league=league;
                                matchs[iNo].group=group;
                                //matchs[iNo].hteam=atoi(hteam.c_str());
                                //matchs[iNo].ateam=atoi(ateam.c_str());
                                //matchs[iNo].hgoal=hscore;
                                //matchs[iNo].agoal=ascore;

                                //addMatch(matchid,league,group,homeid,awayid,iStatus,hscore,ascore);
                                addMatch(matchid,league,group,iStatus);
                                if (iStatus>0) getMatch(matchid);
                                if (DEBUG) cout<<iNo<<" id="<<matchid<<", league="<<league<<", group="<<group<<", Satus="<<iStatus<<endl;
                            }
                            else
                            {
                                if ((iStatus>0) && (matchs[iNo].status<=0)) //begin match
                                {
                                    matchs[iNo].status=iStatus;
                                    getMatch(matchs[iNo].mid);
                                }
                                if ((iStatus>=7) && (matchs[iNo].status<7))// stop match
                                {
                                    matchs[iNo].status=iStatus;
                                    getTable(league,league_slug);
                                }
                            }

                            iNo++;
                            if (iNo>=mmax)
                            {
                                write_log_call("Over the maximum of matches");
                                return;
                            }
                        }
                        nh=n.cutTagByName("div");
                    }

                    nh=n.cutTagByName("td");
                    //status=nh.getText();
                    //iStatus=parseStatus(status);
                    //if (DEBUG) cout<<"Match "<<matchid<<" has status "<<iStatus<<endl;
                }
                n=t.cutTagByName("div");
            }
            if (isFirstTime)
            {
                addLeague(league,league_name,league_slug);
            }
            //if (DEBUG) cout<<"End get league "<<endl;
        }
        //if (DEBUG) cout<<"Get NEXT league "<<endl;
        t=m.cutTagByName("div");
    }
    //m.viewContent();
    isFirstTime = false;
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
void reload_ftrigger()
{
    system("service ftriggers reload");
}
void restart_ftrigger()
{
    system("service ftriggers restart");
}
int main(int argc, char** argv)
{
    bool bForce = false;
    if (argc>1) sDate=string(argv[1]);
    if (sDate.find("debug")!=string::npos)
    {
        DEBUG=true;
        if (argc>2)
            sDate=string(argv[2]);
        else
            sDate="";
    }
    else
    {
        if (argc>2) DEBUG=true;
    }
    sDate="";
    if (!DEBUG) daemon_init();
    if (already_running())
    {
        write_log("Footygoat is already running!");
        return 1;
    }
	init_mysql_conf();
    signal(SIGQUIT,call_for_exit);
	signal(SIGKILL,call_for_exit);
	signal(SIGTERM,call_for_exit);
	write_log_call("Starting...");
    while (!STOP)
    {
		if (init_mysql(bForce))
        {
            if (!isFirstTime)
            {
                isFirstTime=isNewDay;
            }
            if ((isFirstTime))
            {

                setEvent2(100);
                write_log_call("is First Time or new day");
                //restart_ftrigger();
            }
            getToday(sDate);
            break;//debug
            if (RELOAD)
            {
                RELOAD=false;
                //reload_ftrigger();
                restart_ftrigger();
            }
            //isFirstTime=false;
            bForce=(stat0==0);
            sleep_time=(bForce?3600:10);
            //cout<<"wait "<<sleep_time<<"seconds..."<<endl;
            if ((stat1==0)  )
            {
                if (isTimeLineFull)
                {
                    deleteTimeline();
                    isTimeLineFull=false;
                }
            }
            else
            {
                isTimeLineFull=true;
            }
        }
        sleep(sleep_time);
        //break;//for debug
    }
    write_log_call("Stoping...");
    return 0;
}
