~~~
＊ CSci4061 F2016 Assignment 4
＊ login: zhou0745
＊ date: 12/11/2016
＊ name: Tiannan Zhou, Annelies Odermann
＊ id: 5232494 (zhou0745), 4740784 (oderm008)
~~~

**How to compile the program.**

~~~
make
~~~

You just need to use GNU makefile to compile this project.

**Who did what on the program**
* Tiannan Zhou
	* Completed framework and extra credits part
* Annelies Odermann
	* Completed log system and debug

**Usage**
~~~
./web_server port_# path num_dispatch num_worker queue_len cache_size
~~~

The server would be automatically started after run the command above.

Log information will be stored in a file called 'web_server_log' in the format of `[ThreadID][RequestID][fd][Request_string][bytes/error][time_cost][Cache_status]`.
