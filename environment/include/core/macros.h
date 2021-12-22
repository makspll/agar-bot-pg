#ifndef MACROS_H
#define MACROS_H 

#define BUFLEN 256
#define CLIENT_HEADER_LEN 6 // msg type, pid, seq number
#define SERVER_HEADER_LEN 5 // msg type, seq number
#define MAX_PLAYERS 8
#define TICK_SECONDS 0.032 // each simulation tick period
#define STATE_SYNC_SECONDS 0.05 // period between sync packets
#define INTERPOLATION_TICKS (std::ceil(STATE_SYNC_SECONDS / TICK_SECONDS))
#define TICK_NS (TICK_SECONDS * 1e9)
#define STATE_SYNC_NS (STATE_SYNC_SECONDS * 1e9)

#define CLIENT_TIMEOUT 10
#define CLIENT_CONNECTION_RETRY_PERIOD 5

#define CLIENT_UNIT_LENGTH 66
#define CLIENT_INIT_VISIBLE_UNITS 10
#define ARENA_HALF_WIDTH_UNITS 5
#define MAX_BLOBS 16
#define INITIAL_MASS 10
#define INITIAL_SPEED_UNITS_PER_SECOND 1
#define MAX_FOOD_PELLETS 10
#define MAX_FOOD_MASS 5
#define FOOD_PROB_PER_SECOND 1


#define EPS 1e-6
#define ABS(N) ((N<0)?(-N):(N))
#define APPROX_EQ(x,y) (ABS(x - y) < EPS)
#endif //MACROS_H