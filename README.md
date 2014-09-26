Hypernode PHP module
====================

This is a PHP module for some enhanced functionality.

Currently, it does only one thing: it won't process fastcgi requests
when the client has already closed the connection.

Requests are queued by the Linux kernel and placed in a TCP backlog.
Once an FPM worker becomes available, it gets the next queued request
and starts processing, regardless of the state of the connection with
the client.

This module checks if the client is still connected before processing
the requests and skips it when the client is already gone.

The behavior can be controlled by the `hypernode.kill_gone_requests`
setting. To disable the behavior for a specific directory, place a file
called `.user.ini` in it with the following contents:

```
hypernode.kill_gone_requests = 0
```
