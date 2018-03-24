#ifndef __TIMER_H_
#define __TIMER_H_

#define NUM_TIMERS 64

struct timer {
  unsigned on[NUM_TIMERS];
  double start[NUM_TIMERS];
  double elapsed[NUM_TIMERS];
};

struct timer *create_timer();
#define delete_timer(t) free(t)

void click_timer(struct timer *timer, int i);
double read_timer(struct timer *timer, int i);

#endif
