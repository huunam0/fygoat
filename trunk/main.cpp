#include <iostream>

#include "shtml.h"
using namespace std;
void test1()
{
    /**/
    shtml sh("Hello");
    sh.viewContent();
    sh.setContent("<div id=\"I_bar\"><ul><li><a href=\"/r/\">Ref</a></li><li><a href=\"/s/\">Str</a></li><li class=\"here\">string</li></ul></div><div id='I_user'>XXX</div>");
    sh.viewContent();
    cout<<endl;
    //cout<<sh.cutPairBetween("a","n",1);
    //sh.removePairBetween("a","n",2);
    //sh.getPairBetween("a","n",1).viewContent();
    //sh.deleteTo("a",2);
    //cout<<"con lai:"<<endl;
    //sh.viewContent();
    sh.getTagByName("ul").viewContent();
    return ;
    /**/
}
void test2()
{
    shtml sh;
    sh.setContent("<div id=\"I_bar\"><ul><li><a href=\"/r/\">Ref</a></li><li><a href=\"/s/\">Str</a></li><li class=\"here\">string</li></ul></div><div id='I_user'>XXX</div>");
    sh.viewContent();
    sh.cutTagByName("a").viewContent();
    cout<<"con lai:"<<endl;
    sh.viewContent();
    return ;
    /**/
}
void test3() //removebetween
{
    shtml sh;
    sh.setContent("<div id=\"I_bar\"><ul><li><a href=\"/r/\">Ref</a></li><li><a href=\"/s/\">Str</a></li><li class=\"here\">string</li></ul></div><div id='I_user'>XXX</div>");
    sh.viewContent();
    sh.removeBetween("<",">",-1);
    cout<<"con lai:"<<endl;
    sh.viewContent();
    return ;
    /**/
}
void test4() //removealltags
{
    shtml sh;
    sh.setContent("<div id=\"I_bar\"><ul><li><a href=\"/r/\">Ref</a></li><li><a href=\"/s/\">Str</a></li><li class=\"here\">string</li></ul></div><div id='I_user'>XXX</div>");
    sh.viewContent();
    cout<<"con lai:"<<endl<<sh.getText()<<endl;

    return ;
    /**/
}
void test5() //removealltags
{
    shtml sh,t;
    sh.loadfromfile("D:\\DUAN\\C\\fgoat\\342339.htm");
    //sh.setContent("<div id=\"I_bar\"><ul><li><a href=\"/r/\">Ref</a></li><li><a href=\"/s/\">Str</a></li><li class=\"here\">string</li></ul></div><div id='I_user'>XXX</div>");
    int i=0;
    sh.viewContent();
    sh.setContent(sh.getTagByName("body").getContent());
    sh.viewContent();
    t=sh.cutTagByName("div");
    while (!t.isEmpty())
    {
        cout<<"---->"<<i++<<endl;
        t.viewContent();
        if (t.containAttr("bg-elements")) break;
        if (sh.isEmpty()) break;
        t=sh.cutTagByName("div");
    }
    t.viewContent();
    return ;
    /**/
}
void test6()
{
    shtml sh("ab");
    if (sh.isEmpty()) cout<<"Empty"; else sh.viewContent();
}
void test7() //removealltags
{
    shtml sh,t;
    shtml nh;
    int i=0;
    //sh.loadfromfile("D:\\DUAN\\C\\fgoat\\342339.htm");
    sh.loadfromfile("match.htm");
    sh.removeBetween("<!--","-->",-1);
    //sh.viewContent();
    //sh.setContent(sh.getTagByName("body").getContent());
    //sh.viewContent();
    t=sh.cutTagByName("div");
    while (!t.isEmpty())
    {
        if (t.containAttr("bg-elements"))
        {
            //sh.setContent(t.getContent());
            sh=t;
            break;
        }
        if (sh.isEmpty()) break;
        t=sh.cutTagByName("div");
    }
    //sh.viewContent();
    sh.retainTagByName("div",2);
    sh.retainTagByName("div");
    sh.retainTagByName("div",2);
    sh.retainTagByName("div");
    t=sh.cutTagByName("div",2);
    t.retainTagByName("div");

    shtml n = t.cutTagByName("div");
    n.retainTagByName("span");
    n.viewContent();
    t.retainTagByName("p");
    t.viewContent();
    sh.retainTagByName("div",2);
    //sh.viewContent();
    t=sh.cutTagByName("div");
    t.retainTagByName("tbody");
    n=t.cutTagByName("tr");
    while (!n.isEmpty())
    {
        for (i=1;i<=2;i++)
        {
            nh=n.cutTagByName("td");
            if (nh.contain("'"))
            {
                nh.retainBetween("(","'");
                int v;
                v=nh.toInt();
                cout<<"Team "<<i<<" win at "<<v<<" minute th"<<endl;
            }
        }
        n=t.cutTagByName("tr");
    }
    t=sh.cutTagByName("div");
    t.retainTagByName("tbody");
    int v[5];
    //n=t.cutTagByName("tr");

    n=t.cutTagByName("tr");
    while (!n.isEmpty())
    {

        for (i=1;i<=2;i++)
        {
            nh=n.cutTagByName("td",i);
            v[i]=nh.toInt();
            nh.retainBetween("(",")");
            v[i+2]=nh.toInt();
        }
        n.replace(" ","",-1);
        n.trim();
        cout<<"+"<<n.getText()<<v[1]<<" and "<<v[2]<<"   /"<<v[3]<<"/"<<v[4]<<endl;
        n=t.cutTagByName("tr");
    }
    for (i=1; i<=2; i++)
    {
        t=sh.cutTagByName("div");
        t.removeTagByName("div",3);
        int card[3];
        card[1]=t.count("soccer-icons-yellowcard");
        card[2]=t.count("soccer-icons-redcard");
        cout<<"Team "<<i<<" has "<<card[1]<<" yellow card(s) and "<<card[2]<<" red card(s)."<<endl;
    }
    //t.viewContent();
    return ;
    /**/
}
void test8()
{
    shtml sh,t;
    sh.setContent("<div id=\"I_bar\"><ul><li><a href=\"/r/\">Ref</a></li><li><a href=\"/s/\">Str</a></li><li class=\"here\">string</li></ul></div><div id='I_user'>XXX</div>");
    sh.removeTagByName("li",5);
    sh.viewContent();
    return ;
}
void test9()
{
    shtml sh;
    sh.setContent("<div id=\"I_bar\"><ul><li><a href=\"/r/\">Ref</a></li><li><a href=\"/s/\">Str</a></li><li class=\"here\">string</li></ul></div><div id='I_user'>XXX</div>");
    sh.replace("li","l",-1);

    cout<<"con lai:"<<endl;
    sh.viewContent();
    return ;
    /**/
}
void test10()
{
    shtml m,t,n,nh;
    string status,hteam,ateam,score,gid,group,hname,aname;
    m.loadfromfile("scores2.htm");
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
void test11()
{
    shtml sh,t;
    sh.setContent("<div id=\"I_bar\"><ul><li><a href=\"/r/\">Ref</a></li><li><a href=\"/s/\">Str</a></li><li class=\"here\">string</li></ul></div><div id='I_user'>XXX</div>");
    t=sh.getTagByOrder(2);
    t.viewContent();
}
void test12()
{
    shtml m,t,n,nh;
    int pos,play,hw,hd,hl,aw,ad,al,ow,od,ol,tabletype;
    float hf,ha,af,aa,of,oa;
    string teamid, teamname,group;
    m.loadfromfile("tables.htm");
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
int main()
{
    test12();
    return 0;
}
