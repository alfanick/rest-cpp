#ifndef REST_CPP_DISPATCHER_LEASTCONNECTIONS_H
#define REST_CPP_DISPATCHER_LEASTCONNECTIONS_H

#include "../dispatcher.h"

namespace REST {

/**
 * Selects next Worker with least connections in its queue.
 */
class LeastConnectionsDispatcher : public Dispatcher {

};

}

#endif
