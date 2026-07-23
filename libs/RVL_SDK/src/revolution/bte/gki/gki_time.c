/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

/* RVL retail slice: only the 8 symbols in gki_time.s (ogws/Petari donors adapted). */

#define REVOLUTION 1
#include <revolution/BTE/gki/platform/gki_int.h>

#ifndef BT_ERROR_TRACE_0
#define BT_ERROR_TRACE_0(l, m)
#endif

#ifndef GKI_NUM_TIMERS
#error NO TIMERS: Must define at least 1 timer in the system!
#endif

#define GKI_NO_NEW_TMRS_STARTED (0x7fffffffL)
#define GKI_UNUSED_LIST_ENTRY (0x80000000L)
#define GKI_MAX_INT32 (0x7fffffffL)

void gki_timers_init(void) {
    UINT8 tt;

    gki_cb.com.OSTicksTilExp = 0;
    gki_cb.com.OSNumOrigTicks = 0;

    for (tt = 0; tt < GKI_MAX_TASKS; tt++) {
        gki_cb.com.OSWaitTmr[tt] = 0;

#if (GKI_NUM_TIMERS > 0)
        gki_cb.com.OSTaskTmr0[tt] = 0;
        gki_cb.com.OSTaskTmr0R[tt] = 0;
#endif
#if (GKI_NUM_TIMERS > 1)
        gki_cb.com.OSTaskTmr1[tt] = 0;
        gki_cb.com.OSTaskTmr1R[tt] = 0;
#endif
#if (GKI_NUM_TIMERS > 2)
        gki_cb.com.OSTaskTmr2[tt] = 0;
        gki_cb.com.OSTaskTmr2R[tt] = 0;
#endif
#if (GKI_NUM_TIMERS > 3)
        gki_cb.com.OSTaskTmr3[tt] = 0;
        gki_cb.com.OSTaskTmr3R[tt] = 0;
#endif
    }
}

UINT32 GKI_get_tick_count(void) {
    return gki_cb.com.OSTicks;
}

void GKI_start_timer(UINT8 tnum, INT32 ticks, BOOLEAN is_continuous) {
    INT32 reload;
    INT32 orig_ticks;
    UINT8 task_id = GKI_get_taskid();
    BOOLEAN bad_timer = FALSE;

    if (ticks <= 0) {
        ticks = 1;
    }

    orig_ticks = ticks;

    if (is_continuous) {
        reload = ticks;
    } else {
        reload = 0;
    }

    GKI_disable();

    if (GKI_MAX_INT32 - (gki_cb.com.OSNumOrigTicks - gki_cb.com.OSTicksTilExp) > ticks) {
        ticks += gki_cb.com.OSNumOrigTicks - gki_cb.com.OSTicksTilExp;
    } else {
        ticks = GKI_MAX_INT32;
    }

    switch (tnum) {
#if (GKI_NUM_TIMERS > 0)
    case TIMER_0:
        gki_cb.com.OSTaskTmr0R[task_id] = reload;
        gki_cb.com.OSTaskTmr0[task_id] = ticks;
        break;
#endif
#if (GKI_NUM_TIMERS > 1)
    case TIMER_1:
        gki_cb.com.OSTaskTmr1R[task_id] = reload;
        gki_cb.com.OSTaskTmr1[task_id] = ticks;
        break;
#endif
#if (GKI_NUM_TIMERS > 2)
    case TIMER_2:
        gki_cb.com.OSTaskTmr2R[task_id] = reload;
        gki_cb.com.OSTaskTmr2[task_id] = ticks;
        break;
#endif
#if (GKI_NUM_TIMERS > 3)
    case TIMER_3:
        gki_cb.com.OSTaskTmr3R[task_id] = reload;
        gki_cb.com.OSTaskTmr3[task_id] = ticks;
        break;
#endif
    default:
        bad_timer = TRUE;
        break;
    }

    /* Inline gki_adjust_timer_count (not a separate retail symbol). */
    if (!bad_timer && orig_ticks > 0) {
        if (gki_cb.com.OSNumOrigTicks == 0 ||
            (orig_ticks < gki_cb.com.OSTicksTilExp && gki_cb.com.OSTicksTilExp > 0)) {
            gki_cb.com.OSNumOrigTicks =
                gki_cb.com.OSNumOrigTicks - gki_cb.com.OSTicksTilExp + orig_ticks;
            gki_cb.com.OSTicksTilExp = orig_ticks;
        }
    }

    GKI_enable();
}

void GKI_stop_timer(UINT8 tnum) {
    UINT8 task_id = GKI_get_taskid();

    switch (tnum) {
#if (GKI_NUM_TIMERS > 0)
    case TIMER_0:
        gki_cb.com.OSTaskTmr0R[task_id] = 0;
        gki_cb.com.OSTaskTmr0[task_id] = 0;
        break;
#endif
#if (GKI_NUM_TIMERS > 1)
    case TIMER_1:
        gki_cb.com.OSTaskTmr1R[task_id] = 0;
        gki_cb.com.OSTaskTmr1[task_id] = 0;
        break;
#endif
#if (GKI_NUM_TIMERS > 2)
    case TIMER_2:
        gki_cb.com.OSTaskTmr2R[task_id] = 0;
        gki_cb.com.OSTaskTmr2[task_id] = 0;
        break;
#endif
#if (GKI_NUM_TIMERS > 3)
    case TIMER_3:
        gki_cb.com.OSTaskTmr3R[task_id] = 0;
        gki_cb.com.OSTaskTmr3[task_id] = 0;
        break;
#endif
    default:
        break;
    }
}

