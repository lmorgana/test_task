
#include "FdListener.hpp"

void FdListener::Handle()
{
    the_master->JoinPair(GetFd());
}
