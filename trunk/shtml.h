#ifndef SHTML_H
#define SHTML_H
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <algorithm>
using namespace std;

class shtml
{
    public:
        shtml();
        shtml(string _s);
        shtml(string _s, string _a);
        virtual ~shtml();
        void setContent(string sht);
        string getContent();
        string getAttr();
        void viewContent();
        string getBetween(string sFrom, string sTo, int iIndex=1);
        //string getPairBetween(string sFrom, string sTo,int iIndex=1);
        string getPairBetween(string sFrom, string sTo,int iIndex=1);
        int getLastPairBetween(string sFrom, string sTo,int iIndex=1);
        string cutBetween(string sFrom, string sTo, int iIndex=1);
        string cutPairBetween(string sFrom, string sTo,int iIndex=1);
        void retainBetween(string sFrom, string sTo, int iIndex=1);
        void retainPairBetween(string sFrom, string sTo,int iIndex=1);
        void removeBetween(string sFrom, string sTo, int iTime=1);
        bool removePairBetween(string sFrom, string sTo,int iIndex=1);
        void deleteTo(string sSub, int iIndex=1);
        shtml getTagByName(string sTag, int iIndex=1);
        shtml getTagByOrder(int iOrder=1);
        shtml cutTagByName(string sTag, int iIndex=1);
        void retainTagByName(string sTag, int iIndex=1);
        void removeTagByName(string sTag, int iTime=1);
        void removeAllTags();
        string getText();
        bool loadfromfile(const string fileName);
        bool writeFile(const string fileName);
        bool isEmpty();
        bool contain(string sSub);
        bool containAttr(string sSub);
        bool containTag(string sTag);
        int toInt();
        float toFloat();
        void replace(string sFind, string sReplace, int iTime=1);
        void trim();
        int count(string sSub);

    protected:
    private:
        string htm;
        string attr;
};

#endif // SHTML_H
