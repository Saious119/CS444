I used mutex locks both for UNIX and WINDOWS sections. 

I used mutexes to protect the reading for if fifo is full or not and again to check if its empty or not.
I also used it to protect queueAdd in producer and queueDel in consumer.