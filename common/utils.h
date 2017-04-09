/*****************************************************************************

 This file is part of QSS Solver.

 QSS Solver is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 QSS Solver is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with QSS Solver.  If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#ifndef UTILS_H_
#define UTILS_H_

#ifdef _WIN32
#include <sys/time.h>
#else
#include <time.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#ifdef __linux__
#include <pthread.h>
#endif

/**
 *
 */
#define ABS(X) ((X)>0.0 ? (X) : -(X))

/**
 *
 */
#define MAX(X,Y) ((X)>(Y) ? (X) : (Y))

/**
 *
 */
#define NULL_INDEX -10000

/**
 *
 */
#define UPDATE_DEPS -1

/**
 *
 */
#define ASSIGNED -1

/**
 *
 */
#define NOT_ASSIGNED -2

/**
 *
 */
#define ASSIGNED_INPUT -3


/**
 *
 */
#define NOT_EQUAL -1


/**
 *
 */
#define TRUE 	1

/**
 *
 */
#define FALSE 	0

/*! \brief INFINIT definition. */
#define INF		1e20

/**
 *  @brief EPSILON definition.
 *
 *
 */
#define EPSILON 1e-8

#define SET_ALL 0xFFFFFFFF //!< Mask used to set all the bits in a vector field.

/**
 *
 */
#define MAX_LPS 64


/**
 *
 */
typedef char bool;

/**
 *
 */
typedef char *string;

/**
 *
 * @param len
 */
void *
checkedMalloc (unsigned long long len);

/**
 *
 * @param coeff
 * @param order
 * @return
 */
double
minPosRoot (double *coeff, int order) __attribute__((hot));

/*! \brief Advances the time of \f$ p(t) = a_0 + a_1 t + ... + a_{n} t^{n} \f$ 
 *
 * \param i Coefficient matrix index.
 * \param dt \f$ \Delta t \f$
 * \param p Coefficient matrix.
 * \param order Order of the polynomi.
 *
 * As a side effect, changes the values of the coefficient matrix.
 */
void
integrateState (int, double, double*, int) __attribute__((hot));

/*! \brief Evaluates \f$ p(t) = a_0 + a_1 t + ... + a_{n-1} t^{n-1} \f$ 
 *
 * \param i Coefficient matrix index.
 * \param dt \f$ \Delta t \f$
 * \param p Coefficient matrix.
 * \param order Order of the polynomi.
 * \return \f$ p(t+\Delta t) \f$ 
 */
double
evaluatePoly (int, double, double*, int) __attribute__((hot));

/**
 *
 * @param dt
 * @param p
 * @param order
 * @return
 */
double
evaluateVectorPoly (double dt, double *p, int order);

/**
 *
 * @param te
 */
#ifdef _WIN32
double
getTimeValue (struct timeval *te);
#else
double
getTimeValue (struct timespec *te);
#endif

/**
 *
 * @param te
 */
#ifdef _WIN32
void
getTime (struct timeval *te);
#else
void
getTime (struct timespec *te);
#endif

/**
 *
 * @param te
 */
#ifdef _WIN32
void
getTimeRes (struct timeval *te);
#else
void
getTimeRes (struct timespec *te);
#endif

/**
 *
 * @param v
 * @param u
 */
#ifdef _WIN32
void
subTime (struct timeval *v, struct timeval *u);
#else
void
subTime (struct timespec *v, struct timespec *u);
#endif

/*! \brief Sign function.
 *
 * If the value is 0, the function returns 1.
 */
int
sign (double x);

/** List data structures */

/** \brief Default node size for memory list
 *
 */
#define NODE_SIZE 10000

/** \brief Memory list pointer.
 *
 */
typedef struct node_ *node;

/** \brief Memory list node
 *
 *   A node is composed of a matrix that contains N arrays (N is the maximum number of values per node)
 *   where each of them can store up to M coefficients values.
 *
 *   \todo Replace \p val matrix for a vector of size \f$ N\timesM \f$
 */
struct node_
{
  double **val; //!< Storage matrix.
  double *timeVal; //!< Time-stamp of the i-th stored value.
  int used; //!< Pointer to the last value saved.
  struct node_ *next; //!< Pointer to the next node in the list.
};

/** \brief Memory list pointer
 *
 */
typedef struct list_ *list;

/** \brief Memory list
 *
 *   The memory list is used to log the output of the simulation. A new list is defined for each output expression in the simulation.
 *   If the simulation requires DENSE output on the state variables, then a coefficients of the state approximation polynomial are stored
 */
struct list_
{
  int nodeSize; //!< Maximum number of values that can be stored in a single node (the default value is NODE_SIZE).
  int order; //!< Number of coefficients of the approximation polynomial stored.
  node begin; //!< Pointer to the first element in the list.
  node end; //!< Pointer to the last element in the list.
};
/** \brief Memory list contructor.
 *
 * @param ns Maximum number of values that can be stored in a single node (the default value is NODE_SIZE).
 * @param ord Number of coefficients of the approximation polynomial stored.
 * @return Pointer to an allocated list.
 */
