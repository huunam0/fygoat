#include "shtml.h"

shtml::shtml()
{
    //ctor
}
shtml::shtml(string _s)
{
    htm=_s;
}
shtml::shtml(string _s, string _a)
{
    htm=_s;
    attr=_a;
}
shtml::~shtml()
{
    htm.clear();
}

void shtml::setContent(string sht)
{
    htm = sht;
}

string shtml::getContent()
{
    return htm;
}

string shtml::getAttr()
{
    return attr;
}

void shtml::viewContent()
{
    cout<<htm<<endl;
    cout<<attr<<endl;
}
/*****************************************************
 * Function getBetween
 * Input: sFrom, sTo, iIndex
 * Output: substring between two substring sFrom and sTo
 * Author: Tran Huu Nam - huunam0@gmail.com
 * Date: 19 July 2012
 */
string shtml::getBetween(string sFrom, string sTo, int iIndex)
{
    int iPosFrom = htm.find(sFrom);
    int iFromLength=sFrom.length();
    while ((iPosFrom!=string::npos) && (iIndex>1))
    {
        iIndex--;
        iPosFrom = htm.find(sFrom,iPosFrom+iFromLength);
    }
    if (iPosFrom==string::npos)
    {
        return string("");
    }
    else
    {
        iPosFrom+=iFromLength;
        int iPosTo = htm.find(sTo,iPosFrom);
        if (iPosTo==string::npos)
        {
            return htm.substr(iPosFrom);
        }
        else
        {
            return htm.substr(iPosFrom,iPosTo-iPosFrom);
        }
    }
}
string shtml::getBetweenAttr(string sFrom, string sTo, int iIndex)
{
    int iPosFrom = attr.find(sFrom);
    int iFromLength=sFrom.length();
    while ((iPosFrom!=string::npos) && (iIndex>1))
    {
        iIndex--;
        iPosFrom = attr.find(sFrom,iPosFrom+iFromLength);
    }
    if (iPosFrom==string::npos)
    {
        return string("");
    }
    else
    {
        iPosFrom+=iFromLength;
        int iPosTo = attr.find(sTo,iPosFrom);
        if (iPosTo==string::npos)
        {
            return attr.substr(iPosFrom);
        }
        else
        {
            return attr.substr(iPosFrom,iPosTo-iPosFrom);
        }
    }
}
/*****************************************************
 * Function getPairBetween
 * Input: sFrom, sTo, iIndex
 * Output: substring between two substring sFrom and sTo in Pair
 * Author: Tran Huu Nam - huunam0@gmail.com
 * Date: 19 July 2012
 */
