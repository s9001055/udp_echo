# udp_echo

1. Server
    a. ./server "server IP" "server Port" 開啟server
    b. 呼叫 socket() bind() 來建立 socket
    c. 建立 thread 來接收 client 封包
    d. thread 使用 recvfrom() 來等待封包送達 server
    e. 當 thread 收到封包後，將 msg 使用 sendto() 送回 Client
    f. server 可輸入 exit 來關閉 server

2. Client
    a. ./client "server IP" "server Port" 來開啟 client
    b. 呼叫 socket() 來與 server 建立 socket
    c. 使用 fgets() 來輸入 msg
    d. 輸入後使用 sendto() 發送第一次封包
    e. create thread 來等待 server 回傳 msg
        (a) 使用 isRecv 全域變數來判斷是否收到 server 的 echo msg 
            是: isRecv = 1
            否: isRecv = -1

    f. 進入 waitFromServer() 來執行 retry
        (a) 沒收到 server echo msg 時，執行 exponential backoff algorithm
        (b) exponential backoff algorithm:
            I.   multiplier = 2
            II.  base = 500ms
            III. max wait_interval = 8s
        (c) 時間到時重新傳送 msg 給 server
        (d) 當收到 echo msg 回傳 0，否則回傳 1
