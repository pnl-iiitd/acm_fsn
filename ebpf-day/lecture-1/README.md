# Lecture 1 - ACM Full Stack Networking Winter School.

# Setup

## Install the tools

From the repository

`sudo apt install linux-headers-$(uname -r) bpfcc-tools`

Check the tools

`ls /usr/sbin | grep bpfcc`

You should see a number of tools with `bpfcc` as a suffix.

# Testing ebpf tools.

## Trace new executing processes

Snoop on the new processes being created in the system. 
Hooks on the `exec` syscall and reports which processes are opening.

`$ execsnoop-bpfcc`

You can create any process and see it running in the terminal.

## Trace VFS stats 

Get VFS (Virtual File System) Read/Write Stats.

Hooks on the vfs_read/vfs_write/vfs_create etc

`$ vfsstat-bpfcc`

Look at the Code:

https://github.com/iovisor/bcc/blob/master/libbpf-tools/vfsstat.bpf.c

## Trace tcp connections 

Trace new forming tcp connections.

`tcpconnect-bpfcc`

Trace active TCP connections via connect() system call, 
by tracing the kernel tcp_v4_connect() and tcp_v6_connect()

## Trace ICMP Packet count and its type

`trace-bpfcc -t 'icmp_out_count "type: %d" arg2'`

Here we are specifying the function to trace. Its argument to print
and the type of argument to decode.
