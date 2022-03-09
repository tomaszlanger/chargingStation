#ifndef _TIMER_H_
#define _TIMER_H_

typedef void (*TIMER_CALBACK)(void);

void initTimer1(TIMER_CALBACK callback);

#endif  // _TIMER_H_