#ifndef SHTML_H
#define SHTML_H

#include "needed.h"

class shtml
{
    public:
        shtml();
        shtml(const string _s);
        shtml(const string _s,const  string _a);
        virtual ~shtml();
        void setContent(const string sht);
        string getContent();
        string getAttr();
        void viewContent();
        string getBetween(const string sFrom,const  string sTo,const  int iIndex=1);
        string getBetweenAttr(const string sFrom,const  string sTo,const  int iIndex=1);
        string getPairBetween(const string sFrom,const  string sTo,const int iIndex=1);
        int getLastPairBetween(const string sFrom,const  string sTo,const int iIndex=1);
        string cutBetween(const string sFrom,const  string sTo,const  int iIndex=1);
        string cutPairBetween(const string sFrom,const  string sTo,const int iIndex=1);
        void retainBetween(const string sFrom, const string sTo, const int iIndex=1);
        void retainPairBetween(const string sFrom, const string sTo,const int iIndex=1);
        void removeBetween(const string sFrom, const string sTo, const int iTime=1);
        bool removePairBetween(const string sFrom, const string sTo,const int iIndex=1);
        void deleteTo(const string sSub, const int iIndex=1);
        shtml getTagByName(const string sTag, const int iIndex=1);
        shtml getTagByOrder(const int iOrder=1);
        shtml cutTagByName(const string sTag, const int iIndex=1);
        void retainTagByName(const string sTag, const int iIndex=1);
        void removeTagByName(const string sTag, const int iTime=1);
        void removeAllTags();
        string getText();
        bool loadfromfile(const string fileName);
        bool loadFromURL(const string sUrl,const  bool bconv = false);
        bool writeFile(const string fileName);
        bool isEmpty();
        bool contain(const string sSub);
        bool containAttr(const string sSub);
        bool containTag(const string sTag);
        int toInt();
        float toFloat();
        void replace(const string sFind,const  string sReplace,const  int iTime=1);
        void trim();
        int count(const string sSub);

    protected:
    private:
        string htm;
        string attr;
};

#endif // SHTML_H
