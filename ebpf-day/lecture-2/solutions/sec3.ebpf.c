/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

#include <stddef.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <linux/icmp.h>
#include <linux/in.h>
#include <bpf/bpf_endian.h>

#define MAXELEM 1

struct stats {
  __u64 cnt;
  __u64 bytes;
};

struct {
   __uint(type, BPF_MAP_TYPE_HASH);
   __type(key, __u32);
   __type(value, struct stats);
   __uint(max_entries, MAXELEM);
} pkt_cnt SEC(".maps");

SEC("xdp-pass")
int  xdp_allow(struct xdp_md *ctx)
{
    void *data = (void *)(long)ctx->data;
    void *data_end  = (void *)(long)ctx->data_end;

    struct ethhdr *eth = (struct ethhdr *) data;
    if ((void *)eth + sizeof(*eth) > data_end) {
        return XDP_PASS;
    }

    if (eth->h_proto != bpf_ntohs(ETH_P_IP)) {
        // Not an IP packet
        return XDP_PASS;
    }

    void *next_hdr = (void *)eth + sizeof(*eth);
    struct iphdr *ip = (struct iphdr *)next_hdr;
    if ((void *)ip + sizeof(*ip) > data_end) {
        return XDP_PASS;
    }

    // Single byte represents protocol 
    if (ip->protocol != IPPROTO_ICMP) {
        return XDP_PASS;
    }

	__u64 bytes =  (__u64)(data_end - data); 
	__u32 key = 0; //We have only one element.
	struct stats *s = bpf_map_lookup_elem(&pkt_cnt, &key);
	if (!s) {
		struct stats data;
		data.cnt = 1;
		data.bytes = bytes;
	        int rc = bpf_map_update_elem(&pkt_cnt, &key, &data, 0);
		if (rc != 0) {
			bpf_printk("failed to initialize map");
		}
	} else {
		s->cnt += 1;
		s->bytes += bytes;
	}

	bpf_printk("You can go now");
	return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
