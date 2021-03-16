#include "contiki.h"
#include "net/uip.h"
#include "net/uip-ds6.h"

#include "simple-udp.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "messenger-app.h"

#define UDP_PORT 1234

static struct simple_udp_connection broadcast_connection;
char *username;

/*---------------------------------------------------------------------------*/
PROCESS(shell_sendmessage_process, "send a message");
SHELL_COMMAND(sendmessage_command,
	      "send",
	      "send <message>: send a message through the udp connection",
	      &shell_sendmessage_process);
/*---------------------------------------------------------------------------*/

static void receiver(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
  printf("%s\n", data);
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(shell_sendmessage_process, ev, data)
{
  char* message;
  char* input;

  uip_ipaddr_t addr;

  PROCESS_BEGIN();
  simple_udp_register(&broadcast_connection, UDP_PORT,
                      NULL, UDP_PORT,
                      receiver);

  input = data;

  if (input == NULL || strlen(input) == 0)
  {
    shell_output_str(&sendmessage_command, 
    	"send <message>: A message must be given", "");
    PROCESS_EXIT();
  }

  if (username == NULL || strlen(username) == 0)
  {
    shell_output_str(&sendmessage_command,
    	"send <message>: A username must be set first", "");
    PROCESS_EXIT();
  }

  message = malloc(sizeof(char) * (strlen(input) + strlen(username) + 2));
  strcat(message, username);
  strcat(message, ": ");
  strcat(message, input);
  shell_output_str(&sendmessage_command, "message:\n", message);

  uip_create_linklocal_allnodes_mcast(&addr);
  simple_udp_sendto(&broadcast_connection, message, strlen(message), &addr);

  free(message);   
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
void shell_sendmessage_init(void)
{
  shell_register_command(&sendmessage_command);
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
PROCESS(shell_setname_process, "Set an username");
SHELL_COMMAND(setname_command,
	      "setname",
	      "setname <username>: set the user's name for chatting",
	      &shell_setname_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(shell_setname_process, ev, data)
{
  PROCESS_BEGIN();

  username = malloc(sizeof(char)*50);
  strncpy(username, data, 50);
  
  if (username == NULL || strlen(username) == 0)
  {
    shell_output_str(&setname_command, 
    	"setname <username>: an user name must be given", "");
    PROCESS_EXIT();
  }

  shell_output_str(&setname_command, "Your username is: ", username);

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
void
shell_setname_init(void)
{
  shell_register_command(&setname_command);
}
/*---------------------------------------------------------------------------*/

