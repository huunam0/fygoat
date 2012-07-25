#ifndef FMATCH_H
#define FMATCH_H

#include "needed.h"
#include "shtml.h"
class fmatch
{
    public:
        int status;
        fmatch();
        fmatch(int _id);
        fmatch(int _id, int _hteam, int _ateam);
        virtual ~fmatch();
        void getData();
        void setValue(int iIndex, int iValue, int iTeam=0);
        void emitEvent(int iIndex, int iValue=0, int iTeam=0);
    protected:
    private:
        int m[2][8];//d0 0:home,1:away; d1 0:goal,1:goal first time, 2:red card, 3: yellow card, 4: shot, 5: shot on goal, 6: corner kick, 7: possession;
        int team [2]; //id of
        int id;
        bool isFirstTime;

};

#endif // FMATCH_H