string shtml::getPairBetween(string sFrom, string sTo, int iIndex )
{
    if (htm.empty()) return "";
    int iPosFrom = htm.find(sFrom);
    int iMatch=0;
    int iPosTo =iPosFrom;
    int iFromLength = sFrom.length();
    int iToLength = sTo.length();
    int iAdd = iFromLength;
    int v1,v2;
    if (iPosFrom!=string::npos) iMatch=1;
    else return "";
    while (iPosFrom!=string::npos)
    {
        iPosTo+=iAdd;
        v1=htm.find(sFrom,iPosTo);
        v2=htm.find(sTo,iPosTo);
        if (v2==string::npos) break;
        if ((v2<v1)||(v1==string::npos))
        {
            iMatch--;
            iPosTo=v2;
            iAdd=iToLength;
        }
        else
        {
            iMatch++;
            iPosTo=v1;
            iAdd=iFromLength;
        }
        if (iMatch==0)
        {
            iIndex--;
            if (iIndex==0)
            {
                //cout<<iPosFrom<<"->"<<iPosTo<<endl;
                return (htm.substr(iPosFrom+iFromLength,iPosTo-iPosFrom-iFromLength));
            }
            else
            {
                iPosFrom=htm.find(sFrom,iPosTo+iToLength);
                iAdd=iFromLength;
                iPosTo=iPosFrom;
                if (iPosFrom!=string::npos) iMatch=1;
                else return "";
            }
        }

    }
    //cout<<iPosFrom<<"-"<<iPosTo<<" /"<<iMatch<<endl;
    if ((iPosTo==string::npos)||(iMatch>0)) return (htm.substr(iPosFrom+iFromLength));
    return ("");

}
int shtml::getLastPairBetween(string sFrom, string sTo, int iIndex )
{
    if (htm.empty()) return string::npos;
    int iPosFrom = htm.find(sFrom);
    int iMatch=0;
    int iPosTo =iPosFrom;
    int iFromLength = sFrom.length();
    int iToLength = sTo.length();
    int iAdd = iFromLength;
    int v1,v2;
    if (iPosFrom!=string::npos) iMatch=1;
    else return string::npos;
    while (iPosFrom!=string::npos)
    {
        iPosTo+=iAdd;
        v1=htm.find(sFrom,iPosTo);
        v2=htm.find(sTo,iPosTo);
        if (v2==string::npos) break;
        if ((v2<v1)||(v1==string::npos))
        {
            iMatch--;
            iPosTo=v2;
            iAdd=iToLength;
        }
        else
        {
            iMatch++;
            iPosTo=v1;
            iAdd=iFromLength;
        }
        if (iMatch==0)
        {
            iIndex--;
            if (iIndex==0)
            {
                //cout<<iPosFrom<<"->"<<iPosTo<<endl;
                return iPosTo+iToLength;
            }
            else
            {
                iPosFrom=htm.find(sFrom,iPosTo+iToLength);
                iAdd=iFromLength;
                iPosTo=iPosFrom;
                if (iPosFrom!=string::npos) iMatch=1;
                else return string::npos;
            }
        }

    }
    //cout<<iPosFrom<<"-"<<iPosTo<<" /"<<iMatch<<endl;
    if ((iPosTo==string::npos)||(iMatch>0)) return htm.length();
    return string::npos;

}
/*****************************************************
 * Function cutBetween
 * Input: sFrom, sTo, iIndex
 * Output: substring between two substring sFrom and sTo. Remove it from data (htm)
 * Author: Tran Huu Nam - huunam0@gmail.com
 * Date: 19 July 2012
 */
string shtml::cutBetween(string sFrom, string sTo, int iIndex)
{
    int iPosFrom = htm.find(sFrom);
    int iFromLength=sFrom.length();
    while ((iPosFrom!=string::npos) && (iIndex>1))
    {
        iIndex--;
        iPosFrom = htm.find(sFrom,iPosFrom+iFromLength);
    }
    if (iPosFrom==string::npos)
    {
        return string("");
    }
    else
    {
        iPosFrom+=iFromLength;
        int iPosTo = htm.find(sTo,iPosFrom);
        if (iPosTo==string::npos)
        {
            string sRet =htm.substr(iPosFrom);
            htm.erase(iPosFrom-iFromLength);
            return sRet;
        }
        else
        {
            string sRet =htm.substr(iPosFrom,iPosTo-iPosFrom);
            htm.erase(iPosFrom-iFromLength,iPosTo-iPosFrom+iFromLength+sTo.length());
            return sRet;
        }
    }
}
/*****************************************************
 * Function cutPairBetween
 * Input: sFrom, sTo, iIndex
 * Output: substring between two substring sFrom and sTo in pair. Remove it from data (htm)
 * Author: Tran Huu Nam - huunam0@gmail.com
 * Date: 19 July 2012
 */
string shtml::cutPairBetween(string sFrom, string sTo, int iIndex )
{
    if (htm.empty()) return "";
    int iPosFrom = htm.find(sFrom);
    int iMatch=0;
    int iPosTo =iPosFrom;
    int iFromLength = sFrom.length();
    int iToLength = sTo.length();
    int iAdd = iFromLength;
    int v1,v2;
    if (iPosFrom!=string::npos) iMatch=1;
    else return "";
    string sRet;
    while (iPosFrom!=string::npos)
    {
        iPosTo+=iAdd;
        v1=htm.find(sFrom,iPosTo);
        v2=htm.find(sTo,iPosTo);
        if (v2==string::npos) break;
        if ((v2<v1)||(v1==string::npos))
        {
            iMatch--;
            if (iMatch<3)
            {
                int xx=0;
            }
            iPosTo=v2;
            iAdd=iToLength;
        }
        else
        {
            iMatch++;
            iPosTo=v1;
            iAdd=iFromLength;
        }
        if (iMatch==0)
        {
            iIndex--;
            if (iIndex==0)
            {
                //cout<<iPosFrom<<"->"<<iPosTo<<endl;
                sRet=htm.substr(iPosFrom+iFromLength,iPosTo-iPosFrom-iFromLength);
                htm.erase(iPosFrom,iPosTo-iPosFrom+iToLength);
                return (sRet);
            }
            else
            {
                iPosFrom=htm.find(sFrom,iPosTo+iToLength);
                iAdd=iFromLength;
                iPosTo=iPosFrom;
                if (iPosFrom!=string::npos) iMatch=1;
                else return "";
            }
        }

    }
    //cout<<iPosFrom<<"-"<<iPosTo<<" /"<<iMatch<<endl;
    if ((iPosTo==string::npos)||(iMatch>0))
    {
        sRet= htm.substr(iPosFrom+iFromLength);
        htm.erase(iPosFrom);
        return (sRet);
    }
    return ("");

}

