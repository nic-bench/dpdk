#DPDK with support for rte\_flow\_update

This repository contains the modified DPDK with support for the new API and testing options of test-flow-perf for our upcoming PAM'21 "NIC Bench" paper.

The rte\_flow\_api avoids destroying and inserting rules. Internally it also reuses the parsed patterns. We implemented support for mlx5, that relies on a modified rdma-core library that similarly implements a more efficient update mechanism.

# Original DPDK README
DPDK is a set of libraries and drivers for fast packet processing.
It supports many processor architectures and both FreeBSD and Linux.

The DPDK uses the Open Source BSD-3-Clause license for the core libraries
and drivers. The kernel components are GPL-2.0 licensed.

Please check the doc directory for release notes,
API documentation, and sample application information.

For questions and usage discussions, subscribe to: users@dpdk.org
Report bugs and issues to the development mailing list: dev@dpdk.org
