#include "fmatch.h"
#include <time.h>
#include <pthread.h>

void *getMatch(void *iMatch)
{

    int ii=0, iMid = *(int*)iMatch;
    cout<<"Get Match "<<iMid<<endl;
    fmatch m(iMid);
    if (!m.getData()) return NULL;
    cout<<"Loop "<<ii++<<endl;
    while (m.status<4)
    {
        cout<<"Loop "<<ii++<<endl;
        m.getData();
        sleep(3);
    }
    cout<<"Endof getmatch "<<iMid<<endl;

}
void test_thread()
{
    pthread_t thread1, thread2;
    int  iret1, iret2;
    int m1=349361, m2=349360;
    printf("Create Threads\n");
    iret1 = pthread_create( &thread1, NULL, getMatch, &m1);
    pthread_join( thread1, NULL);
    iret2 = pthread_create( &thread1, NULL, getMatch, &m2);
    pthread_join( thread1, NULL);
    printf("Thread 1 returns: %d\n",iret1);
    printf("Thread 2 returns: %d\n",iret2);
}
int main(int argc, char** argv)
{
    //database db;
    //int database::c=1;
    //int m1=349361, m2=349360;
    //getMatch(&m1);
    test_thread();

    return 0;
}