/*****************************************************
 * Function retainBetween
 * Input: sFrom, sTo, iIndex
 * Output: none
 * Action: retain only substring between two substring sFrom and sTo (change to htm variable)
 * Author: Tran Huu Nam - huunam0@gmail.com
 * Date: 19 July 2012
 */
void shtml::retainBetween(string sFrom, string sTo, int iIndex)
{
    int iPosFrom = htm.find(sFrom);
    int iFromLength=sFrom.length();
    while ((iPosFrom!=string::npos) && (iIndex>1))
    {
        iIndex--;
        iPosFrom = htm.find(sFrom,iPosFrom+iFromLength);
    }
    if (iPosFrom!=string::npos)
    {
        iPosFrom+=iFromLength;
        int iPosTo = htm.find(sTo,iPosFrom);
        if (iPosTo==string::npos)
        {
            htm.erase(0,iPosFrom);
        }
        else
        {
            htm.erase(iPosTo);
            htm.erase(0,iPosFrom);
        }
    }
}
/*****************************************************
 * Function retainPairBetween
 * Input: sFrom, sTo, iIndex
 * Output: none
 * Action: retain only substring between two substring sFrom and sTo in Pair
 * Author: Tran Huu Nam - huunam0@gmail.com
 * Date: 19 July 2012
 */
void shtml::retainPairBetween(string sFrom, string sTo, int iIndex )
{
    int iPosFrom = htm.find(sFrom);
    int iMatch=0;
    int iPosTo =iPosFrom;
    int iFromLength = sFrom.length();
    int iToLength = sTo.length();
    int iAdd = iFromLength;
    int v1,v2;
    if (iPosFrom!=string::npos) iMatch=1;
    while (iPosFrom!=string::npos)
    {
        iPosTo+=iAdd;
        v1=htm.find(sFrom,iPosTo);
        v2=htm.find(sTo,iPosTo);
        if (v2==string::npos) break;
        if ((v2<v1)||(v1==string::npos))
        {
            iMatch--;
            iPosTo=v2;
            iAdd=iToLength;
        }
        else
        {
            iMatch++;
            iPosTo=v1;
            iAdd=iFromLength;
        }
        if (iMatch==0)
        {
            iIndex--;
            if (iIndex==0)
            {
                //cout<<iPosFrom<<"->"<<iPosTo<<endl;
                htm.erase(iPosTo);
                htm.erase(0,iPosFrom+iFromLength);
                return;
            }
            else
            {
                iPosFrom=htm.find(sFrom,iPosTo+iToLength);
                iAdd=iFromLength;
                iPosTo=iPosFrom;
                if (iPosFrom!=string::npos) iMatch=1;
                else return;
            }
        }

    }
    //cout<<iPosFrom<<"-"<<iPosTo<<" /"<<iMatch<<endl;
    if ((iPosTo==string::npos)||(iMatch>0)) htm.erase(0,iPosFrom+iFromLength);
}

/*****************************************************
 * Function removeBetween
 * Input: sFrom, sTo, iIndex
 * Output: none
 * Action: remove substring between two substring sFrom and sTo (change to htm variable)
 * Author: Tran Huu Nam - huunam0@gmail.com
 * Date: 19 July 2012
 */
