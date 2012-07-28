#include "database.h"
int database::c=1;

database::database()
{
    //ctor
}

database::~database()
{
    //dtor
}
void database::anounce(string s)
{
    cout<<database::c++<<":"<<s<<endl;
}
