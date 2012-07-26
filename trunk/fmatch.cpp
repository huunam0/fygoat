#include "fmatch.h"

fmatch::fmatch()
{
    id=0;
    team[0]=team[1]=0;
    isFirstTime=true;
}
fmatch::fmatch(int _id)
{
    id=_id;
    team[0]=team[1]=0;
    isFirstTime=true;
}
fmatch::fmatch(int _id, int _hteam, int _ateam)
{
    id=_id;
    team[0]=_hteam;
    team[1]=_ateam;
    isFirstTime=true;
}
fmatch::~fmatch()
{
    //dtor
}
/** @brief Set a value for the match
  *
  * Author: Tran Huu Nam, huunam0@gmail.com
  */
void fmatch::setValue(int iIndex, int iValue, int iTeam)
{
    //cout<<"Set value "<<iValue<<" for index "<<iIndex<<" of team "<<iTeam<<endl;
    if ((isFirstTime)||(m[iTeam][iIndex]!=iValue))
    {
        m[iTeam][iIndex]=iValue;
        emitEvent(iIndex,iValue,iTeam);
    }
}
bool fmatch::getData()
{
    shtml sh,t,n,nh,nh2;
    int i=0,v=0;
    char url[300];
    bool reCard = true;
    sprintf(url,"http://soccernet.espn.go.com/match/_/id/%d?cc=4716",id);
    //sprintf(url,"http://soccernet.espn.go.com/match?id=%d&cc=4716",id);
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
    else
    {
        status=3;
    }
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
    cout<<"End of get data"<<endl;
    return true;
}

void fmatch::emitEvent(int iIndex, int iValue, int iTeam)
{
    char sql[300];
    sprintf(sql,"INSERT INTO f_timeline (event, value, team, match, date) VALUE (%d,%d,%d,%d,NOW())",iIndex,iValue,iTeam,id);
    cout<<sql<<endl;
    string t,field;
    t=(iTeam==1?"a":"h");
    switch (iIndex)
    {
    case 0:
        field="goal";
        break;
    case 1:
        field="1goal";
        break;
    case 2:
        field="red";
        break;
    case 3:
        field="yellow";
        break;
    case 4:
        field="shot";
        break;
    case 5:
        field="shotgoal";
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
        sprintf(sql,"UPDATE f_matches SET %s=%d where match_id=%d",(t+field).c_str(),iValue,id);
        //cout<<sql<<endl;
    }

}
