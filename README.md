# udp_echo

1. server
    a. ./server "server IP" "server Port" 開啟server
    b. 呼叫 socket() bind() 來建立 socket
    c. 建立 thread 來接收 client 封包
    d. 
