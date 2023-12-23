## Setup 

### Clean iptables and enable ip forwarding

    ```
    # Enable IP-forwarding.
    $ echo 1 > /proc/sys/net/ipv4/ip_forward

    # Flush forward rules.
    $ iptables -P FORWARD ACCEPT
    $ iptables -F FORWARD

    # Flush nat rules.
    $ iptables -t nat -F
    ```

### Compile and install bpftool

Install dependencies (run as root)

    ```
    $ apt-get update -y
    $ apt-get install -y make gcc libssl-dev bc libelf-dev libcap-dev \
        clang gcc-multilib llvm libncurses5-dev git pkg-config libmnl-dev \
        bison flex libbpf-dev iproute2 jq wget apt binutils-dev
    ```

### Setup Golang for userspace

    ```
    export GOVERSION=1.15.8
    mkdir /tmp/golang;
    pushd; cd /tmp/golang;
    wget https://dl.google.com/go/go${GOVERSION}.linux-amd64.tar.gz
    tar -xzf go${GOVERSION}.linux-amd64.tar.gz
    mv go /usr/local/go${GOVERSION}
    ln -sfn /usr/local/go${GOVERSION} /usr/local/go
    ```

### Get bpftool

    Install from official repository

    ```
    sudo apt-get install linux-tools-generic
    ```

    or compile from source.

    ```
    export KERNEL_GIT=git://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git
    git clone --depth 1 -b master ${KERNEL_GIT} /tmp/linux && \
    cd /tmp/linux/tools/bpf/bpftool/ &&\
    sed -i '/CFLAGS += -O2/a CFLAGS += -static' Makefile && \
    sed -i 's/LIBS = -lelf $(LIBBPF)/LIBS = -lelf -lz $(LIBBPF)/g' Makefile && \
    printf 'feature-libbfd=0\nfeature-libelf=1\nfeature-bpf=1\nfeature-libelf-mmap=1' >> FEATURES_DUMP.bpftool && \
    FEATURES_DUMP=`pwd`/FEATURES_DUMP.bpftool make -j `getconf _NPROCESSORS_ONLN` && \
    strip bpftool && \
    ldd bpftool 2>&1 | grep -q -e "Not a valid dynamic program" \
        -e "not a dynamic executable" || \
        ( echo "Error: bpftool is not statically linked"; false )
    ```

### Install iproute2

    ```
    git clone https://git.kernel.org/pub/scm/network/iproute2/iproute2.git
    cd iproute2
    ./configure --prefix=/usr
    sudo make install
    # Copy the `bpf_api.h` helpers file that lives under `./include`
    # to your `/usr/include` directory.
    #
    # ps.: this could be anywhere - including your current source tree.
    install -m 0644 ./include/bpf_api.h /usr/include/iproute2
    ```

### Before running all the tests create a namespace

Sets up a namespaces with two interfaces, veth outside the namespace and
vpeer inside the namespace mimic a contianer setup.

    ```
    ip netns add vns1
    ip link add veth1 type veth peer name vpeer1
    ip link set vpeer1 netns vns1

    ip addr add 192.168.100.1/24 dev veth1
    ip link set veth1 up

    ip netns exec vns1 ip link set lo up
    ip netns exec vns1 ip link set vpeer1 up
    ip netns exec vns1 ip addr add 192.168.100.2/24 dev vpeer1
    ip netns exec vns1 ip route add 192.168.100.1 dev vpeer1
    ip netns exec vns1 ip route add default via 192.168.100.1
    ```
