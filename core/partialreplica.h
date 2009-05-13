#ifndef __PARTIAL_REPLICA_H__
#define __PARTIAL_REPLICA_H__

#include "types.h"
#include "sw.h"
#include "mrimrf.h"

void partial_replica_exchange_move(MRIMRF & primary_replica, 
				   MRIMRF & secondary_replica, int attemptcount);

#endif // 
