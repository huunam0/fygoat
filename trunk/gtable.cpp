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
static bool DEBUG=false;

static MYSQL *conn;

string teamid, teamname,group,team_slug;
string tleague;
int pos,play,hw,hd,hl,aw,ad,al,ow,od,ol,tabletype;
int hf,ha,af,aa,of,oa;
int gd,pts;
string teamlistname="'0'";

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

	sprintf(buffer,"/var/log/footygoat/table.log");
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
    if (DEBUG)
    {
        cout<<sql<<endl;
        return true;
    }

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
void addTeam(string tid, string tname, int type)
{
    char sql[1000];
    if (tid.empty())
    {
		//return;
		if (type)
        {
            sprintf(sql,"Update f_teams set team_pos=%d,`team_op`=%d,`team_ow`=%d,`team_od`=%d,`team_ol`=%d,`team_of`=%d,`team_oa`=%d,`team_hw`=%d,`team_hd`=%d,`team_hl`=%d,`team_hf`=%d,`team_ha`=%d,`team_aw`=%d,`team_ad`=%d,`team_al`=%d,`team_af`=%d,`team_aa`=%d,`team_gd`=%d,`team_pts`=%d,team_updated=1,team_date=NOW() where team_league='%s' and team_name='%s'",pos,play,ow,od,ol,of,oa,hw,hd,hl,hf,ha,aw,ad,al,af,aa,gd,pts,tleague.c_str(),tname.c_str());
        }
        else
        {
            sprintf(sql,"Update f_teams set `team_pos`=%d,`team_op`=%d,`team_ow`=%d,`team_od`=%d,`team_ol`=%d,`team_of`=%d,`team_oa`=%d,`team_gd`=%d,`team_pts`=%d,team_updated=1,team_date=NOW() where team_league='%s' and team_name='%s'",pos,play,ow,od,ol,of,oa,gd,pts,tleague.c_str(),tname.c_str());
        }
        teamlistname+=",'"+tname+"'";
    }
    else
    {
        if (type)
        {
            sprintf(sql,"INSERT INTO `f_teams` (`team_id`,`team_name`,`team_group`,`team_league`,`team_pos`,`team_op`,`team_ow`,`team_od`,`team_ol`,`team_of`,`team_oa`,`team_hw`,`team_hd`,`team_hl`,`team_hf`,`team_ha`,`team_aw`,`team_ad`,`team_al`,`team_af`,`team_aa`,`team_gd`,`team_pts`,team_date,team_updated) VALUES ('%s','%s','%s','%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d',NOW(),1) ON DUPLICATE KEY UPDATE team_group='%s',team_league='%s',team_pos='%d',team_op=%d,team_ow=%d,team_od=%d,team_ol=%d,team_of=%d,team_oa=%d,team_hw=%d,team_hd=%d,team_hl=%d,team_hf=%d,team_ha=%d,team_aw=%d,team_ad=%d,team_al=%d,team_af=%d,team_aa=%d,team_gd=%d,team_pts=%d,team_date=NOW(),team_updated=1;",tid.c_str(),tname.c_str(),group.c_str(),tleague.c_str(),pos,play,ow,od,ol,of,oa,hw,hd,hl,hf,ha,aw,ad,al,af,aa,gd,pts,group.c_str(),tleague.c_str(),pos,play,ow,od,ol,of,oa,hw,hd,hl,hf,ha,aw,ad,al,af,aa,gd,pts);
        }
        else
        {
            sprintf(sql,"INSERT INTO `f_teams` (`team_id`,`team_name`,`team_group`,`team_league`,`team_pos`,`team_op`,`team_ow`,`team_od`,`team_ol`,`team_of`,`team_oa`,`team_gd`,`team_pts`,team_date,team_updated) VALUES ('%s','%s','%s','%s','%d','%d','%d','%d','%d','%d','%d','%d','%d',NOW(),1) ON DUPLICATE KEY UPDATE team_group='%s',team_league='%s',team_pos='%d',team_op=%d,team_ow=%d,team_od=%d,team_ol=%d,team_of=%d,team_oa=%d,team_gd=%d,team_pts=%d,team_date=NOW(),team_update=1;",tid.c_str(),tname.c_str(),group.c_str(),tleague.c_str(),pos,play,ow,od,ol,of,oa,gd,pts,group.c_str(),tleague.c_str(),pos,play,ow,od,ol,of,oa,gd,pts);
        }
    }
    //cout<<"Chay sql"<<endl;
    //write_log(sql);
    executesql(sql);
}
void addTeam(const string tid, const string tname, const string tslug)
{
    char sql[1000];
    sprintf(sql,"INSERT INTO `f_teams` (`team_id`,`team_slug`,`team_name`,`team_group`,`team_league`,`team_pos`,`team_op`,`team_ow`,`team_od`,`team_ol`,`team_of`,`team_oa`,`team_hw`,`team_hd`,`team_hl`,`team_hf`,`team_ha`,`team_aw`,`team_ad`,`team_al`,`team_af`,`team_aa`,`team_gd`,`team_pts`,team_date,team_updated) VALUES ('%s','%s','%s','%s','%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d',NOW(),1) ON DUPLICATE KEY UPDATE team_group='%s',team_league='%s',team_pos='%d',team_op=%d,team_ow=%d,team_od=%d,team_ol=%d,team_of=%d,team_oa=%d,team_hw=%d,team_hd=%d,team_hl=%d,team_hf=%d,team_ha=%d,team_aw=%d,team_ad=%d,team_al=%d,team_af=%d,team_aa=%d,team_gd=%d,team_pts=%d,team_date=NOW(),team_updated=1;",tid.c_str(),tslug.c_str(),tname.c_str(),group.c_str(),tleague.c_str(),pos,play,ow,od,ol,of,oa,hw,hd,hl,hf,ha,aw,ad,al,af,aa,gd,pts,group.c_str(),tleague.c_str(),pos,play,ow,od,ol,of,oa,hw,hd,hl,hf,ha,aw,ad,al,af,aa,gd,pts);
    teamlistname+=",'"+tname+"'";
    executesql(sql);
}
void resetTable(string sLeague, string tids, string tnames)
{
    char sql[500];
    sprintf(sql,"update f_teams set team_updated=0,team_date=NOW() where (team_league='%s') and (team_id not in (%s)) and (team_name not in (%s))",sLeague.c_str(),tids.c_str(),tnames.c_str());
    executesql(sql);
}
void getTable(string sLeague) //sLeague includes slug & id
{
    shtml m,t,n,nh;
    string teamlist="0";
    tleague = sLeague;
    size_t ix = tleague.find("/");
    if (ix!=string::npos)
        tleague.erase(0,ix+1);
    //m.loadfromfile("http://www.espnfc.com/league-of-ireland-premier-division/116/table");
	if (!m.loadFromURL((string("http://www.espnfc.com/")+sLeague+string("/table")).c_str())) return;
    m.removeBetween("<!--","-->",-1);
    t=m.cutTagByName("div");
    while (!t.isEmpty())
    {
        if (t.containAttr("site"))
        {
            m=t;
            break;
        }
        if (m.isEmpty()) break;
        t=m.cutTagByName("div");
    }
    //sh.viewContent();
    m.retainTagByName("div");
    m.retainTagByName("div",3);
    m.retainTagByName("div");
    m.retainTagByName("div",2);
    m.retainTagByName("div");
    m.retainTagByName("div");
    m.retainTagByName("div");
    m.retainTagByName("div",4);
    m.retainTagByName("div");
    m.retainTagByName("div");
    m.retainTagByName("div",3);

    t=m.cutTagByName("div");
    while (!t.isEmpty())
    {
        if (t.containAttr("responsive-table")) //t=each group
        {
            t.retainTagByName("table");
            n=t.getTagByName("thead");
            n.retainTagByName("tr");
            n.retainTagByName("th");
            if (n.containTag("a"))
            {
                n.retainTagByName("a");
                group=n.getContent();
            }
            else
            {
                group="";
            }
            t.retainTagByName("tbody");
            n=t.cutTagByName("tr");
            while (!n.isEmpty())
            {
                if (n.containAttr("background-color"))
                {
                    nh=n.cutTagByName("td");
                    pos=nh.toInt();
                    nh=n.cutTagByName("td");
                    if (nh.containTag("a"))
                        nh.retainTagByName("a");
                    teamname=nh.getContent();
                    team_slug=nh.getBetweenAttr("/","/",2);
                    teamid=nh.getBetweenAttr("/","/",3);
                    teamlist+=","+teamid;
                    nh=n.cutTagByName("td");
                    nh=n.cutTagByName("td");
                    pts=nh.toInt();
                    nh=n.cutTagByName("td");
                    gd=nh.toInt();
                    nh=n.cutTagByName("td");
                    nh=n.cutTagByName("td");
                    play=nh.toInt();
                    nh=n.cutTagByName("td");
                    ow=nh.toInt();
                    nh=n.cutTagByName("td");
                    od=nh.toInt();
                    nh=n.cutTagByName("td");
                    ol=nh.toInt();
                    nh=n.cutTagByName("td");
                    of=nh.toInt();
                    nh=n.cutTagByName("td");
                    oa=nh.toInt();
                    nh=n.cutTagByName("td");
                    nh=n.cutTagByName("td");
                    hw=nh.toInt();
                    nh=n.cutTagByName("td");
                    hd=nh.toInt();
                    nh=n.cutTagByName("td");
                    hl=nh.toInt();
                    nh=n.cutTagByName("td");
                    hf=nh.toInt();
                    nh=n.cutTagByName("td");
                    ha=nh.toInt();
                    aw=ow-hw;
                    ad=od-hd;
                    al=ol-hl;
                    af=of-hf;
                    aa=oa-ha;
                    addTeam(teamid,teamname,team_slug);
                }
                n=t.cutTagByName("tr");
            }
        }
        t=m.cutTagByName("div");
    }
    //m.viewContent();
    resetTable(tleague, teamlist,teamlistname);
}
int main(int argc, char** argv)
{

    if (argc>1)
    {
        if (argc>2) DEBUG=true;

        if (!DEBUG)
        {
            init_mysql_conf();
            init_mysql();
        }
        getTable(argv[1]);
    }
	else
    {
        cout<<"Usage: gtable <league_id>"<<endl;
        cout<<"by Tran Huu Nam, huunam0@gmail.com"<<endl;
    }
    return 0;
}
