#include <sys/time.h>

#include "common.h"
#include "timer.h"

#define US 1e-6

static struct timeval _tv;

struct timer *create_timer() {
  struct timer *timer =
    (struct timer *) malloc(sizeof(struct timer));

  throw(!timer, "faild to allocate memory for timer.");

  for (int i = 0; i < NUM_TIMERS; i++) {
    timer->on[i] = 0;
    timer->start[i] = 0.0;
    timer->elapsed[i] = 0.0;
  }

  return timer;
}

void click_timer(struct timer *timer, int i) {
  gettimeofday(&_tv, NULL);
  double current = (double) _tv.tv_sec
                 + (double) _tv.tv_usec * US;

  if (timer->on[i]) {
    timer->elapsed[i] += (current - timer->start[i]);
    timer->on[i] = 0;
  }
  else {
    timer->start[i] = current;
    timer->on[i] = 1;
  }
}

double read_timer(struct timer *timer, int i) {
  return timer->elapsed[i];
}
