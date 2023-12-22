# Simple Echo Server

Instructions:

```
gcc echo.c -o echo
./echo
```

In another tab:
```
echo -n "Hello" | nc localhost 9091
```

returns Hello
