#ifndef REST_CPP_DISPATCHER_ROUNDROBIN_H
#define REST_CPP_DISPATCHER_ROUNDROBIN_H

#include "../dispatcher.h"

namespace REST {

/**
 * Selects next not yet used Worker (Round Robin algorithm).
 */
class RoundRobinDispatcher : public Dispatcher {

};

}

#endif
