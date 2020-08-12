/* 
    Created by Alessandro Giordano on 03/11/2018ù

    Libreria di comunicazione rete con il programma desktop

*/

#ifndef TCP_UTILS
#define TCP_UTILS

#ifdef __cplusplus
extern "C" {
#endif


#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "tcpip_adapter.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>


#define TCP_PORT 3000

static ip4_addr_t s_ip_addr;


/* 
    Connection status flag

    polling into connected status bit to wait wifi connection

    0 -> Ip address assegnato
    MACROS -> others (https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/event-handling.html)
*/
static int32_t connected = -1;

static char ssid[32];
static char password[64];

void wifiConnect();
void tcpServerConnect();
void wifiDisconnect();
void tcpStopTask();
void sendTcpData(char * buffer, int len);
void registerRxCallback(void ( * rx_callback)(char * , size_t));

#ifdef __cplusplus
}
#endif

#endif