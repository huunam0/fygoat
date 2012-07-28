#ifndef DATABASE_H
#define DATABASE_H

#include "needed.h"

class database
{
    public:
        static int c;
        static void anounce(string s);
        database();
        virtual ~database();
        void createThread();
    protected:
    private:
};

#endif // DATABASE_H