list
List (int ns, int ord);

/** \brief Adds a new element at the end of the given list.
 *
 * @param l Memory list.
 * @param t Time-stamp of the new value.
 * @param add Value of the simulation output.
 */
void
append (list l, double t, double *add);

/**
 *
 */
typedef struct vector_ *vector;

/**
 *
 */
struct vector_
{
  int *values; //!<
  int size; //!<
  int inc; //!<
  int used; //!<
  int iter; //!<
  bool ordered; //!<
};

/**
 *
 * @param cant
 * @return
 */
vector
Vector (int cant);

/**
 *
 * @param v
 */
void
freeVector (vector v);

/**
 *
 * @param v
 * @param value
 */
void
insert (vector v, int value);

/**
 *
 * @param v
 * @return
 */
int
vectorFirst(vector v);

/**
 *
 * @param v
 * @return
 */
int
vectorNext (vector v);

/**
 *
 * @param v
 * @return
 */
bool
vectorEnd (vector v);

/**
 *
 */
typedef vector *vMatrix;

/**
 *
 * @param rows
 * @param cols
 * @return
 */
vMatrix
VMatrix(int rows, int cols);

/**
 *
 * @param m
 * @param rows
 */
void
freeVMatrix(vMatrix m, int rows);

/**
 *
 */
#define ABS(X) ((X)>0.0 ? (X) : -(X))

/**
 *
 */
#define MAX(X,Y) ((X)>(Y) ? (X) : (Y))

/**
 *
 */
#define MIN2(X,Y) ((X)<(Y) ? (X) : (Y))

/**
 *
 */
#define LOG2(a)			(log10(a)/log10(2))

/**
 *
 */
#define DIV2(a)  		(a>>1)

/**
 *
 */
#define MUL2(a)  		(a<<1)

/**
 *
 */
#define MIN(a,b,c,d) 	((a)<=(c)?(b):(d))

/**
 *
 */
#define ODD				1

/* Bit vector data type */

/**
 *
 */
typedef uint32_t word_t;

/**
 *
 */
enum
{
  BITS_PER_WORD = 32//!< BITS_PER_WORD
};

/**
 *
 */
typedef struct BIT_vector_ *BIT_vector;

/**
 *
 */
struct BIT_vector_
{
  word_t *words; //!<
  word_t *resetMask; //!<
  int nwords; //!<
  int nbits; //!<
  int mask; //!<
  int word; //!<
};

/**
 *
 * @param bits
 * @return
 */
BIT_vector
BIT_Vector (int bits);

/**
 *
 * @param b
 * @param bit
 */
void
BIT_set (BIT_vector b, int bit);

/**
 *
 * @param b
 * @param bit
 */
void
BIT_clear (BIT_vector b, int bit);

/**
 *
 * @param b
 * @param bit
 */
unsigned long
BIT_isSet (BIT_vector b, int bit);

/**
 *
 * @param b
 */
void
BIT_freeVector (BIT_vector b);

/**
 *
 * @param b
 */
void
BIT_print (BIT_vector b);

/**
 *
 * @param b
 */
void
BIT_clearAll(BIT_vector b);

/**
 *
 * @param b
 */
void
BIT_setAll (BIT_vector b);

/**
 *
 * @param b
 * @return
 */
word_t
BIT_isAnySet(BIT_vector b);

/**
 *
 * @param i
 * @return
 */
word_t
BIT_numberOfSetBits(word_t i);

/**
 *
 * @param b
 * @return
 */
word_t
BIT_setBits(BIT_vector b);

/**
 *
 * @param b
 * @param bit
 */
void
BIT_setMask (BIT_vector b, int bit);

/**
 *
 */
typedef BIT_vector *BIT_matrix;

/**
 *
 * @param rows
 * @param cols
 * @return
 */
BIT_matrix
BIT_Matrix (int rows, int cols);

/**
 *
 * @param matrix
 * @param rows
 */
void
BIT_freeMatrix (BIT_matrix matrix, int rows);

/**
 *
 * @param v
 * @return
 */
int
BIT_first (BIT_vector v);

/**
 *
 * @param v
 * @return
 */
int
BIT_next (BIT_vector v);

#ifdef __linux__

/**
 * Message passing data structures for parallel simulators.
 */

/**
 *
 */
#define BUFFER_SIZE 10000

/**
 *
 */
#define MESSAGE_SIZE 1024

/**
 *
 */
typedef enum
{
  MSG_EVENT,//!< MSG_EVENT
  MSG_ACK   //!< MSG_ACK
} MSG_Type;

/**
 *
 */
typedef struct IBX_message_ IBX_message;

/**
 *
 */
