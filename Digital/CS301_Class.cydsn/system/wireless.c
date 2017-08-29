CY_ISR(rf_rx) {

}

void wireless_init() {
    UART_Start();
    isrRF_RX_StartEx(rf_rx);
}
