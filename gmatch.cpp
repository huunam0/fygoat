

#include "fmatch.h"
#include <time.h>

int main(int argc, char** argv)
{
    if (argc>1)
    {
        int i=0;
        fmatch m(atoi(argv[1]));
        if (!m.getData()) return 1;
        while (m.status<4)
        {
            m.getData();
            sleep(3);
            if (i++>10) break;
        }


    }
    else
    {
        cout<<"Usage: gmatch <match_id>"<<endl;
        cout<<"by Tran Huu Nam, huunam0@gmail.com"<<endl;
    }
    return 0;
}