struct IBX_message_
{
  int from; //!<
  int type; //!<
  int index; //!<
  int sendAck; //!<
  double value[MESSAGE_SIZE]; //!<
  double time; //!<
};


/**
 *
 */
typedef struct IBX_inbox_ *IBX_inbox;

/**
 *
 */
struct IBX_inbox_
{
  IBX_message *messages; //!<
  IBX_message *orderedMessages; //!<
  IBX_message *waiting; //!<
  BIT_vector waitingMessage; //!<
  BIT_vector waitingAck; //!<
  BIT_vector received; //!<
  pthread_mutex_t receivedMutex; //!<
  int *qMap;
  int states;
  int head; //!<
  int tail; //!<
  int size; //!<
};

/**
 *
 */
typedef struct MLB_mailbox_ *MLB_mailbox;

/**
 *
 */
struct MLB_mailbox_
{
  IBX_inbox **inbox; //!< Inbox data structure used to send and receive simulation events.
  int size; //!<
};

/**
 *
 * @param states
 * @param ack
 * @return
 */
IBX_inbox
IBX_Inbox(int states, int ack, int *qMap);

/**
 *
 * @param inbox
 */
void
IBX_freeInbox(IBX_inbox inbox);

/**
 *
 * @param inbox
 * @param from
 * @param message
 */
void
IBX_add (MLB_mailbox mailbox, IBX_inbox inbox, int from, int to, IBX_message message);

/**
 *
 * @param inbox
 * @param from
 */
void
IBX_ack(IBX_inbox inbox, int from);


/**
 *
 * @param inbox
 * @return
 */
word_t
IBX_checkMail(IBX_inbox inbox);

void
IBX_receiveMessages (IBX_inbox inbox);

void
IBX_receiveAndAckMessages (IBX_inbox inbox, MLB_mailbox mailbox, int id);

/**
 *
 * @param inbox
 */
void
IBX_checkInbox(IBX_inbox inbox);

/**
 *
 * @param inbox
 * @param mailbox
 * @param id
 */
void
IBX_checkAckInbox(IBX_inbox inbox, MLB_mailbox mailbox, int id);

/**
 *
 * @param inbox
 * @return
 */
word_t
IBX_ackMessages(IBX_inbox inbox);

/**
 *
 * @param inbox
 * @return
 */
double
IBX_nextMessageTime(IBX_inbox inbox);

/**
 *
 * @param inbox
 */
void
IBX_reset(IBX_inbox inbox);

/**
 *
 * @param
 * @return
 */
IBX_message
IBX_nextMessage(IBX_inbox);

/**
 *
 * @param inbox
 * @param dir
 */
void
IBX_close(IBX_inbox inbox, int dir);

/**
 *
 * @param lps
 * @return
 */
MLB_mailbox
MLB_Mailbox(int lps);

/**
 *
 * @param mailbox
 */
void
MLB_freeMailbox(MLB_mailbox mailbox);

/**
 *
 * @param mailbox
 * @param to
 * @param from
 * @param message
 */
void
MLB_send(MLB_mailbox mailbox, int to, int from, IBX_message message);

/**
 *
 * @param mailbox
 * @param to
 * @param from
 */
void
MLB_ack(MLB_mailbox mailbox, int to, int from);

/**
 *
 * @param mailbox
 * @param to
 * @param dir
 */
void
MLB_close(MLB_mailbox mailbox, int to, int dir);

#else

typedef struct IBX_message_ IBX_message;

/**
 *
 */
struct IBX_message_
{
  double dummy;
};

/**
 *
 */
typedef struct IBX_inbox_ *IBX_inbox;

/**
 *
 */
struct IBX_inbox_
{
  int dummy;
};

/**
 *
 */
typedef struct MLB_mailbox_ *MLB_mailbox;

/**
 *
 */
struct MLB_mailbox_
{
  int dummy;
};

MLB_mailbox
MLB_Mailbox(int lps);

/**
 *
 * @param mailbox
 */
void
MLB_freeMailbox(MLB_mailbox mailbox);

/**
 *
 * @param mailbox
 * @param to
 * @param from
 * @param message
 */
void
MLB_send(MLB_mailbox mailbox, int to, int from, IBX_message message);

/**
 *
 * @param mailbox
 * @param to
 * @param from
 */
void
MLB_ack(MLB_mailbox mailbox, int to, int from);

/**
 *
 * @param mailbox
 * @param to
 * @param dir
 */
void
MLB_close(MLB_mailbox mailbox, int to, int dir);

#endif

/**
 *
 * @param vector
 * @param value
 * @param size
 */
void
cleanVector (int *vector, int value, int size);

/**
 *
 * @param vector
 * @param value
 * @param size
 */
void
cleanDoubleVector (double *vector, int value, int size);

#ifdef SYNC_RT

void waitUntil(double until) ;

void setInitRealTime();

#endif

#endif  /* UTILS_H_ */
