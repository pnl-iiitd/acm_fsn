   What good is ebpf if we cannot get data out from it, well very limited to be honest. So lets make our ebpf more useful by working with maps.

    ebpf Maps are datastructures which can be used by the kernel space and user space code both, its a bidirectional channel between kernel and
    user space. 

* In this exercise we will add a single map to our code by adding this piece of code.

    ```
    #define MAXELEM 1

    struct {
        __uint(type, BPF_MAP_TYPE_HASH);
        __type(key, __u32);
        __type(value, __u64);
        __uint(max_entries, MAXELEM);
    } pkt_cnt SEC(".maps");`
    ```

    This map is called `pkt_cnt` map which counts number of packets passing through the interface. The key and value sizes are loaded in the map definition.
    `PIN_GLOBAL_NS` tells libbpf library (ebpf helper functions) to pin this map in the global namespace for us to access easily.
    The number of elements is set to 1.

    How do we update this map?

    Well first we need to get the value which exists in this map. If the value is not found we can initilaize it to zero.

    ```
    __u32 key = 0; //We have only one element.
    __u64 *cnt = bpf_map_lookup_elem(&pkt_cnt, &key);
    if (!cnt) {

        /* How do we initialize the count variable? */
    } else {
        // Increment the count variable
    }
    ```

    The filled in code looks like

    ```
    SEC("xdp-pass")
    int  xdp_allow(struct xdp_md *ctx)
    {
        __u32 key = 0; //We have only one element.
        __u64 *cnt = bpf_map_lookup_elem(&pkt_cnt, &key);
        if (!cnt) {
            // Fill in the class
        } else {
            *cnt += 1;
        }

        bpf_printk("You can go now");
        return XDP_PASS;
    }

    char _license[] SEC("license") = "GPL";
    ```

    Hopefully at this point you guys have added the map, completed the code to initialize count.

    Now you can again detach the xdp program and load it again. This time we should see our map with the program.

* How do we see our ebpf maps?

    Well we can use `bpftool` again to see our maps.

    You can run 

    ```
    $ bpftool map show
        35752: hash  name pkt_cnt  flags 0x0
        key 4B  value 8B  max_entries 1  memlock 4096B 
        btf_id 297
    ```

    Which shows our map `pkt_cnt` after the program is loaded.

    We can also use `bpftool` to pin the maps just like we pinned the programs like,

    ```
    root@tcnode6:/home/dushyant/tutor/lecture-2# bpftool map pin id 35752 /sys/fs/bpf/ebpf/pkt_cnt
    root@tcnode6:/home/dushyant/tutor/lecture-2# ls /sys/fs/bpf/ebpf/
    pkt_cnt  xdp_allow
    ```

    Finally we can view what is present in our ebpf map using `bpftool` as well.

    ```
    $ bpftool map dump pinned /sys/fs/bpf/ebpf/pkt_cnt
    [] 
    ```

    Which currently shows an empty dictionary.

* Lets attach the ebpf program

    We can now attach the ebpf program, run some traffic via ping and see the values in map again.

    After doing the above, your map should look like,

    ```
    $ bpftool map dump pinned /sys/fs/bpf/ebpf/pkt_cnt
    [{
        "key": 0,
        "value": 8
    }]
    ```

    Showing that the value has incremented.

### Exercise 1: Extend the map.

* This is left as an exercise so we have not included here, you should extend the map now to also show the number of
bytes seen at the xdp program in the same key.

Maybe some questions to answer are.

1. How do we put two values in the same ebpf map element?

1. How do we get the number of bytes passed? How do we get the number of bytes in each packet.

1. How do we incrementally both values, the count of packets and number of bytes?

Lets solve this in classroom together.
