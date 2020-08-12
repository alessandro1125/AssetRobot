#include "TcpUtils.h" 

static const char *TAG = "TCP_UTILS";

static char tx_buffer[1024];
static int txLen = 0;

static char rx_buffer[1024];
static int rxLen = 0;

void ( * tcp_rx_handler)(char * , size_t); // Callback for rx recived bytes

static TaskHandle_t tcpTask = NULL;

static void tcp_server_task(void *pvParameters)
{
    char addr_str[128];
    int addr_family;
    int ip_protocol;

    while (1) {

        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(TCP_PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;
        inet_ntoa_r(dest_addr.sin_addr, addr_str, sizeof(addr_str) - 1);


        int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
        if (listen_sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket created");

        int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err != 0) {
            ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket bound, port %d", TCP_PORT);

        err = listen(listen_sock, 1);
        if (err != 0) {
            ESP_LOGE(TAG, "Error occurred during listen: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket listening");

        struct sockaddr_in6 source_addr; // Large enough for both IPv4 or IPv6
        uint addr_len = sizeof(source_addr);
        int sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket accepted");

        while (1) {
            int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
            // Error occurred during receiving
            if (len < 0) {
                ESP_LOGE(TAG, "recv failed: errno %d", errno);
                break;
            }
            // Connection closed
            else if (len == 0) {
                ESP_LOGI(TAG, "Connection closed");
                break;
            }
            // Data received
            else {
                // Get the sender's ip address as string
                if (source_addr.sin6_family == PF_INET) {
                    inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr.s_addr, addr_str, sizeof(addr_str) - 1);
                } else if (source_addr.sin6_family == PF_INET6) {
                    inet6_ntoa_r(source_addr.sin6_addr, addr_str, sizeof(addr_str) - 1);
                }

                rxLen = len;
                //if(tcp_rx_handler!=NULL)
                tcp_rx_handler(rx_buffer,len);

                //rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
                
                //ESP_LOGI(TAG, "Received %d bytes from %s:", len, addr_str);
                //ESP_LOGI(TAG, "%s", rx_buffer);

                /*
                int k = 0;
                int re = 0;
                for(k = 0; k < txLen; k++) {
                    re += tx_buffer[k];
                }*/

                //ESP_LOGI(TAG, "Sending : %d, len: %d", re, txLen);
                int err = send(sock, tx_buffer, txLen, 0);
                if (err < 0) {
                    ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                    break;
                }
            }
        }

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
            shutdown(listen_sock,0);
            close(listen_sock);
            vTaskDelay(5);
        }
    }
    connected = -1;
    wifiDisconnect();
    vTaskDelete(NULL);
}

void sendTcpData(char * buffer, int len) {
    //strncpy(tx_buffer, buffer, len);

    for(int i = 0; i < len; i++) {
        tx_buffer[i] = buffer[i];
        //ESP_LOGI("TEST_DATA", "%x", tx_buffer[i]);
    }

    txLen = len;
}

static void on_wifi_event(void *arg, esp_event_base_t event_base,
                            int32_t event_id, void *event_data)
{

    if(event_id == WIFI_EVENT_STA_CONNECTED)
        ESP_LOGI(TAG, "Connected");
    if(event_id == WIFI_EVENT_STA_DISCONNECTED)
        ESP_LOGI(TAG, "Sta disconnected");
    if(event_id == WIFI_EVENT_STA_WPS_ER_FAILED)
        ESP_LOGI(TAG, "Sta Failed");
    if(event_id == WIFI_EVENT_STA_WPS_ER_TIMEOUT)
        ESP_LOGI(TAG, "Sta Timeout");
    
    connected = event_id;
}

static void on_got_ip(void *arg, esp_event_base_t event_base,
                      int32_t event_id, void *event_data)
{
    ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
    memcpy(&s_ip_addr, &event->ip_info.ip, sizeof(s_ip_addr));
    ESP_LOGI(TAG, "IPv4 address: " IPSTR, IP2STR(&s_ip_addr));
    connected = 0;
    xTaskCreate(tcp_server_task, "tcp_server", 4096*2, NULL, 5, &tcpTask);
}

void wifiDisconnect() {
    esp_wifi_disconnect();
    esp_wifi_stop();
}

void tcpStopTask() {
    if( tcpTask != NULL ) {
     vTaskDelete( tcpTask );
    }
}

void wifiConnect() {
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_CONNECTED, &on_wifi_event, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &on_wifi_event, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_WPS_ER_FAILED, &on_wifi_event, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_WPS_ER_TIMEOUT, &on_wifi_event, NULL));

    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &on_got_ip, NULL));

    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "",
            .password = "",
        },
    };

    memcpy(wifi_config.sta.ssid, ssid, 32);
    memcpy(wifi_config.sta.password, password, 64);

    ESP_LOGI(TAG, "Connecting to %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());
}

void registerRxCallback(void ( * rx_callback)(char * , size_t)) {
    tcp_rx_handler = rx_callback;
}

void tcpServerConnect() {
    ESP_ERROR_CHECK(nvs_flash_init());
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /*
    TMP
    */
    sprintf(ssid, "GIORDANO-HOUSE");
    sprintf(password, "Cicciobombo");

    /*
    sprintf(ssid, "iPhone di Alessandro");
    sprintf(password, "pass1125");
    */

    wifiConnect();
}