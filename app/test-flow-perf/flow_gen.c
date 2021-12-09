/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright 2020 Mellanox Technologies, Ltd
 *
 * The file contains the implementations of the method to
 * fill items, actions & attributes in their corresponding
 * arrays, and then generate or update an rte_flow rule.
 *
 * After the generation the rule goes to validation then
 * creation state and then return the results.
 */

#include <stdint.h>

#include "flow_gen.h"
#include "items_gen.h"
#include "actions_gen.h"
#include "config.h"

static void
fill_attributes(struct rte_flow_attr *attr,
	uint64_t *flow_attrs, uint16_t group, uint16_t priority)
{
	uint8_t i;
	for (i = 0; i < MAX_ATTRS_NUM; i++) {
		if (flow_attrs[i] == 0)
			break;
		if (flow_attrs[i] & INGRESS)
			attr->ingress = 1;
		else if (flow_attrs[i] & EGRESS)
			attr->egress = 1;
		else if (flow_attrs[i] & TRANSFER)
			attr->transfer = 1;
	}
	attr->group = group;
	attr->priority = priority;
}

static void
fill_target_flow_attributes(struct rte_flow_attr *attr, uint16_t group, uint16_t priority)
{
	if (!attr) {
		return;
	}

	attr->ingress = 1;
	attr->group = group;
	attr->priority = priority;
}

struct rte_flow *
generate_target_flow(uint16_t port_id,
	uint16_t group, uint16_t priority,
	struct rte_flow_error *error)
{
	struct rte_flow_attr attr;
	struct rte_flow_item items[3];
	struct rte_flow_action actions[2];
	struct rte_flow *flow = NULL;

	memset(&attr, 0, sizeof(struct rte_flow_attr));
	memset(items, 0, sizeof(items));
	memset(actions, 0, sizeof(actions));

	fill_target_flow_attributes(&attr, group, priority);
	fill_target_flow_items(items);
	fill_target_flow_actions(actions);

	flow = rte_flow_create(port_id, &attr, items, actions, error);
	return flow;
}

struct rte_flow *
generate_flow(uint16_t port_id,
	uint16_t group,
	uint16_t priority,
	uint64_t *flow_attrs,
	uint64_t *flow_items,
	uint64_t *flow_actions,
	uint16_t next_table,
	uint32_t outer_ip_src,
	uint16_t hairpinq,
	uint64_t encap_data,
	uint64_t decap_data,
	uint64_t rss_flags,
	struct rte_flow_error *error)
{
	struct rte_flow_attr attr;
	struct rte_flow_item items[MAX_ITEMS_NUM];
	struct rte_flow_action actions[MAX_ACTIONS_NUM];
	struct rte_flow *flow = NULL;

	memset(items, 0, sizeof(items));
	memset(actions, 0, sizeof(actions));
	memset(&attr, 0, sizeof(struct rte_flow_attr));

	fill_attributes(&attr, flow_attrs, group, priority);

	fill_actions(actions, flow_actions,
		outer_ip_src, next_table, hairpinq,
		encap_data, decap_data, rss_flags);

	fill_items(items, flow_items, outer_ip_src);

	return rte_flow_create(port_id, &attr, items, actions, error);
}

int
update_flow(struct rte_flow *flow,
	uint16_t port_id,
	uint16_t group,
	uint16_t priority,
	uint64_t *flow_attrs,
	uint64_t *flow_items,
	uint64_t *flow_actions,
	uint16_t next_table,
	uint32_t outer_ip_src,
	uint16_t hairpinq,
	uint64_t encap_data,
	uint64_t decap_data,
	struct rte_flow_error *error)
{
//	struct rte_flow_attr attr;
	struct rte_flow_item items[MAX_ITEMS_NUM];
	struct rte_flow_action actions[MAX_ACTIONS_NUM];

	memset(items, 0, sizeof(items));
	memset(actions, 0, sizeof(actions));
//	memset(&attr, 0, sizeof(struct rte_flow_attr));

//	fill_attributes(&attr, flow_attrs, group);

/*	fill_actions(actions, flow_actions,
		outer_ip_src, next_table, hairpinq,
		encap_data, decap_data);*/

	fill_items(items, flow_items, outer_ip_src);

	return rte_flow_update(port_id, flow, items, actions, error);
}
