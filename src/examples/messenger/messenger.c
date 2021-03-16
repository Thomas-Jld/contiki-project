#include <stdio.h>
#include "contiki.h"
#include "dev/serial-line.h"
#include "shell.h"
#include "serial-shell.h"
#include "messenger-app.h"

PROCESS(messenger_process,"Messenger_process");
AUTOSTART_PROCESSES(&messenger_process);
PROCESS_THREAD(messenger_process,ev,data)
{
PROCESS_BEGIN();
serial_line_init();
serial_shell_init();

shell_reboot_init();
shell_power_init();
shell_ps_init();
shell_ping_init();

shell_sendmessage_init();
shell_setname_init();

PROCESS_END();
}
