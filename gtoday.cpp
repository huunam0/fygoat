#include <iostream>
#include "shtml.h"

using namespace std;

void getToday(string day="")
{
    shtml m,t,n,nh;
    string status,hteam,ateam,score,gid,group,hname,aname;
    //m.loadfromfile("scores2.htm");
    if (day.empty())
    {
        m.loadFromURL("http://soccernet.espn.go.com/scores?cc=4716");
    }
    else
    {
        m.loadFromURL((string("http://soccernet.espn.go.com/scores?date=")+day+string("&cc=4716&league=all")).c_str());
    }
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
    m.retainTagByName("div");
    t=m.cutTagByName("div");
    while (!t.isEmpty())
    {
        if (t.containAttr("group-set"))
        {
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
            cout<<"League: "<<nh.getContent()<<" : "<<n.getText()<<endl;
            t.retainTagByName("table");
            n=t.cutTagByName("tr");
            while (!n.isEmpty())
            {
                if (n.containAttr("gamebox"))
                {
                    nh=n.cutTagByName("td");
                    status=nh.getText();
                    nh=n.cutTagByName("td");
                    hname=nh.getText();
                    nh.setContent(nh.getAttr());
                    hteam=nh.getBetween("teamId-","\"");
                    nh=n.cutTagByName("td");
                    score=nh.getText();
                    nh=n.cutTagByName("td");
                    aname=nh.getText();
                    nh.setContent(nh.getAttr());
                    ateam=nh.getBetween("teamId-","\"");
                    cout<<"Status "<<status<<". Home:"<<hteam<<". Score:"<<score<<". Away:"<<ateam<<endl;
                }
                else
                {
                    nh=n.getTagByName("a");
                    //gid=nh.getAttr();
                    group=nh.getText();
                    nh.setContent(nh.getAttr());
                    gid=nh.getBetween("/groupId/","\"");
                    cout<<"Group "<<gid<<":"<<group<<endl;
                }
                n=t.cutTagByName("tr");
            }
        }

        t=m.cutTagByName("div");
    }
    //m.viewContent();
}

int main1(int argc, char** argv)
{
    getToday(argv[1]);
    return 0;
}