void shtml::removeBetween(string sFrom, string sTo, int iTimes)
{
    int iPosFrom = htm.find(sFrom);
    int iFromLength=sFrom.length();
    int iPosTo;
    int iToLength=sTo.length();
    while ((iTimes!=0) && (iPosFrom!=string::npos) )
    {
        iTimes--;
        //iPosFrom+=iFromLength;
        iPosTo = htm.find(sTo,iPosFrom+iFromLength);
        if (iPosTo==string::npos)
        {
            htm.erase(iPosFrom);
        }
        else
        {
            htm.erase(iPosFrom,iPosTo-iPosFrom+iToLength);
        }
        iPosFrom = htm.find(sFrom,iPosFrom);
    }

}
/*****************************************************
 * Function removePairBetween
 * Input: sFrom, sTo, iIndex
 * Output: none
 * Action: remove substring between two substring sFrom and sTo in Pair (change to htm variable)
 * Author: Tran Huu Nam - huunam0@gmail.com
 * Date: 19 July 2012
 */
bool shtml::removePairBetween(string sFrom, string sTo, int iIndex )
{
    if (htm.empty()) return false;
    int iPosFrom = htm.find(sFrom);
    int iMatch=0;
    int iPosTo =iPosFrom;
    int iFromLength = sFrom.length();
    int iToLength = sTo.length();
    int iAdd = iFromLength;
    int v1,v2;
    if (iPosFrom!=string::npos) iMatch=1;
    else return false;
    string sRet;
    while (iPosFrom!=string::npos)
    {
        iPosTo+=iAdd;
        v1=htm.find(sFrom,iPosTo);
        v2=htm.find(sTo,iPosTo);
        if (v2==string::npos) break;
        if ((v2<v1)||(v1==string::npos))
        {
            iMatch--;
            iPosTo=v2;
            iAdd=iToLength;
        }
        else
        {
            iMatch++;
            iPosTo=v1;
            iAdd=iFromLength;
        }
        if (iMatch==0)
        {
            iIndex--;
            if (iIndex==0)
            {
                htm.erase(iPosFrom,iPosTo-iPosFrom+iToLength);
                return true;
            }
            else
            {
                iPosFrom=htm.find(sFrom,iPosTo+iToLength);
                iAdd=iFromLength;
                iPosTo=iPosFrom;
                if (iPosFrom!=string::npos) iMatch=1;
                else return true;
            }
        }

    }
    //cout<<iPosFrom<<"-"<<iPosTo<<" /"<<iMatch<<endl;
    if ((iPosTo==string::npos)||(iMatch>0))
    {
        htm.erase(iPosFrom);
        return true;
    }
    return false;
}
/*****************************************************
 * Function deleteTo
 * Input: sSub, iIndex
 * Output: none
 * Action: remove substring before substring sSub (change to htm variable)
 * Author: Tran Huu Nam - huunam0@gmail.com
 * Date: 19 July 2012
 */
void shtml::deleteTo(string sSub, int iIndex)
{
    int iPosFrom = htm.find(sSub);
    int iFromLength=sSub.length();
    while ((iPosFrom!=string::npos) && (iIndex>1))
    {
        iIndex--;
        iPosFrom = htm.find(sSub,iPosFrom+iFromLength);
    }
    if (iPosFrom!=string::npos)
    {
        htm.erase(0,iPosFrom+iFromLength);
    }
}
/* Function getTagByName
 * Input: stag, iIndex
 * Output: innerHTML of given tag.
 * Action: none
 * Author: Tran Huu Nam - huunam0@gmail.com
 * Date: 19 July 2012
 */
shtml shtml::getTagByName(string sTag, int iIndex)
{
    string s = getPairBetween(string("<")+sTag,string("</")+sTag+string(">"),iIndex);
    int v = s.find(">");
    if (v!=string::npos)
    {
        //attr=s.substr(0,v);
        return shtml(s.substr(v+1),s.substr(0,v));
    }
    return shtml(s);
}
shtml shtml::getTagByOrder(int iOrder)
{
    shtml t(htm),r;
    string sTag = t.getBetween("<"," ");
    while ((iOrder>0) && (!sTag.empty()) )
    {
        r=t.cutTagByName(sTag);
        sTag = t.getBetween("<"," ");
        iOrder--;
    }
    return r;
}
/* Function cutTagByName
 * Input: stag, iIndex
 * Output: innerHTML of given tag. Then remove that from data (change to htm)
 * Action: none
 * Author: Tran Huu Nam - huunam0@gmail.com
 * Date: 19 July 2012
 */
