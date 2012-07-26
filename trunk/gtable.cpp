#include <iostream>
#include "shtml.h"

using namespace std;

void getTable(string sLeague)
{
    shtml m,t,n,nh;
    int pos,play,hw,hd,hl,aw,ad,al,ow,od,ol,tabletype;
    float hf,ha,af,aa,of,oa;
    string teamid, teamname,group;
    //m.loadfromfile("tables.htm");
	m.loadFromURL((string("http://soccernet.espn.go.com/tables?league=")+sLeague).c_str());
    m.removeBetween("<!--","-->",-1);
    t=m.cutTagByName("div");
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
    m.retainTagByName("div",2);
    m.retainTagByName("div");
    m.retainTagByName("div",2);
    m.retainTagByName("div");
    m.retainTagByName("div",2);
    t=m.cutTagByName("div");
    while (!t.isEmpty())
    {

        t.retainTagByName("table");
        n.setContent(t.getAttr());
        group=n.getBetween("Live_false_group_","\"");
        n=t.cutTagByName("thead");
        if (n.contain("Overall")) tabletype=1;
        else tabletype=2;
        //t.retainTagByName("tbody");
        n=t.cutTagByName("tr");
        while (!n.isEmpty())
        {
            nh=n.cutTagByName("td");
            pos=nh.toInt();
            nh=n.cutTagByName("td");
            nh=n.cutTagByName("td");
            if (nh.containTag("a")) nh.retainTagByName("a");
            teamname = nh.getContent();
            nh.setContent(nh.getAttr());
            teamid = nh.getBetween("/id/","/");
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
            nh=n.cutTagByName("td");//remove
            if (tabletype==1)
            {
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
                nh=n.cutTagByName("td");//remove
                nh=n.cutTagByName("td");
                aw=nh.toInt();
                nh=n.cutTagByName("td");
                ad=nh.toInt();
                nh=n.cutTagByName("td");
                al=nh.toInt();
                nh=n.cutTagByName("td");
                af=nh.toInt();
                nh=n.cutTagByName("td");
                aa=nh.toInt();
            }
            cout<<pos<<"/"<<teamid<<"/"<<teamname<<"/"<<play<<"/"<<ow<<"/"<<od<<"/"<<ol<<"/"<<of<<"/"<<oa<<"/"<<hw<<"/"<<hd<<"/"<<hl<<"/"<<hf<<"/"<<ha<<"/"<<aw<<"/"<<ad<<"/"<<al<<"/"<<af<<"/"<<aa<<endl;
            n=t.cutTagByName("tr");
        }
        t=m.cutTagByName("div");
    }
    //m.viewContent();
}
int main2(int argc, char** argv)
{
    if (argc>1)
    {
        getTable(argv[1]);
    }
	else
    {
        cout<<"Usage: gtable <league_id>"<<endl;
        cout<<"by Tran Huu Nam, huunam0@gmail.com"<<endl;
    }
    return 0;
}
