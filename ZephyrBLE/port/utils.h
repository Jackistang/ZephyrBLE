#ifndef __ZEPHYR_BLE_PORT_UTILS_H__
#define __ZEPHYR_BLE_PORT_UTILS_H__

#include <rtthread.h>

#define MSEC_PER_SEC    RT_TICK_PER_SECOND
#define K_MSEC(t)       (t)
#define K_SECONDS(s)    K_MSEC((s) * MSEC_PER_SEC)

typedef rt_int32_t k_timeout_t;
#define K_NO_WAIT   RT_WAITING_NO
#define K_FOREVER   RT_WAITING_FOREVER

/**
 * @brief k_sem, use RT-Thread rt_semaphore wrapper it and add `limit` feature.
*/
struct k_sem {
    struct rt_ipc_object parent;                        /**< inherit from ipc_object */

    rt_uint16_t          value;                         /**< value of sem. */
    rt_uint16_t          limit;                         /**< limit of sem. */
};

#define k_sem_init(sem, initial_count, limit) do {\
        sem->limit = limit; \
        rt_sem_init((rt_sem_t)sem, #sem, initial_count, RT_IPC_FLAG_FIFO);  \
    } while(0)

rt_inline int k_sem_take(struct k_sem *sem, k_timeout_t timeout)
{
    return rt_sem_take((rt_sem_t)sem, timeout);
}

void k_sem_give(struct k_sem *sem);


/**
 * @brief k_fifo, use RT-Thread mailbox wrapper it.
*/
#define k_fifo  rt_mailbox
#define CONFIG_K_FIFO_SIZE  20

#define k_fifo_init(fifo)   do {\
    fifo = rt_mb_create(#fifo, CONFIG_K_FIFO_SIZE, RT_IPC_FLAG_FIFO);   \
    if (fifo == NULL)   \
        rt_kprintf("Create mailbox %s error\n", #fifo); \
}

#define k_fifo_put(fifo, data)  \
    rt_mb_send(fifo, (rt_ubase_t)data)

rt_inline void* k_fifo_get(struct k_fifo *fifo, k_timeout_t timeout)
{
    rt_uint32_t* value = NULL;
    rt_mb_recv(fifo, value, timeout);
    return value;
}

/**
 * @brief k_work, use RT-Thread rt_work wrapper it.
*/
#define k_work  rt_work
#define k_delayed_work rt_delayed_work
typedef void (*k_work_handler_t)(struct k_work *work, void *work_data);
typedef rt_tick_t k_ticks_t

rt_inline void k_work_init(struct k_work *work, k_work_handler_t handler)
{
    rt_work_init(work, handler, RT_NULL);
}

rt_inline void k_work_submit(struct k_work *work)
{
    rt_work_submit(work, 0);
}

rt_inline void k_delayed_work_init(struct k_delayed_work *work, k_work_handler_t handler)
{
    rt_delayed_work_init(work, handler, NULL);
}

rt_inline int k_delayed_work_submit(struct k_delayed_work *work, k_timeout_t delay)
{
    return rt_work_submit(work, delay);
}

/**
 * @brief May have a problem.
*/
rt_inline k_ticks_t k_delayed_work_remaining_get(const struct k_delayed_work *work)
{
    return work->work.timer.timeout_tick - rt_tick_get();
}

#endif /* __ZEPHYR_BLE_PORT_UTILS_H__ */
