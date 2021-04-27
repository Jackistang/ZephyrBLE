#include "utils.h"

#define LOG_TAG     "zephyr.utils"
#define LOG_LVL     LOG_LVL_DBG
#include <ulog.h>

void k_sem_give(struct k_sem *sem)
{
    register rt_base_t temp;
    register rt_bool_t need_schedule;

    /* parameter check */
    RT_ASSERT(sem != RT_NULL);
    RT_ASSERT(rt_object_get_type(&sem->parent.parent) == RT_Object_Class_Semaphore);

    RT_OBJECT_HOOK_CALL(rt_object_put_hook, (&(sem->parent.parent)));

    need_schedule = RT_FALSE;

    /* disable interrupt */
    temp = rt_hw_interrupt_disable();

    RT_DEBUG_LOG(RT_DEBUG_IPC, ("thread %s releases sem:%s, which value is: %d\n",
                                rt_thread_self()->name,
                                ((struct rt_object *)sem)->name,
                                sem->value));

    /* Add limit condition. */
    if (sem->value < sem->limit)
    {
        if (!rt_list_isempty(&sem->parent.suspend_thread))
        {
            /* resume the suspended thread */
            rt_ipc_list_resume(&(sem->parent.suspend_thread));
            need_schedule = RT_TRUE;
        }
        else
            sem->value ++; /* increase value */
    }

    /* enable interrupt */
    rt_hw_interrupt_enable(temp);

    /* resume a thread, re-schedule */
    if (need_schedule == RT_TRUE)
        rt_schedule();

    return ;
}