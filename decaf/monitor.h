#ifndef MONITOR_H
#define MONITOR_H

#include "qemu-common.h"
#include "qemu-char.h"
#include "qerror.h"
#include "qdict.h"
#include "block.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

extern Monitor *cur_mon;
extern Monitor *default_mon;

/* flags for monitor_init */
#define MONITOR_IS_DEFAULT    0x01
#define MONITOR_USE_READLINE  0x02
#define MONITOR_USE_CONTROL   0x04
#define MONITOR_USE_PRETTY    0x08

/* flags for monitor commands */
#define MONITOR_CMD_ASYNC       0x0001

/* QMP events */
typedef enum MonitorEvent {
    QEVENT_SHUTDOWN,
    QEVENT_RESET,
    QEVENT_POWERDOWN,
    QEVENT_STOP,
    QEVENT_RESUME,
    QEVENT_VNC_CONNECTED,
    QEVENT_VNC_INITIALIZED,
    QEVENT_VNC_DISCONNECTED,
    QEVENT_BLOCK_IO_ERROR,
    QEVENT_RTC_CHANGE,
    QEVENT_WATCHDOG,
    QEVENT_SPICE_CONNECTED,
    QEVENT_SPICE_INITIALIZED,
    QEVENT_SPICE_DISCONNECTED,
    QEVENT_MAX,
} MonitorEvent;

int monitor_cur_is_qmp(void);

void monitor_protocol_event(MonitorEvent event, QObject *data);
void monitor_init(CharDriverState *chr, int flags);

int monitor_suspend(Monitor *mon);
void monitor_resume(Monitor *mon);

int monitor_read_bdrv_key_start(Monitor *mon, BlockDriverState *bs,
                                BlockDriverCompletionFunc *completion_cb,
                                void *opaque);

int monitor_get_fd(Monitor *mon, const char *fdname);

void monitor_vprintf(Monitor *mon, const char *fmt, va_list ap)
    GCC_FMT_ATTR(2, 0);
void monitor_printf(Monitor *mon, const char *fmt, ...) GCC_FMT_ATTR(2, 3);
void monitor_print_filename(Monitor *mon, const char *filename);
void monitor_flush(Monitor *mon);
int monitor_set_cpu(int cpu_index);
int monitor_get_cpu_index(void);

typedef void (MonitorCompletion)(void *opaque, QObject *ret_data);

void monitor_set_error(Monitor *mon, QError *qerror);

// AWH - moved here from monitor.c so that the plugins can use it
typedef struct mon_cmd_t {
    const char *name;
    const char *args_type;
    const char *params;
    const char *help;
    void (*user_print)(Monitor *mon, const QObject *data);
    union {
        void (*info)(Monitor *mon);
        void (*cmd)(Monitor *mon, const QDict *qdict);
        int  (*cmd_new)(Monitor *mon, const QDict *params, QObject **ret_data);
        int  (*cmd_async)(Monitor *mon, const QDict *params,
                          MonitorCompletion *cb, void *opaque);
    } mhandler;
    bool qapi;
    int flags;
} mon_cmd_t;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* !MONITOR_H */
