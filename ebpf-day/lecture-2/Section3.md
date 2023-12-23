In this part we will learn how to parse packet headers in ebpf.

In XDP when we get `ctx` we get the entire packet raw data. We need to parse the packet headers one by one.

* Parsing headers.

We can start by parsing the ethernet header as that is the outermost header of the packet. In our code we can do that by,

```
    struct ethhdr *eth = (struct ethhdr *) ctx->data;
```

We need to include these header files

```
#include <stddef.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/icmp.h>
#include <linux/icmpv6.h>
#include <linux/udp.h>
#include <linux/tcp.h>
```

Let's say we want to print the protocol of the next header, we can do that by writing

```
    struct ethhdr *eth = (struct ethhdr *) ctx->data;
    __u32 proto = eth->h_proto;
```

Can you tell if this code will work fine? What is the catch here?

Well first we need to convert `h_proto` to correct endianness by calling `bpf_ntohs()`

```
    struct ethhdr *eth = (struct ethhdr *) ctx->data;
    __u32 proto = bpf_ntohs(eth->h_proto);
```

Now if we just add this to our code the verifier will not be happy. You can check this yourself.

* Verifier bounds check.

In ebpf we cannot access any "unsafe" pointer. We need to specifically ensure that the pointer is valid before
using it.

We can make sure the pointer is valid by explicitly checking it in the code.

```
    struct ethhdr *eth = (struct ethhdr *) ctx->data;
    if ((void *)eth + sizeof(*eth) > data_end) {
        return XDP_DROP;
    }
    __u32 proto = bpf_ntohs(eth->h_proto);
```

* Counting the number of ICMP packets.

Now the rest of this section is left as an exercise to be performed in the class.

You need to

1. Parse packets till the IP header.
1. Check if the protocol is ICMP.
1. Maintain a count in the same map we used for last exercise.

But change the code so that it counts only the number of ICMP packets and maintains the bytes of those ICMP packets.

* Can you write code to parse VLAN tags from the packet?

Again left as an exercise.