void GKI_init_timer_list(TIMER_LIST_Q* p_timer_listq) {
    p_timer_listq->p_first = NULL;
    p_timer_listq->p_last = NULL;
    p_timer_listq->last_ticks = 0;
}

UINT16 GKI_update_timer_list(TIMER_LIST_Q* p_timer_listq, INT32 num_units_since_last_update) {
    TIMER_LIST_ENT* p_tle;
    UINT16 num_time_out = 0;
    INT32 rem_ticks;
    INT32 temp_ticks;

    p_tle = p_timer_listq->p_first;

    while ((p_tle != NULL) && (p_tle->ticks <= 0)) {
        num_time_out++;
        p_tle = p_tle->p_next;
    }

    rem_ticks = num_units_since_last_update;

    while ((p_tle != NULL) && (rem_ticks > 0)) {
        temp_ticks = p_tle->ticks;
        p_tle->ticks -= rem_ticks;

        if (p_tle->ticks <= 0) {
            p_tle->ticks = 0;
            num_time_out++;
        }

        rem_ticks -= temp_ticks;
        p_tle = p_tle->p_next;
    }

    if (p_timer_listq->last_ticks > 0) {
        p_timer_listq->last_ticks -= num_units_since_last_update;

        if (p_timer_listq->last_ticks < 0) {
            p_timer_listq->last_ticks = 0;
        }
    }

    return num_time_out;
}

void GKI_add_to_timer_list(TIMER_LIST_Q* p_timer_listq, TIMER_LIST_ENT* p_tle) {
    UINT32 nr_ticks_total;
    TIMER_LIST_ENT* p_temp;

    if (p_tle->ticks >= 0) {
        if (p_tle->ticks >= p_timer_listq->last_ticks) {
            if (p_timer_listq->p_first == NULL) {
                p_timer_listq->p_first = p_tle;
            } else {
                if (p_timer_listq->p_last != NULL) {
                    p_timer_listq->p_last->p_next = p_tle;
                }
                p_tle->p_prev = p_timer_listq->p_last;
            }

            p_tle->p_next = NULL;
            p_timer_listq->p_last = p_tle;
            nr_ticks_total = p_tle->ticks;
            p_tle->ticks -= p_timer_listq->last_ticks;
            p_timer_listq->last_ticks = nr_ticks_total;
        } else {
            p_temp = p_timer_listq->p_first;
            while (p_tle->ticks > p_temp->ticks) {
                if (p_temp->ticks > 0) {
                    p_tle->ticks -= p_temp->ticks;
                }
                p_temp = p_temp->p_next;
            }

            if (p_temp == p_timer_listq->p_first) {
                p_tle->p_next = p_timer_listq->p_first;
                p_timer_listq->p_first->p_prev = p_tle;
                p_timer_listq->p_first = p_tle;
            } else {
                p_temp->p_prev->p_next = p_tle;
                p_tle->p_prev = p_temp->p_prev;
                p_temp->p_prev = p_tle;
                p_tle->p_next = p_temp;
            }
            p_temp->ticks -= p_tle->ticks;
        }

        p_tle->in_use = TRUE;
    }
}

void GKI_remove_from_timer_list(TIMER_LIST_Q* p_timer_listq, TIMER_LIST_ENT* p_tle) {
    if (p_tle == NULL || p_tle->in_use == FALSE || p_timer_listq->p_first == NULL) {
        return;
    }

    if (p_tle->p_next != NULL) {
        p_tle->p_next->ticks += p_tle->ticks;
    } else {
        p_timer_listq->last_ticks -= p_tle->ticks;
    }

    if (p_timer_listq->p_first == p_tle) {
        p_timer_listq->p_first = p_tle->p_next;

        if (p_timer_listq->p_first != NULL) {
            p_timer_listq->p_first->p_prev = NULL;
        }

        if (p_timer_listq->p_last == p_tle) {
            p_timer_listq->p_last = NULL;
        }
    } else {
        if (p_timer_listq->p_last == p_tle) {
            p_timer_listq->p_last = p_tle->p_prev;

            if (p_timer_listq->p_last != NULL) {
                p_timer_listq->p_last->p_next = NULL;
            }
        } else {
            if (p_tle->p_next != NULL && p_tle->p_next->p_prev == p_tle) {
                p_tle->p_next->p_prev = p_tle->p_prev;
            } else {
                return;
            }

            if (p_tle->p_prev != NULL && p_tle->p_prev->p_next == p_tle) {
                p_tle->p_prev->p_next = p_tle->p_next;
            } else {
                return;
            }
        }
    }

    p_tle->p_next = p_tle->p_prev = NULL;
    p_tle->ticks = GKI_UNUSED_LIST_ENTRY;
    p_tle->in_use = FALSE;
}
