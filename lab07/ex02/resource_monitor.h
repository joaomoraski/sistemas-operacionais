#include <pthread.h>

/* --- ex02 operations --- */
void initMonitor();
void destroyMonitor();
int put(int thread_id);
int take();