shtml shtml::cutTagByName(string sTag, int iIndex)
{
    string s = cutPairBetween(string("<")+sTag,string("</")+sTag+string(">"),iIndex);
    int v = s.find(">");
    if (v!=string::npos)
    {
        //attr=s.substr(0,v);
        return shtml(s.substr(v+1),s.substr(0,v));
    }
    return shtml(s);
}
void shtml::retainTagByName(string sTag, int iIndex)
{
    string s = getPairBetween(string("<")+sTag,string("</")+sTag+string(">"),iIndex);
    int v = s.find(">");
    if (v!=string::npos)
    {
        htm=s.substr(v+1);
        attr=s.substr(0,v);
    }
    else
    {
        htm=s;
        attr="";
    }
}
void shtml::removeTagByName(string sTag, int iTime)
{
    while ((iTime--!=0) && removePairBetween(string("<")+sTag,string("</")+sTag+string(">")))
    {

    }

}
/* Function cutTagByName
 * Input: stag, iIndex
 * Output: innerHTML of given tag. Then remove that from data (change to htm)
 * Action: none
 * Author: Tran Huu Nam - huunam0@gmail.com
 * Date: 19 July 2012
 */
string shtml::getText()
{
    shtml s(htm);
    s.removeAllTags();
    return s.getContent();
}
void shtml::removeAllTags()
{
    removeBetween("<",">",-1);
}
bool shtml::loadfromfile(const string fileName)
{
	string line;
	htm.clear();
	ifstream myFile(fileName.c_str());
	//cout<<"Open file "<<fileName<<endl;
	if(!myFile.good())
	{
		//cout << "Error opening file "<<fileName<<endl;
		return false;
	}

	while(!myFile.eof())
	{
		getline(myFile, line);
		htm+=line;
	}
	myFile.close();
	return true;
}
bool shtml::loadFromURL(const string sUrl)
{
    FILE *in;
    char buff[1024];
    char command[250];
    sprintf(command,"wget -q -O -  '%s' | iconv -f iso-8859-1 -t utf-8",sUrl.c_str());
    //cout<<"Load from "<<sUrl<<endl;
    htm.clear();
    if(!(in = popen(command, "r")))
        {
            return false;
        }

    while(fgets(buff, sizeof(buff), in)!=NULL)
        {
            string s(buff);
            htm+= s;
        }
    pclose(in);
    return true;
}
bool shtml::writeFile(const string fileName)
{
    ofstream out(fileName.c_str(), ios::out | ios::binary);

    if(!out)
    {
        //cout << "Cannot open output file.\n";
        return false;
    }

    out.write(htm.c_str(), htm.length());

    out.close();
    return true;
}

bool shtml::isEmpty()
{
    return htm.empty() && attr.empty();
}

bool shtml::contain(string sSub)
{
    return htm.find(sSub)!=string::npos;
}
bool shtml::containAttr(string sSub)
{
    return attr.find(sSub)!=string::npos;
}
bool shtml::containTag(string sTag)
{
    return contain(string("<")+sTag);
}
int shtml::toInt()
{
    return atoi(htm.c_str());
}
float shtml::toFloat()
{
    return atof(htm.c_str());
}

void shtml::replace(string sFind, string sReplace, int iTime)
{
    int iFind = htm.find(sFind);
    int iFindLength = sFind.length();
    int iReplaceLength = sReplace.length();
    while ((iTime!=0) && (iFind!=string::npos))
    {
        htm.replace(iFind,iFindLength,sReplace);
        iTime--;
        iFind = htm.find(sFind,iFind+iReplaceLength);
    }
}
void shtml::trim()
{
    htm.erase(remove(htm.begin(), htm.end(), '\t'), htm.end());
}

int shtml::count(string sSub)
{
    int v=htm.find(sSub);
    int iLen = sSub.length();
    int count=0;
    while (v!=string::npos)
    {
        count++;
        v=htm.find(sSub,v+iLen);
    }
    return count;
}
