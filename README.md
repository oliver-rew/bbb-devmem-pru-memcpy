# bbb-devmem-copy
This is a simple example of `memcpy()`ing from PRU shared ram (0x4a310000) by `mmap()`ing `/dev/mem`. This works fine(I have some PRU code that write this address and I can successfully read it off) but it is awfully slow. I am getting ~25MB/s and from what I read online, this should be MUCH faster. 
