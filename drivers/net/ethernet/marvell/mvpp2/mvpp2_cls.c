// SPDX-License-Identifier: GPL-2.0
/*
 * RSS and Classifier helpers for Marvell PPv2 Network Controller
 *
 * Copyright (C) 2014 Marvell
 *
 * Marcin Wojtas <mw@semihalf.com>
 */

#include "mvpp2.h"
#include "mvpp2_cls.h"
#include "mvpp2_prs.h"
#include <linux/if_vlan.h>

#define MVPP2_DEF_FLOW(_type, _id, _opts, _ri, _ri_mask)	\
{								\
	.flow_type = _type,					\
	.flow_id = _id,						\
	.supported_hash_opts = _opts,				\
	.prs_ri = {						\
		.ri = _ri,					\
		.ri_mask = _ri_mask				\
	}							\
}

static const struct mvpp2_cls_flow cls_flows[MVPP2_N_PRS_FLOWS] = {
	/* TCP over IPv4 flows, Not fragmented, no vlan tag */
	MVPP2_DEF_FLOW(TCP_V4_FLOW, MVPP2_FL_IP4_TCP_NF_UNTAG,
		       MVPP22_CLS_HEK_IP4_5T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP4 |
		       MVPP2_PRS_RI_L4_TCP,
		       MVPP2_PRS_IP_MASK | MVPP2_PRS_RI_VLAN_MASK),

	MVPP2_DEF_FLOW(TCP_V4_FLOW, MVPP2_FL_IP4_TCP_NF_UNTAG,
		       MVPP22_CLS_HEK_IP4_5T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP4_OPT |
		       MVPP2_PRS_RI_L4_TCP,
		       MVPP2_PRS_IP_MASK | MVPP2_PRS_RI_VLAN_MASK),

	MVPP2_DEF_FLOW(TCP_V4_FLOW, MVPP2_FL_IP4_TCP_NF_UNTAG,
		       MVPP22_CLS_HEK_IP4_5T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP4_OTHER |
		       MVPP2_PRS_RI_L4_TCP,
		       MVPP2_PRS_IP_MASK | MVPP2_PRS_RI_VLAN_MASK),

	/* TCP over IPv4 flows, Not fragmented, with vlan tag */
	MVPP2_DEF_FLOW(TCP_V4_FLOW, MVPP2_FL_IP4_TCP_NF_TAG,
		       MVPP22_CLS_HEK_IP4_5T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP4 | MVPP2_PRS_RI_L4_TCP,
		       MVPP2_PRS_IP_MASK),

	MVPP2_DEF_FLOW(TCP_V4_FLOW, MVPP2_FL_IP4_TCP_NF_TAG,
		       MVPP22_CLS_HEK_IP4_5T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP4_OPT | MVPP2_PRS_RI_L4_TCP,
		       MVPP2_PRS_IP_MASK),

	MVPP2_DEF_FLOW(TCP_V4_FLOW, MVPP2_FL_IP4_TCP_NF_TAG,
		       MVPP22_CLS_HEK_IP4_5T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP4_OTHER | MVPP2_PRS_RI_L4_TCP,
		       MVPP2_PRS_IP_MASK),

	/* TCP over IPv4 flows, fragmented, no vlan tag */
	MVPP2_DEF_FLOW(TCP_V4_FLOW, MVPP2_FL_IP4_TCP_FRAG_UNTAG,
		       MVPP22_CLS_HEK_IP4_2T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP4 |
		       MVPP2_PRS_RI_L4_TCP,
		       MVPP2_PRS_IP_MASK | MVPP2_PRS_RI_VLAN_MASK),

	MVPP2_DEF_FLOW(TCP_V4_FLOW, MVPP2_FL_IP4_TCP_FRAG_UNTAG,
		       MVPP22_CLS_HEK_IP4_2T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP4_OPT |
		       MVPP2_PRS_RI_L4_TCP,
		       MVPP2_PRS_IP_MASK | MVPP2_PRS_RI_VLAN_MASK),

	MVPP2_DEF_FLOW(TCP_V4_FLOW, MVPP2_FL_IP4_TCP_FRAG_UNTAG,
		       MVPP22_CLS_HEK_IP4_2T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP4_OTHER |
		       MVPP2_PRS_RI_L4_TCP,
		       MVPP2_PRS_IP_MASK | MVPP2_PRS_RI_VLAN_MASK),

	/* TCP over IPv4 flows, fragmented, with vlan tag */
	MVPP2_DEF_FLOW(TCP_V4_FLOW, MVPP2_FL_IP4_TCP_FRAG_TAG,
		       MVPP22_CLS_HEK_IP4_2T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP4 | MVPP2_PRS_RI_L4_TCP,
		       MVPP2_PRS_IP_MASK),

	MVPP2_DEF_FLOW(TCP_V4_FLOW, MVPP2_FL_IP4_TCP_FRAG_TAG,
		       MVPP22_CLS_HEK_IP4_2T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP4_OPT | MVPP2_PRS_RI_L4_TCP,
		       MVPP2_PRS_IP_MASK),

	MVPP2_DEF_FLOW(TCP_V4_FLOW, MVPP2_FL_IP4_TCP_FRAG_TAG,
		       MVPP22_CLS_HEK_IP4_2T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP4_OTHER | MVPP2_PRS_RI_L4_TCP,
		       MVPP2_PRS_IP_MASK),

	/* UDP over IPv4 flows, Not fragmented, no vlan tag */
	MVPP2_DEF_FLOW(UDP_V4_FLOW, MVPP2_FL_IP4_UDP_NF_UNTAG,
		       MVPP22_CLS_HEK_IP4_5T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP4 |
		       MVPP2_PRS_RI_L4_UDP,
		       MVPP2_PRS_IP_MASK | MVPP2_PRS_RI_VLAN_MASK),

	MVPP2_DEF_FLOW(UDP_V4_FLOW, MVPP2_FL_IP4_UDP_NF_UNTAG,
		       MVPP22_CLS_HEK_IP4_5T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP4_OPT |
		       MVPP2_PRS_RI_L4_UDP,
		       MVPP2_PRS_IP_MASK | MVPP2_PRS_RI_VLAN_MASK),

	MVPP2_DEF_FLOW(UDP_V4_FLOW, MVPP2_FL_IP4_UDP_NF_UNTAG,
		       MVPP22_CLS_HEK_IP4_5T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP4_OTHER |
		       MVPP2_PRS_RI_L4_UDP,
		       MVPP2_PRS_IP_MASK | MVPP2_PRS_RI_VLAN_MASK),

	/* UDP over IPv4 flows, Not fragmented, with vlan tag */
	MVPP2_DEF_FLOW(UDP_V4_FLOW, MVPP2_FL_IP4_UDP_NF_TAG,
		       MVPP22_CLS_HEK_IP4_5T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP4 | MVPP2_PRS_RI_L4_UDP,
		       MVPP2_PRS_IP_MASK),

	MVPP2_DEF_FLOW(UDP_V4_FLOW, MVPP2_FL_IP4_UDP_NF_TAG,
		       MVPP22_CLS_HEK_IP4_5T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP4_OPT | MVPP2_PRS_RI_L4_UDP,
		       MVPP2_PRS_IP_MASK),

	MVPP2_DEF_FLOW(UDP_V4_FLOW, MVPP2_FL_IP4_UDP_NF_TAG,
		       MVPP22_CLS_HEK_IP4_5T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP4_OTHER | MVPP2_PRS_RI_L4_UDP,
		       MVPP2_PRS_IP_MASK),

	/* UDP over IPv4 flows, fragmented, no vlan tag */
	MVPP2_DEF_FLOW(UDP_V4_FLOW, MVPP2_FL_IP4_UDP_FRAG_UNTAG,
		       MVPP22_CLS_HEK_IP4_2T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP4 |
		       MVPP2_PRS_RI_L4_UDP,
		       MVPP2_PRS_IP_MASK | MVPP2_PRS_RI_VLAN_MASK),

	MVPP2_DEF_FLOW(UDP_V4_FLOW, MVPP2_FL_IP4_UDP_FRAG_UNTAG,
		       MVPP22_CLS_HEK_IP4_2T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP4_OPT |
		       MVPP2_PRS_RI_L4_UDP,
		       MVPP2_PRS_IP_MASK | MVPP2_PRS_RI_VLAN_MASK),

	MVPP2_DEF_FLOW(UDP_V4_FLOW, MVPP2_FL_IP4_UDP_FRAG_UNTAG,
		       MVPP22_CLS_HEK_IP4_2T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP4_OTHER |
		       MVPP2_PRS_RI_L4_UDP,
		       MVPP2_PRS_IP_MASK | MVPP2_PRS_RI_VLAN_MASK),

	/* UDP over IPv4 flows, fragmented, with vlan tag */
	MVPP2_DEF_FLOW(UDP_V4_FLOW, MVPP2_FL_IP4_UDP_FRAG_TAG,
		       MVPP22_CLS_HEK_IP4_2T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP4 | MVPP2_PRS_RI_L4_UDP,
		       MVPP2_PRS_IP_MASK),

	MVPP2_DEF_FLOW(UDP_V4_FLOW, MVPP2_FL_IP4_UDP_FRAG_TAG,
		       MVPP22_CLS_HEK_IP4_2T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP4_OPT | MVPP2_PRS_RI_L4_UDP,
		       MVPP2_PRS_IP_MASK),

	MVPP2_DEF_FLOW(UDP_V4_FLOW, MVPP2_FL_IP4_UDP_FRAG_TAG,
		       MVPP22_CLS_HEK_IP4_2T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP4_OTHER | MVPP2_PRS_RI_L4_UDP,
		       MVPP2_PRS_IP_MASK),

	/* TCP over IPv6 flows, not fragmented, no vlan tag */
	MVPP2_DEF_FLOW(TCP_V6_FLOW, MVPP2_FL_IP6_TCP_NF_UNTAG,
		       MVPP22_CLS_HEK_IP6_5T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP6 |
		       MVPP2_PRS_RI_L4_TCP,
		       MVPP2_PRS_IP_MASK | MVPP2_PRS_RI_VLAN_MASK),

	MVPP2_DEF_FLOW(TCP_V6_FLOW, MVPP2_FL_IP6_TCP_NF_UNTAG,
		       MVPP22_CLS_HEK_IP6_5T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP6_EXT |
		       MVPP2_PRS_RI_L4_TCP,
		       MVPP2_PRS_IP_MASK | MVPP2_PRS_RI_VLAN_MASK),

	/* TCP over IPv6 flows, not fragmented, with vlan tag */
	MVPP2_DEF_FLOW(TCP_V6_FLOW, MVPP2_FL_IP6_TCP_NF_TAG,
		       MVPP22_CLS_HEK_IP6_5T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP6 | MVPP2_PRS_RI_L4_TCP,
		       MVPP2_PRS_IP_MASK),

	MVPP2_DEF_FLOW(TCP_V6_FLOW, MVPP2_FL_IP6_TCP_NF_TAG,
		       MVPP22_CLS_HEK_IP6_5T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP6_EXT | MVPP2_PRS_RI_L4_TCP,
		       MVPP2_PRS_IP_MASK),

	/* TCP over IPv6 flows, fragmented, no vlan tag */
	MVPP2_DEF_FLOW(TCP_V6_FLOW, MVPP2_FL_IP6_TCP_FRAG_UNTAG,
		       MVPP22_CLS_HEK_IP6_2T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP6 |
		       MVPP2_PRS_RI_IP_FRAG_TRUE | MVPP2_PRS_RI_L4_TCP,
		       MVPP2_PRS_IP_MASK | MVPP2_PRS_RI_VLAN_MASK),

	MVPP2_DEF_FLOW(TCP_V6_FLOW, MVPP2_FL_IP6_TCP_FRAG_UNTAG,
		       MVPP22_CLS_HEK_IP6_2T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP6_EXT |
		       MVPP2_PRS_RI_IP_FRAG_TRUE | MVPP2_PRS_RI_L4_TCP,
		       MVPP2_PRS_IP_MASK | MVPP2_PRS_RI_VLAN_MASK),

	/* TCP over IPv6 flows, fragmented, with vlan tag */
	MVPP2_DEF_FLOW(TCP_V6_FLOW, MVPP2_FL_IP6_TCP_FRAG_TAG,
		       MVPP22_CLS_HEK_IP6_2T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP6 | MVPP2_PRS_RI_IP_FRAG_TRUE |
		       MVPP2_PRS_RI_L4_TCP,
		       MVPP2_PRS_IP_MASK),

	MVPP2_DEF_FLOW(TCP_V6_FLOW, MVPP2_FL_IP6_TCP_FRAG_TAG,
		       MVPP22_CLS_HEK_IP6_2T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP6_EXT | MVPP2_PRS_RI_IP_FRAG_TRUE |
		       MVPP2_PRS_RI_L4_TCP,
		       MVPP2_PRS_IP_MASK),

	/* UDP over IPv6 flows, not fragmented, no vlan tag */
	MVPP2_DEF_FLOW(UDP_V6_FLOW, MVPP2_FL_IP6_UDP_NF_UNTAG,
		       MVPP22_CLS_HEK_IP6_5T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP6 |
		       MVPP2_PRS_RI_L4_UDP,
		       MVPP2_PRS_IP_MASK | MVPP2_PRS_RI_VLAN_MASK),

	MVPP2_DEF_FLOW(UDP_V6_FLOW, MVPP2_FL_IP6_UDP_NF_UNTAG,
		       MVPP22_CLS_HEK_IP6_5T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP6_EXT |
		       MVPP2_PRS_RI_L4_UDP,
		       MVPP2_PRS_IP_MASK | MVPP2_PRS_RI_VLAN_MASK),

	/* UDP over IPv6 flows, not fragmented, with vlan tag */
	MVPP2_DEF_FLOW(UDP_V6_FLOW, MVPP2_FL_IP6_UDP_NF_TAG,
		       MVPP22_CLS_HEK_IP6_5T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP6 | MVPP2_PRS_RI_L4_UDP,
		       MVPP2_PRS_IP_MASK),

	MVPP2_DEF_FLOW(UDP_V6_FLOW, MVPP2_FL_IP6_UDP_NF_TAG,
		       MVPP22_CLS_HEK_IP6_5T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP6_EXT | MVPP2_PRS_RI_L4_UDP,
		       MVPP2_PRS_IP_MASK),

	/* UDP over IPv6 flows, fragmented, no vlan tag */
	MVPP2_DEF_FLOW(UDP_V6_FLOW, MVPP2_FL_IP6_UDP_FRAG_UNTAG,
		       MVPP22_CLS_HEK_IP6_2T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP6 |
		       MVPP2_PRS_RI_IP_FRAG_TRUE | MVPP2_PRS_RI_L4_UDP,
		       MVPP2_PRS_IP_MASK | MVPP2_PRS_RI_VLAN_MASK),

	MVPP2_DEF_FLOW(UDP_V6_FLOW, MVPP2_FL_IP6_UDP_FRAG_UNTAG,
		       MVPP22_CLS_HEK_IP6_2T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP6_EXT |
		       MVPP2_PRS_RI_IP_FRAG_TRUE | MVPP2_PRS_RI_L4_UDP,
		       MVPP2_PRS_IP_MASK | MVPP2_PRS_RI_VLAN_MASK),

	/* UDP over IPv6 flows, fragmented, with vlan tag */
	MVPP2_DEF_FLOW(UDP_V6_FLOW, MVPP2_FL_IP6_UDP_FRAG_TAG,
		       MVPP22_CLS_HEK_IP6_2T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP6 | MVPP2_PRS_RI_IP_FRAG_TRUE |
		       MVPP2_PRS_RI_L4_UDP,
		       MVPP2_PRS_IP_MASK),

	MVPP2_DEF_FLOW(UDP_V6_FLOW, MVPP2_FL_IP6_UDP_FRAG_TAG,
		       MVPP22_CLS_HEK_IP6_2T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP6_EXT | MVPP2_PRS_RI_IP_FRAG_TRUE |
		       MVPP2_PRS_RI_L4_UDP,
		       MVPP2_PRS_IP_MASK),

	/* IPv4 flows, no vlan tag */
	MVPP2_DEF_FLOW(IPV4_FLOW, MVPP2_FL_IP4_UNTAG,
		       MVPP22_CLS_HEK_IP4_2T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP4,
		       MVPP2_PRS_RI_VLAN_MASK | MVPP2_PRS_RI_L3_PROTO_MASK),
	MVPP2_DEF_FLOW(IPV4_FLOW, MVPP2_FL_IP4_UNTAG,
		       MVPP22_CLS_HEK_IP4_2T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP4_OPT,
		       MVPP2_PRS_RI_VLAN_MASK | MVPP2_PRS_RI_L3_PROTO_MASK),
	MVPP2_DEF_FLOW(IPV4_FLOW, MVPP2_FL_IP4_UNTAG,
		       MVPP22_CLS_HEK_IP4_2T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP4_OTHER,
		       MVPP2_PRS_RI_VLAN_MASK | MVPP2_PRS_RI_L3_PROTO_MASK),

	/* IPv4 flows, with vlan tag */
	MVPP2_DEF_FLOW(IPV4_FLOW, MVPP2_FL_IP4_TAG,
		       MVPP22_CLS_HEK_IP4_2T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP4,
		       MVPP2_PRS_RI_L3_PROTO_MASK),
	MVPP2_DEF_FLOW(IPV4_FLOW, MVPP2_FL_IP4_TAG,
		       MVPP22_CLS_HEK_IP4_2T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP4_OPT,
		       MVPP2_PRS_RI_L3_PROTO_MASK),
	MVPP2_DEF_FLOW(IPV4_FLOW, MVPP2_FL_IP4_TAG,
		       MVPP22_CLS_HEK_IP4_2T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP4_OTHER,
		       MVPP2_PRS_RI_L3_PROTO_MASK),

	/* IPv6 flows, no vlan tag */
	MVPP2_DEF_FLOW(IPV6_FLOW, MVPP2_FL_IP6_UNTAG,
		       MVPP22_CLS_HEK_IP6_2T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP6,
		       MVPP2_PRS_RI_VLAN_MASK | MVPP2_PRS_RI_L3_PROTO_MASK),
	MVPP2_DEF_FLOW(IPV6_FLOW, MVPP2_FL_IP6_UNTAG,
		       MVPP22_CLS_HEK_IP6_2T,
		       MVPP2_PRS_RI_VLAN_NONE | MVPP2_PRS_RI_L3_IP6,
		       MVPP2_PRS_RI_VLAN_MASK | MVPP2_PRS_RI_L3_PROTO_MASK),

	/* IPv6 flows, with vlan tag */
	MVPP2_DEF_FLOW(IPV6_FLOW, MVPP2_FL_IP6_TAG,
		       MVPP22_CLS_HEK_IP6_2T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP6,
		       MVPP2_PRS_RI_L3_PROTO_MASK),
	MVPP2_DEF_FLOW(IPV6_FLOW, MVPP2_FL_IP6_TAG,
		       MVPP22_CLS_HEK_IP6_2T | MVPP22_CLS_HEK_TAGGED,
		       MVPP2_PRS_RI_L3_IP6,
		       MVPP2_PRS_RI_L3_PROTO_MASK),

	/* Non IP flow, no vlan tag */
	MVPP2_DEF_FLOW(ETHER_FLOW, MVPP2_FL_NON_IP_UNTAG,
		       0,
		       MVPP2_PRS_RI_VLAN_NONE,
		       MVPP2_PRS_RI_VLAN_MASK),
	/* Non IP flow, with vlan tag */
	MVPP2_DEF_FLOW(ETHER_FLOW, MVPP2_FL_NON_IP_TAG,
		       MVPP22_CLS_HEK_OPT_VLAN,
		       0, 0),
};

u32 mvpp2_cls_flow_hits(struct mvpp2 *priv, int index)
{
	mvpp2_write(priv, MVPP2_CTRS_IDX, index);

	return mvpp2_read(priv, MVPP2_CLS_FLOW_TBL_HIT_CTR);
}

void mvpp2_cls_flow_read(struct mvpp2 *priv, int index,
			 struct mvpp2_cls_flow_entry *fe)
{
	fe->index = index;
	mvpp2_write(priv, MVPP2_CLS_FLOW_INDEX_REG, index);
	fe->data[0] = mvpp2_read(priv, MVPP2_CLS_FLOW_TBL0_REG);
	fe->data[1] = mvpp2_read(priv, MVPP2_CLS_FLOW_TBL1_REG);
	fe->data[2] = mvpp2_read(priv, MVPP2_CLS_FLOW_TBL2_REG);
}

/* Update classification flow table registers */
static void mvpp2_cls_flow_write(struct mvpp2 *priv,
				 struct mvpp2_cls_flow_entry *fe)
{
	mvpp2_write(priv, MVPP2_CLS_FLOW_INDEX_REG, fe->index);
	mvpp2_write(priv, MVPP2_CLS_FLOW_TBL0_REG,  fe->data[0]);
	mvpp2_write(priv, MVPP2_CLS_FLOW_TBL1_REG,  fe->data[1]);
	mvpp2_write(priv, MVPP2_CLS_FLOW_TBL2_REG,  fe->data[2]);
}

u32 mvpp2_cls_lookup_hits(struct mvpp2 *priv, int index)
{
	mvpp2_write(priv, MVPP2_CTRS_IDX, index);

	return mvpp2_read(priv, MVPP2_CLS_DEC_TBL_HIT_CTR);
}

void mvpp2_cls_lookup_read(struct mvpp2 *priv, int lkpid, int way,
			   struct mvpp2_cls_lookup_entry *le)
{
	u32 val;

	val = (way << MVPP2_CLS_LKP_INDEX_WAY_OFFS) | lkpid;
	mvpp2_write(priv, MVPP2_CLS_LKP_INDEX_REG, val);
	le->way = way;
	le->lkpid = lkpid;
	le->data = mvpp2_read(priv, MVPP2_CLS_LKP_TBL_REG);
}

/* Update classification lookup table register */
static void mvpp2_cls_lookup_write(struct mvpp2 *priv,
				   struct mvpp2_cls_lookup_entry *le)
{
	u32 val;

	val = (le->way << MVPP2_CLS_LKP_INDEX_WAY_OFFS) | le->lkpid;
	mvpp2_write(priv, MVPP2_CLS_LKP_INDEX_REG, val);
	mvpp2_write(priv, MVPP2_CLS_LKP_TBL_REG, le->data);
}

/* Operations on flow entry */
static int mvpp2_cls_flow_hek_num_get(struct mvpp2_cls_flow_entry *fe)
{
	return fe->data[1] & MVPP2_CLS_FLOW_TBL1_N_FIELDS_MASK;
}

static void mvpp2_cls_flow_hek_num_set(struct mvpp2_cls_flow_entry *fe,
				       int num_of_fields)
{
	fe->data[1] &= ~MVPP2_CLS_FLOW_TBL1_N_FIELDS_MASK;
	fe->data[1] |= MVPP2_CLS_FLOW_TBL1_N_FIELDS(num_of_fields);
}

static int mvpp2_cls_flow_hek_get(struct mvpp2_cls_flow_entry *fe,
				  int field_index)
{
	return (fe->data[2] >> MVPP2_CLS_FLOW_TBL2_FLD_OFFS(field_index)) &
		MVPP2_CLS_FLOW_TBL2_FLD_MASK;
}

static void mvpp2_cls_flow_hek_set(struct mvpp2_cls_flow_entry *fe,
				   int field_index, int field_id)
{
	fe->data[2] &= ~MVPP2_CLS_FLOW_TBL2_FLD(field_index,
						MVPP2_CLS_FLOW_TBL2_FLD_MASK);
	fe->data[2] |= MVPP2_CLS_FLOW_TBL2_FLD(field_index, field_id);
}

static void mvpp2_cls_flow_eng_set(struct mvpp2_cls_flow_entry *fe,
				   int engine)
{
	fe->data[0] &= ~MVPP2_CLS_FLOW_TBL0_ENG(MVPP2_CLS_FLOW_TBL0_ENG_MASK);
	fe->data[0] |= MVPP2_CLS_FLOW_TBL0_ENG(engine);
}

int mvpp2_cls_flow_eng_get(struct mvpp2_cls_flow_entry *fe)
{
	return (fe->data[0] >> MVPP2_CLS_FLOW_TBL0_OFFS) &
		MVPP2_CLS_FLOW_TBL0_ENG_MASK;
}

static void mvpp2_cls_flow_port_id_sel(struct mvpp2_cls_flow_entry *fe,
				       bool from_packet)
{
	if (from_packet)
		fe->data[0] |= MVPP2_CLS_FLOW_TBL0_PORT_ID_SEL;
	else
		fe->data[0] &= ~MVPP2_CLS_FLOW_TBL0_PORT_ID_SEL;
}

static void mvpp2_cls_flow_last_set(struct mvpp2_cls_flow_entry *fe,
				    bool is_last)
{
	fe->data[0] &= ~MVPP2_CLS_FLOW_TBL0_LAST;
	fe->data[0] |= !!is_last;
}

static void mvpp2_cls_flow_pri_set(struct mvpp2_cls_flow_entry *fe, int prio)
{
	fe->data[1] &= ~MVPP2_CLS_FLOW_TBL1_PRIO(MVPP2_CLS_FLOW_TBL1_PRIO_MASK);
	fe->data[1] |= MVPP2_CLS_FLOW_TBL1_PRIO(prio);
}

static void mvpp2_cls_flow_port_add(struct mvpp2_cls_flow_entry *fe,
				    u32 port)
{
	fe->data[0] |= MVPP2_CLS_FLOW_TBL0_PORT_ID(port);
}

static void mvpp2_cls_flow_port_remove(struct mvpp2_cls_flow_entry *fe,
				       u32 port)
{
	fe->data[0] &= ~MVPP2_CLS_FLOW_TBL0_PORT_ID(port);
}

static u8 mvpp2_cls_flow_pmap_get(struct mvpp2_cls_flow_entry *fe)
{
	return (fe->data[0] >> 4) & MVPP2_CLS_FLOW_TBL0_PORT_ID_MASK;
}

static void mvpp2_cls_flow_lu_type_set(struct mvpp2_cls_flow_entry *fe,
				       u8 lu_type)
{
	fe->data[1] &= ~MVPP2_CLS_FLOW_TBL1_LU_TYPE(MVPP2_CLS_LU_TYPE_MASK);
	fe->data[1] |= MVPP2_CLS_FLOW_TBL1_LU_TYPE(lu_type);
}

static u8 mvpp2_cls_flow_lu_type_get(struct mvpp2_cls_flow_entry *fe)
{
	return (fe->data[1] &
		MVPP2_CLS_FLOW_TBL1_LU_TYPE(MVPP2_CLS_LU_TYPE_MASK)) >> 3;
}

/* Initialize the parser entry for the given flow */
static void mvpp2_cls_flow_prs_init(struct mvpp2 *priv,
				    const struct mvpp2_cls_flow *flow)
{
	mvpp2_prs_add_flow(priv, flow->flow_id, flow->prs_ri.ri,
			   flow->prs_ri.ri_mask);
}

/* Initialize the Lookup Id table entry for the given flow */
static void mvpp2_cls_flow_lkp_init(struct mvpp2 *priv,
				    const struct mvpp2_cls_flow *flow)
{
	struct mvpp2_cls_lookup_entry le;

	le.way = 0;
	le.lkpid = flow->flow_id;

	/* The default RxQ for this port is set in the C2 lookup */
	le.data = 0;

	/* We point on the first lookup in the sequence for the flow, that is
	 * the C2 lookup.
	 */
	le.data |= MVPP2_CLS_LKP_FLOW_PTR(MVPP2_CLS_FLT_FIRST(flow->flow_id));

	/* CLS is always enabled, RSS is enabled/disabled in C2 lookup */
	le.data |= MVPP2_CLS_LKP_TBL_LOOKUP_EN_MASK;

	mvpp2_cls_lookup_write(priv, &le);
}

/* Adds a field to the Header Extracted Key generation parameters*/
static int mvpp2_flow_add_hek_field(struct mvpp2_cls_flow_entry *fe,
				    u32 field_id)
{
	int nb_fields = mvpp2_cls_flow_hek_num_get(fe);

	if (nb_fields == MVPP2_FLOW_N_FIELDS)
		return -EINVAL;

	mvpp2_cls_flow_hek_set(fe, nb_fields, field_id);

	mvpp2_cls_flow_hek_num_set(fe, nb_fields + 1);

	return 0;
}

static void mvpp2_cls_c2_write(struct mvpp2 *priv,
			       struct mvpp2_cls_c2_entry *c2)
{
	u32 val;
	mvpp2_write(priv, MVPP22_CLS_C2_TCAM_IDX, c2->index);

	val = mvpp2_read(priv, MVPP22_CLS_C2_TCAM_INV);
	if (c2->valid)
		val &= ~MVPP22_CLS_C2_TCAM_INV_BIT;
	else
		val |= MVPP22_CLS_C2_TCAM_INV_BIT;
	mvpp2_write(priv, MVPP22_CLS_C2_TCAM_INV, val);

	mvpp2_write(priv, MVPP22_CLS_C2_ACT_TABLE, c2->act_table);
	mvpp2_write(priv, MVPP22_CLS_C2_ACT, c2->act);

	mvpp2_write(priv, MVPP22_CLS_C2_ATTR0, c2->attr[0]);
	mvpp2_write(priv, MVPP22_CLS_C2_ATTR1, c2->attr[1]);
	mvpp2_write(priv, MVPP22_CLS_C2_ATTR2, c2->attr[2]);
	mvpp2_write(priv, MVPP22_CLS_C2_ATTR3, c2->attr[3]);

	mvpp2_write(priv, MVPP22_CLS_C2_TCAM_DATA0, c2->tcam[0]);
	mvpp2_write(priv, MVPP22_CLS_C2_TCAM_DATA1, c2->tcam[1]);
	mvpp2_write(priv, MVPP22_CLS_C2_TCAM_DATA2, c2->tcam[2]);
	mvpp2_write(priv, MVPP22_CLS_C2_TCAM_DATA3, c2->tcam[3]);
	mvpp2_write(priv, MVPP22_CLS_C2_TCAM_DATA4, c2->tcam[4]);
}

void mvpp2_cls_c2_read(struct mvpp2 *priv, int index,
		       struct mvpp2_cls_c2_entry *c2)
{
	u32 val;
	mvpp2_write(priv, MVPP22_CLS_C2_TCAM_IDX, index);

	c2->index = index;

	c2->tcam[0] = mvpp2_read(priv, MVPP22_CLS_C2_TCAM_DATA0);
	c2->tcam[1] = mvpp2_read(priv, MVPP22_CLS_C2_TCAM_DATA1);
	c2->tcam[2] = mvpp2_read(priv, MVPP22_CLS_C2_TCAM_DATA2);
	c2->tcam[3] = mvpp2_read(priv, MVPP22_CLS_C2_TCAM_DATA3);
	c2->tcam[4] = mvpp2_read(priv, MVPP22_CLS_C2_TCAM_DATA4);

	c2->act_table = mvpp2_read(priv, MVPP22_CLS_C2_ACT_TABLE);
	c2->act = mvpp2_read(priv, MVPP22_CLS_C2_ACT);

	c2->attr[0] = mvpp2_read(priv, MVPP22_CLS_C2_ATTR0);
	c2->attr[1] = mvpp2_read(priv, MVPP22_CLS_C2_ATTR1);
	c2->attr[2] = mvpp2_read(priv, MVPP22_CLS_C2_ATTR2);
	c2->attr[3] = mvpp2_read(priv, MVPP22_CLS_C2_ATTR3);

	val = mvpp2_read(priv, MVPP22_CLS_C2_TCAM_INV);
	c2->valid = !(val & MVPP22_CLS_C2_TCAM_INV_BIT);
}

/* Initialize the flow table entries for the given flow */
static void mvpp2_cls_flow_init(struct mvpp2 *priv,
				const struct mvpp2_cls_flow *flow)
{
	struct mvpp2_cls_flow_entry fe;
	int i, pri = 0;

	/* Assign default values to all entries in the flow */
	for (i = MVPP2_CLS_FLT_FIRST(flow->flow_id);
	     i <= MVPP2_CLS_FLT_LAST(flow->flow_id); i++) {

		memset(&fe, 0, sizeof(fe));
		fe.index = i;
		mvpp2_cls_flow_pri_set(&fe, pri++);

		if (i == MVPP2_CLS_FLT_LAST(flow->flow_id))
			mvpp2_cls_flow_last_set(&fe, 1);

		mvpp2_cls_flow_write(priv, &fe);
	}

	/* RSS config C2 lookup */
	memset(&fe, 0, sizeof(fe));
	fe.index = MVPP2_CLS_FLT_C2_RSS_ENTRY(flow->flow_id);

	mvpp2_cls_flow_eng_set(&fe, MVPP22_CLS_ENGINE_C2);
	mvpp2_cls_flow_port_id_sel(&fe, true);
	mvpp2_cls_flow_lu_type_set(&fe, MVPP2_CLS_LU_ALL);

	/* Add all ports */
	for (i = 0; i < MVPP2_MAX_PORTS; i++)
		mvpp2_cls_flow_port_add(&fe, BIT(i));

	mvpp2_cls_flow_write(priv, &fe);

	/* C3Hx lookups */
	for (i = 0; i < MVPP2_MAX_PORTS; i++) {
		memset(&fe, 0, sizeof(fe));
		fe.index = MVPP2_CLS_FLT_HASH_ENTRY(i, flow->flow_id);

		/* Add a default engine to the entry. */
		mvpp2_cls_flow_eng_set(&fe, MVPP22_CLS_ENGINE_C3HA);
		mvpp2_cls_flow_port_id_sel(&fe, true);
		mvpp2_cls_flow_port_add(&fe, BIT(i));

		mvpp2_cls_flow_write(priv, &fe);
	}
}

/* Map the 802.3Q priority to the given rxq id for that port in the QoS table */
static void mvpp22_qos_set_8023q_to_rxq(struct mvpp2_port *port, u8 vlan_pri, u8 rxq)
{
	/* Select the QoS table and the row we write to */
	mvpp2_write(port->priv, MVPP22_CLS_QOS_IDX,
		    MVPP22_CLS_QOS_TBL_NO(port->id) |
		    MVPP22_CLS_QOS_PRI_IDX(vlan_pri));
	/* Map rxq to 802.3Q priority */
	mvpp2_write(port->priv, MVPP22_CLS_QOS,
		    MVPP22_CLS_QOS_RXQ(port->first_rxq + rxq) |
		    MVPP22_CLS_QOS_PRI(vlan_pri));
}

static int mvpp2_flow_set_hek_fields(struct mvpp2_cls_flow_entry *fe,
				     unsigned long hash_opts)
{
	u32 field_id;
	int i;

	/* Clear old fields */
	mvpp2_cls_flow_hek_num_set(fe, 0);
	fe->data[2] = 0;

	for_each_set_bit(i, &hash_opts, MVPP22_CLS_HEK_N_FIELDS) {
		switch (BIT(i)) {
		case MVPP22_CLS_HEK_OPT_VLAN_PRI_QOS:
			/* This entry has to be added to field 1 */
			field_id = MVPP22_CLS_FIELD_VLAN_PRI_QOS;
			break;
		case MVPP22_CLS_HEK_OPT_MAC_DA:
			field_id = MVPP22_CLS_FIELD_MAC_DA;
			break;
		case MVPP22_CLS_HEK_OPT_VLAN:
			field_id = MVPP22_CLS_FIELD_VLAN;
			break;
		case MVPP22_CLS_HEK_OPT_VLAN_PRI:
			field_id = MVPP22_CLS_FIELD_VLAN_PRI;
			break;
		case MVPP22_CLS_HEK_OPT_IP4SA:
			field_id = MVPP22_CLS_FIELD_IP4SA;
			break;
		case MVPP22_CLS_HEK_OPT_IP4DA:
			field_id = MVPP22_CLS_FIELD_IP4DA;
			break;
		case MVPP22_CLS_HEK_OPT_IP6SA:
			field_id = MVPP22_CLS_FIELD_IP6SA;
			break;
		case MVPP22_CLS_HEK_OPT_IP6DA:
			field_id = MVPP22_CLS_FIELD_IP6DA;
			break;
		case MVPP22_CLS_HEK_OPT_L4SIP:
			field_id = MVPP22_CLS_FIELD_L4SIP;
			break;
		case MVPP22_CLS_HEK_OPT_L4DIP:
			field_id = MVPP22_CLS_FIELD_L4DIP;
			break;
		default:
			return -EINVAL;
		}
		if (mvpp2_flow_add_hek_field(fe, field_id))
			return -EINVAL;
	}

	return 0;
}

const struct mvpp2_cls_flow *mvpp2_cls_flow_get(int flow)
{
	if (flow >= MVPP2_N_PRS_FLOWS)
		return NULL;

	return &cls_flows[flow];
}

/* Set the hash generation options for the given traffic flow.
 * One traffic flow (in the ethtool sense) has multiple classification flows,
 * to handle specific cases such as fragmentation, or the presence of a
 * VLAN / DSA Tag.
 *
 * Each of these individual flows has different constraints, for example we
 * can't hash fragmented packets on L4 data (else we would risk having packet
 * re-ordering), so each classification flows masks the options with their
 * supported ones.
 *
 */
static int mvpp2_port_rss_hash_opts_set(struct mvpp2_port *port, int flow_type,
					u16 requested_opts)
{
	struct mvpp2_cls_flow_entry fe;
	const struct mvpp2_cls_flow *flow;
	int i, engine, flow_index;
	u16 hash_opts;

	for_each_cls_flow_id_with_type(i, flow_type) {
		flow = mvpp2_cls_flow_get(i);
		if (!flow)
			return -EINVAL;

		flow_index = MVPP2_CLS_FLT_HASH_ENTRY(port->id, flow->flow_id);

		mvpp2_cls_flow_read(port->priv, flow_index, &fe);

		hash_opts = flow->supported_hash_opts & requested_opts;

		/* Use C3HB engine to access L4 infos. This adds L4 infos to the
		 * hash parameters
		 */
		if (hash_opts & MVPP22_CLS_HEK_L4_OPTS)
			engine = MVPP22_CLS_ENGINE_C3HB;
		else
			engine = MVPP22_CLS_ENGINE_C3HA;

		if (mvpp2_flow_set_hek_fields(&fe, hash_opts))
			return -EINVAL;

		mvpp2_cls_flow_eng_set(&fe, engine);

		mvpp2_cls_flow_write(port->priv, &fe);
	}

	return 0;
}

u16 mvpp2_flow_get_hek_fields(struct mvpp2_cls_flow_entry *fe)
{
	u16 hash_opts = 0;
	int n_fields, i, field;

	n_fields = mvpp2_cls_flow_hek_num_get(fe);

	for (i = 0; i < n_fields; i++) {
		field = mvpp2_cls_flow_hek_get(fe, i);

		switch (field) {
		case MVPP22_CLS_FIELD_VLAN_PRI_QOS:
			hash_opts |= MVPP22_CLS_HEK_OPT_VLAN_PRI_QOS;
			break;
		case MVPP22_CLS_FIELD_MAC_DA:
			hash_opts |= MVPP22_CLS_HEK_OPT_MAC_DA;
			break;
		case MVPP22_CLS_FIELD_VLAN:
			hash_opts |= MVPP22_CLS_HEK_OPT_VLAN;
			break;
		case MVPP22_CLS_FIELD_VLAN_PRI:
			hash_opts |= MVPP22_CLS_HEK_OPT_VLAN_PRI;
			break;
		case MVPP22_CLS_FIELD_L3_PROTO:
			hash_opts |= MVPP22_CLS_HEK_OPT_L3_PROTO;
			break;
		case MVPP22_CLS_FIELD_IP4SA:
			hash_opts |= MVPP22_CLS_HEK_OPT_IP4SA;
			break;
		case MVPP22_CLS_FIELD_IP4DA:
			hash_opts |= MVPP22_CLS_HEK_OPT_IP4DA;
			break;
		case MVPP22_CLS_FIELD_IP6SA:
			hash_opts |= MVPP22_CLS_HEK_OPT_IP6SA;
			break;
		case MVPP22_CLS_FIELD_IP6DA:
			hash_opts |= MVPP22_CLS_HEK_OPT_IP6DA;
			break;
		case MVPP22_CLS_FIELD_L4SIP:
			hash_opts |= MVPP22_CLS_HEK_OPT_L4SIP;
			break;
		case MVPP22_CLS_FIELD_L4DIP:
			hash_opts |= MVPP22_CLS_HEK_OPT_L4DIP;
			break;
		default:
			break;
		}
	}
	return hash_opts;
}

/* Returns the hash opts for this flow. There are several classifier flows
 * for one traffic flow, this returns an aggregation of all configurations.
 */
static u16 mvpp2_port_rss_hash_opts_get(struct mvpp2_port *port, int flow_type)
{
	struct mvpp2_cls_flow_entry fe;
	const struct mvpp2_cls_flow *flow;
	int i, flow_index;
	u16 hash_opts = 0;

	for_each_cls_flow_id_with_type(i, flow_type) {
		flow = mvpp2_cls_flow_get(i);
		if (!flow)
			return 0;

		flow_index = MVPP2_CLS_FLT_HASH_ENTRY(port->id, flow->flow_id);

		mvpp2_cls_flow_read(port->priv, flow_index, &fe);

		hash_opts |= mvpp2_flow_get_hek_fields(&fe);
	}

	return hash_opts;
}

static void mvpp2_cls_port_init_flows(struct mvpp2 *priv)
{
	const struct mvpp2_cls_flow *flow;
	int i;

	for (i = 0; i < MVPP2_N_PRS_FLOWS; i++) {
		flow = mvpp2_cls_flow_get(i);
		if (!flow)
			break;

		mvpp2_cls_flow_prs_init(priv, flow);
		mvpp2_cls_flow_lkp_init(priv, flow);
		mvpp2_cls_flow_init(priv, flow);
	}
}

static void mvpp2_port_c2_cls_init(struct mvpp2_port *port)
{
	struct mvpp2_cls_c2_entry c2;
	u8 qh, ql, pmap;

	memset(&c2, 0, sizeof(c2));

	c2.index = MVPP22_CLS_C2_QOS_ENTRY(port->id);

	/* Leave the port map empty at init so that this entry doesn't match
	 * by default
	 */
	c2.tcam[4] |= MVPP22_CLS_C2_TCAM_EN(MVPP22_CLS_C2_LU_TYPE(MVPP2_CLS_LU_TYPE_MASK));
	c2.tcam[4] |= MVPP22_CLS_C2_LU_TYPE(MVPP2_CLS_LU_TAGGED_ONLY);

	/* Mark packet as "forwarded to software". If match, update and lock
	 * the dest rxq that we recovered from the QoS table so that the RSS
	 * lookup doesn't override it.
	 */
	c2.act = MVPP22_CLS_C2_ACT_FWD(MVPP22_C2_FWD_SW_LOCK) |
		 MVPP22_CLS_C2_ACT_QHIGH(MVPP22_C2_UPD_LOCK) |
		 MVPP22_CLS_C2_ACT_QLOW(MVPP22_C2_UPD_LOCK);

	/* Extract rxq from the QoS table lookup */
	c2.act_table = MVPP22_CLS_C2_ACT_TABLE_QOS_TBL(port->id) |
		       MVPP22_CLS_C2_ACT_TABLE_PRI_FROM_TBL |
		       MVPP22_CLS_C2_ACT_TABLE_QLO_FROM_TBL |
		       MVPP22_CLS_C2_ACT_TABLE_QHI_FROM_TBL;

	c2.valid = true;

	mvpp2_cls_c2_write(port->priv, &c2);

	memset(&c2, 0, sizeof(c2));

	c2.index = MVPP22_CLS_C2_RSS_ENTRY(port->id);

	pmap = BIT(port->id);
	c2.tcam[4] = MVPP22_CLS_C2_PORT_ID(pmap);
	c2.tcam[4] |= MVPP22_CLS_C2_TCAM_EN(MVPP22_CLS_C2_PORT_ID(pmap));

	/* Match on Lookup Type */
	c2.tcam[4] |= MVPP22_CLS_C2_TCAM_EN(MVPP22_CLS_C2_LU_TYPE(MVPP2_CLS_LU_TYPE_MASK));
	c2.tcam[4] |= MVPP22_CLS_C2_LU_TYPE(MVPP2_CLS_LU_ALL);

	/* Update RSS status after matching this entry */
	c2.act = MVPP22_CLS_C2_ACT_RSS_EN(MVPP22_C2_UPD_LOCK);

	/* Mark packet as "forwarded to software", needed for RSS */
	c2.act |= MVPP22_CLS_C2_ACT_FWD(MVPP22_C2_FWD_SW_LOCK);

	/* Configure the default rx queue : Update Queue Low and Queue High, but
	 * don't lock, since the rx queue selection might be overridden by RSS
	 */
	c2.act |= MVPP22_CLS_C2_ACT_QHIGH(MVPP22_C2_UPD) |
		   MVPP22_CLS_C2_ACT_QLOW(MVPP22_C2_UPD);

	qh = (port->first_rxq >> 3) & MVPP22_CLS_C2_ATTR0_QHIGH_MASK;
	ql = port->first_rxq & MVPP22_CLS_C2_ATTR0_QLOW_MASK;

	c2.attr[0] = MVPP22_CLS_C2_ATTR0_QHIGH(qh) |
		      MVPP22_CLS_C2_ATTR0_QLOW(ql);

	c2.valid = true;

	mvpp2_cls_c2_write(port->priv, &c2);
}

/* Classifier default initialization */
void mvpp2_cls_init(struct mvpp2 *priv)
{
	struct mvpp2_cls_lookup_entry le;
	struct mvpp2_cls_flow_entry fe;
	struct mvpp2_cls_c2_entry c2;
	int index;

	/* Enable classifier */
	mvpp2_write(priv, MVPP2_CLS_MODE_REG, MVPP2_CLS_MODE_ACTIVE_MASK);

	/* Clear classifier flow table */
	memset(&fe.data, 0, sizeof(fe.data));
	for (index = 0; index < MVPP2_CLS_FLOWS_TBL_SIZE; index++) {
		fe.index = index;
		mvpp2_cls_flow_write(priv, &fe);
	}

	/* Clear classifier lookup table */
	le.data = 0;
	for (index = 0; index < MVPP2_CLS_LKP_TBL_SIZE; index++) {
		le.lkpid = index;
		le.way = 0;
		mvpp2_cls_lookup_write(priv, &le);

		le.way = 1;
		mvpp2_cls_lookup_write(priv, &le);
	}

	/* Clear C2 TCAM engine table */
	memset(&c2, 0, sizeof(c2));
	c2.valid = false;
	for (index = 0; index < MVPP22_CLS_C2_N_ENTRIES; index++) {
		c2.index = index;
		mvpp2_cls_c2_write(priv, &c2);
	}

	mvpp2_cls_port_init_flows(priv);
}

void mvpp2_cls_port_config(struct mvpp2_port *port)
{
	struct mvpp2_cls_lookup_entry le;
	u32 val;

	/* Set way for the port */
	val = mvpp2_read(port->priv, MVPP2_CLS_PORT_WAY_REG);
	val &= ~MVPP2_CLS_PORT_WAY_MASK(port->id);
	mvpp2_write(port->priv, MVPP2_CLS_PORT_WAY_REG, val);

	/* Pick the entry to be accessed in lookup ID decoding table
	 * according to the way and lkpid.
	 */
	le.lkpid = port->id;
	le.way = 0;
	le.data = 0;

	/* Set initial CPU queue for receiving packets */
	le.data &= ~MVPP2_CLS_LKP_TBL_RXQ_MASK;
	le.data |= port->first_rxq;

	/* Disable classification engines */
	le.data &= ~MVPP2_CLS_LKP_TBL_LOOKUP_EN_MASK;

	/* Update lookup ID table entry */
	mvpp2_cls_lookup_write(port->priv, &le);

	mvpp2_port_c2_cls_init(port);
}

u32 mvpp2_cls_c2_hit_count(struct mvpp2 *priv, int c2_index)
{
	mvpp2_write(priv, MVPP22_CLS_C2_TCAM_IDX, c2_index);

	return mvpp2_read(priv, MVPP22_CLS_C2_HIT_CTR);
}

static void mvpp2_rss_port_c2_enable(struct mvpp2_port *port)
{
	struct mvpp2_cls_c2_entry c2;

	mvpp2_cls_c2_read(port->priv, MVPP22_CLS_C2_RSS_ENTRY(port->id), &c2);

	c2.attr[2] |= MVPP22_CLS_C2_ATTR2_RSS_EN;

	mvpp2_cls_c2_write(port->priv, &c2);
}

static void mvpp2_rss_port_c2_disable(struct mvpp2_port *port)
{
	struct mvpp2_cls_c2_entry c2;

	mvpp2_cls_c2_read(port->priv, MVPP22_CLS_C2_RSS_ENTRY(port->id), &c2);

	c2.attr[2] &= ~MVPP22_CLS_C2_ATTR2_RSS_EN;

	mvpp2_cls_c2_write(port->priv, &c2);
}

void mvpp22_port_rss_enable(struct mvpp2_port *port)
{
	mvpp2_rss_port_c2_enable(port);
}

void mvpp22_port_rss_disable(struct mvpp2_port *port)
{
	mvpp2_rss_port_c2_disable(port);
}

/* We use a scheme where only one port can use a given C2 entry, so no need to
 * use the matching pattern allowing to match multiple ports with one entry.
 */
static void mvpp22_port_c2_lookup_enable(struct mvpp2_port *port, int entry)
{
	struct mvpp2_cls_c2_entry c2;
	u8 pmap;

	mvpp2_cls_c2_read(port->priv, entry, &c2);

	/* Simply add the port to the pmap of this entry, to enable it */
	pmap = BIT(port->id);
	c2.tcam[4] = MVPP22_CLS_C2_PORT_ID(pmap);
	c2.tcam[4] |= MVPP22_CLS_C2_TCAM_EN(MVPP22_CLS_C2_PORT_ID(pmap));

	mvpp2_cls_c2_write(port->priv, &c2);
}

static void mvpp22_port_c2_lookup_disable(struct mvpp2_port *port, int entry)
{
	struct mvpp2_cls_c2_entry c2;

	mvpp2_cls_c2_read(port->priv, entry, &c2);

	/* Clear the port map so that the entry doesn't match anymore */
	c2.tcam[4] &= ~(MVPP22_CLS_C2_PORT_ID(0) |
			MVPP22_CLS_C2_TCAM_EN(MVPP22_CLS_C2_PORT_ID(0)));

	mvpp2_cls_c2_write(port->priv, &c2);
}

/* Set CPU queue number for oversize packets */
void mvpp2_cls_oversize_rxq_set(struct mvpp2_port *port)
{
	u32 val;

	mvpp2_write(port->priv, MVPP2_CLS_OVERSIZE_RXQ_LOW_REG(port->id),
		    port->first_rxq & MVPP2_CLS_OVERSIZE_RXQ_LOW_MASK);

	mvpp2_write(port->priv, MVPP2_CLS_SWFWD_P2HQ_REG(port->id),
		    (port->first_rxq >> MVPP2_CLS_OVERSIZE_RXQ_LOW_BITS));

	val = mvpp2_read(port->priv, MVPP2_CLS_SWFWD_PCTRL_REG);
	val |= MVPP2_CLS_SWFWD_PCTRL_MASK(port->id);
	mvpp2_write(port->priv, MVPP2_CLS_SWFWD_PCTRL_REG, val);
}

static int mvpp2_cls_flow_spec_get_8023q_pri(struct ethtool_rx_flow_spec *spec)
{
	if (!(spec->flow_type & (FLOW_EXT | FLOW_MAC_EXT)))
		return -EINVAL;

	/* We only support matching the full vlan prio, and nothing else.*/
	if (spec->m_ext.vlan_tci != htons(VLAN_PRIO_MASK))
		return -EINVAL;

	return ntohs(spec->m_ext.vlan_tci &
		     spec->h_ext.vlan_tci) >> VLAN_PRIO_SHIFT;
}

static int mvpp2_port_c2_qos_rule_add(struct mvpp2_port *port,
				      struct mvpp2_rfs_rule *rule)
{
	/* The special case "QoS lookup" has a pre-defined index */
	rule->c2_index = MVPP22_CLS_C2_QOS_ENTRY(port->id);
	mvpp22_port_c2_lookup_enable(port, rule->c2_index);
	mvpp22_qos_set_8023q_to_rxq(port, rule->pri, rule->dst);

	return 0;
}

static int mvpp2_port_c2_qos_rule_del(struct mvpp2_port *port,
				      struct mvpp2_rfs_rule *rule)
{
	bool last = true;
	int i;
	for (i = 0; i <= 7; i++) {
		if (i == rule->loc)
			continue;

		if (port->rfs_rules[i]) {
			last = false;
			break;
		}
	}
	/* The special case "QoS lookup" has a pre-defined index */
	rule->c2_index = MVPP22_CLS_C2_QOS_ENTRY(port->id);
	if (last)
		mvpp22_port_c2_lookup_disable(port, rule->c2_index);
	else
		mvpp22_qos_set_8023q_to_rxq(port, rule->pri, port->first_rxq);

	return 0;
}

static int mvpp2_port_c2_rfs_rule_insert(struct mvpp2_port *port,
					 struct mvpp2_rfs_rule *rule)
{
	int ret = -EOPNOTSUPP;

	if (rule->flags & MVPP2_RFS_F_VLAN_QOS &&
	    rule->action == MVPP2_RFS_ACT_TO_RXQ)
		ret = mvpp2_port_c2_qos_rule_add(port, rule);

	return ret;

}

static int mvpp2_port_flow_table_find(struct mvpp2 *priv, int flow_id, u8 engine,
				      u8 lu_type, u16 hek_fields)
{
	struct mvpp2_cls_flow_entry fe;
	u16 flow_hek_fields;
	int i, flow_engine;
	u8 flow_lu_type;

	/* Iterate on all flow table entries for this ID */
	for (i = MVPP2_CLS_FLT_FIRST(flow_id);
	     i < MVPP2_CLS_FLT_LAST(flow_id); i++) {

		mvpp2_cls_flow_read(priv, i, &fe);

		flow_hek_fields = mvpp2_flow_get_hek_fields(&fe);
		if (flow_hek_fields != hek_fields)
			continue;

		flow_engine = mvpp2_cls_flow_eng_get(&fe);
		if (flow_engine != engine)
			continue;

		flow_lu_type = mvpp2_cls_flow_lu_type_get(&fe);
		if (flow_lu_type != lu_type)
			continue;

		return i;
	}

	return -1;
}

static int mvpp2_port_flow_table_find_free(struct mvpp2 *priv, int flow_id)
{
	struct mvpp2_cls_flow_entry fe;
	int i;
	u8 pmap;

	memset(&fe, 0, sizeof(fe));
	for (i = 0; i < MVPP2_N_RFS_ENTRIES; i++) {
		mvpp2_cls_flow_read(priv, MVPP2_CLS_FLT_C2_RFS(flow_id, i), &fe);

		/* get pmap */
		pmap = mvpp2_cls_flow_pmap_get(&fe);
		if (!pmap)
			return i;
	}

	return -1;
}

static bool mvpp2_port_flt_has_siblings(struct mvpp2_port *port,
					struct mvpp2_rfs_rule *rule)
{
	struct mvpp2_ethtool_fs *efs;
	int i;

	for (i = 0; i <= MVPP2_N_RFS_RULES; i++) {
		efs = port->rfs_rules[i];
		if (!efs)
			continue;
		/* Skip the entry we are trying to find the sibling of */
		if (efs->rule.loc == rule->loc)
			continue;
		if (efs->rule.flt_index == rule->flt_index)
			return true;
	}

	return false;
}

/* Finish */
static int mvpp2_port_cls_rfs_rule_remove(struct mvpp2_port *port,
					  struct mvpp2_rfs_rule *rule)
{
	struct mvpp2_cls_flow_entry fe;

	if (rule->flt_index >= 0 &&
	    !mvpp2_port_flt_has_siblings(port, rule)) {
		mvpp2_cls_flow_read(port->priv, rule->flt_index, &fe);
		mvpp2_cls_flow_port_remove(&fe, BIT(port->id));
		mvpp2_cls_flow_write(port->priv, &fe);
	}

	if (rule->c2_index >= 0) {

		if (rule->c2_index == MVPP22_CLS_C2_QOS_ENTRY(port->id)) {
			mvpp2_port_c2_qos_rule_del(port, rule);
		} else {
			mvpp22_port_c2_lookup_disable(port, rule->c2_index);
		}
	}

	return 0;
}

static int mvpp2_port_flt_rfs_rule_insert(struct mvpp2_port *port,
				      struct mvpp2_rfs_rule *rule)
{
	const struct mvpp2_cls_flow *flow;
	struct mvpp2_cls_flow_entry fe;
	struct mvpp2 *priv = port->priv;
	int index, ret, i;
	u8 lu_type;

	if (rule->engine != MVPP22_CLS_ENGINE_C2)
		return -EOPNOTSUPP;

	ret = mvpp2_port_c2_rfs_rule_insert(port, rule);
	if (ret)
		return ret;

	// Iterate on each flow and determine if we should apply the rule
	for_each_cls_flow_id(i) {
		flow = mvpp2_cls_flow_get(i);
		if (!flow)
			return 0;

		/* Skip untagged flows if we want tagged-only traffic */
		if (rule->flow_type == MVPP2_CLS_TAGGED_ONLY &&
		    !(flow->supported_hash_opts & (MVPP22_CLS_HEK_TAGGED)))
			continue;
		else if (rule->flow_type != MVPP2_CLS_ALL_FLOWS &&
			 flow->flow_type == rule->flow_type)
			continue;

		lu_type = rule->flow_type;
		if (rule->flow_type == MVPP2_CLS_TAGGED_ONLY)
			lu_type = MVPP2_CLS_LU_TAGGED_ONLY;
		else if (rule->flow_type == MVPP2_CLS_ALL_FLOWS)
			lu_type = MVPP2_CLS_LU_ALL;
		else
			return -EOPNOTSUPP;

		index = mvpp2_port_flow_table_find(priv, flow->flow_id,
						   rule->engine, lu_type,
						   rule->hek_fields);

		/* If there already is a flow entry that matches our parameters */
		if (index >= 0) {
			rule->flt_index = index;
			mvpp2_cls_flow_read(priv, index, &fe);
			mvpp2_cls_flow_port_add(&fe, BIT(port->id));
			mvpp2_cls_flow_write(priv, &fe);
			continue;
		} else {
			index = mvpp2_port_flow_table_find_free(priv,
								flow->flow_id);
			if (index < 0)
				return -EOPNOTSUPP;
		}

		/* Create entry */
		memset(&fe, 0, sizeof(fe));
		fe.index = index;
		rule->flt_index = index;

		mvpp2_cls_flow_eng_set(&fe, rule->engine);
		mvpp2_cls_flow_port_id_sel(&fe, true);
		mvpp2_flow_set_hek_fields(&fe, rule->hek_fields);
		mvpp2_cls_flow_lu_type_set(&fe, lu_type);
		mvpp2_cls_flow_port_add(&fe, BIT(port->id));

		mvpp2_cls_flow_write(priv, &fe);
	}

	return 0;
}

static u16 mvpp2_cls_flow_spec_get_hek(struct ethtool_rxnfc *info)
{
	u16 hek = 0;
	int pri;

	pri = mvpp2_cls_flow_spec_get_8023q_pri(&info->fs);
	if (pri >= 0)
		hek |= MVPP22_CLS_HEK_OPT_VLAN_PRI;

	return hek;
}

static int mvpp2_cls_c2_build_tcam_match(struct mvpp2_rfs_rule *rule,
					  struct ethtool_rx_flow_spec *spec)
{
	if (!(spec->flow_type & (FLOW_EXT | FLOW_MAC_EXT)))
		return -EINVAL;

	/* We only support matching the full vlan prio, and nothing else.*/
	if (spec->m_ext.vlan_tci != htons(VLAN_PRIO_MASK))
		return -EINVAL;

	rule->c2_tcam = ((u64)mvpp2_cls_flow_spec_get_8023q_pri(spec)) << 61;
	rule->c2_tcam_mask = ((u64)(VLAN_PRIO_MASK >> VLAN_PRIO_SHIFT)) << 61;

	return 0;
}

/* Convert an ethtool rxnfc rule into a mvpp2-specific rfs flow rule.
 * It is responsible for detecting that a rule is correct.
 */
static int mvpp2_cls_rfs_parse_rule(struct ethtool_rxnfc *info,
				    struct mvpp2_rfs_rule *rule)
{
	int pri;

	memset(rule, 0, sizeof(*rule));

	if (info->fs.ring_cookie == RX_CLS_FLOW_WAKE)
		return -EOPNOTSUPP;

	if (info->fs.flow_type & FLOW_RSS) {
		rule->action = MVPP2_RFS_ACT_TO_RSS;
		if (info->rss_context > 7)
			return -EOPNOTSUPP;
		rule->dst = info->rss_context;
	} else if (info->fs.ring_cookie == RX_CLS_FLOW_DISC) {
		rule->action = MVPP2_RFS_ACT_DROP;
	} else {
		rule->action = MVPP2_RFS_ACT_TO_RXQ;
		rule->dst = info->fs.ring_cookie;
	}

	rule->loc = info->fs.location;

	/* Priority based steering isn't necessarly handled by a match engine,
	 * we can use the dedicated C2 QoS tables.
	 */
	pri = mvpp2_cls_flow_spec_get_8023q_pri(&info->fs);
	if (pri >= 0) {
		rule->pri = pri;
		rule->flags |= MVPP2_RFS_F_VLAN_QOS;
		rule->flow_type = MVPP2_CLS_TAGGED_ONLY;
	} else {
		rule->flow_type = info->flow_type & !(FLOW_EXT | FLOW_MAC_EXT |
						      FLOW_RSS);
	}

	/* Get the HEK parameters and the engine needed to perform the flow
	 * steering */
	rule->hek_fields = mvpp2_cls_flow_spec_get_hek(info);

	/* Special case : We use QoS tables when steering to a queue based on
	 * VLAN PRI. This requires setting a special field in the HEK.
	 */
	if ((rule->hek_fields == MVPP22_CLS_HEK_OPT_VLAN_PRI) &&
	    (rule->action == MVPP2_RFS_ACT_TO_RXQ)) {
		/* The first 8 locations are dedicated to VLAN PRI QoS. There
		 * are 8 possible priorities, but we only use 1 physical entry
		 * for this by using the QoS tables.
		 */
		if (rule->loc >= 8)
			return -EINVAL;

		rule->hek_fields = MVPP22_CLS_HEK_OPT_VLAN_PRI_QOS;
	} else {
		if (rule->loc < 8)
			return -EINVAL;
	}

	/* Classifier can only extract 5 fields from the header */
	if (hweight16(rule->hek_fields) > MVPP2_FLOW_N_FIELDS)
		return -EOPNOTSUPP;

	/* For now, only use the C2 engine which has a HEK size limited to 64
	 * bits for TCAM matching.*/
	rule->engine = MVPP22_CLS_ENGINE_C2;

	if (mvpp2_cls_c2_build_tcam_match(rule, &info->fs))
		return -EINVAL;

	return 0;
}

int mvpp2_ethtool_cls_rule_ins(struct mvpp2_port *port,
			       struct ethtool_rxnfc *info)
{
	struct mvpp2_ethtool_fs *efs, *old_efs;
	int ret;

	if (info->fs.location >= 14 || info->fs.location < 0)
		return -EINVAL;

	efs = kzalloc(sizeof(*efs), GFP_KERNEL);
		if (!efs)
			return -ENOMEM;

	ret = mvpp2_cls_rfs_parse_rule(info, &efs->rule);
	if (ret)
		goto clean_rule;

	ret = mvpp2_port_flt_rfs_rule_insert(port, &efs->rule);
	if (ret)
		goto clean_rule;

	/* Replace an already existing rule */
	if (port->rfs_rules[info->fs.location]) {
		old_efs = port->rfs_rules[info->fs.location];
		ret = mvpp2_port_cls_rfs_rule_remove(port, &old_efs->rule);
		if (ret)
			return ret;
		kfree(old_efs);
	}

	memcpy(&efs->spec, &info->fs, sizeof(struct ethtool_rx_flow_spec));
	port->rfs_rules[info->fs.location] = efs;
	port->n_rfs_rules++;

	return 0;

clean_rule:
	kfree(efs);
	return ret;
}

int mvpp2_ethtool_cls_rule_del(struct mvpp2_port *port,
			       struct ethtool_rxnfc *info)
{
	struct mvpp2_ethtool_fs *efs;
	int ret;

	/* TODO location */
	if (info->fs.location >= 14 || info->fs.location < 0)
		return -EINVAL;

	efs = port->rfs_rules[info->fs.location];
	if (!efs)
		return -EINVAL;

	/* Remove the rule from the engines. */
	ret =  mvpp2_port_cls_rfs_rule_remove(port, &efs->rule);
	if (ret)
		return ret;

	port->n_rfs_rules--;
	port->rfs_rules[info->fs.location] = NULL;
	kfree(efs);

	return 0;
}

static inline u32 mvpp22_rxfh_indir(struct mvpp2_port *port, u32 rxq)
{
	int nrxqs, cpu, cpus = num_possible_cpus();

	/* Number of RXQs per CPU */
	nrxqs = port->nrxqs / cpus;

	/* CPU that will handle this rx queue */
	cpu = rxq / nrxqs;

	if (!cpu_online(cpu))
		return port->first_rxq;

	/* Indirection to better distribute the paquets on the CPUs when
	 * configuring the RSS queues.
	 */
	return port->first_rxq + ((rxq * nrxqs + rxq / cpus) % port->nrxqs);
}

void mvpp22_rss_fill_table(struct mvpp2_port *port, u32 table)
{
	struct mvpp2 *priv = port->priv;
	int i;

	for (i = 0; i < MVPP22_RSS_TABLE_ENTRIES; i++) {
		u32 sel = MVPP22_RSS_INDEX_TABLE(table) |
			  MVPP22_RSS_INDEX_TABLE_ENTRY(i);
		mvpp2_write(priv, MVPP22_RSS_INDEX, sel);

		mvpp2_write(priv, MVPP22_RSS_TABLE_ENTRY,
			    mvpp22_rxfh_indir(port, port->indir[i]));
	}
}

int mvpp2_ethtool_rxfh_set(struct mvpp2_port *port, struct ethtool_rxnfc *info)
{
	u16 hash_opts = 0;

	switch (info->flow_type) {
	case TCP_V4_FLOW:
	case UDP_V4_FLOW:
	case TCP_V6_FLOW:
	case UDP_V6_FLOW:
		if (info->data & RXH_L4_B_0_1)
			hash_opts |= MVPP22_CLS_HEK_OPT_L4SIP;
		if (info->data & RXH_L4_B_2_3)
			hash_opts |= MVPP22_CLS_HEK_OPT_L4DIP;
		/* Fallthrough */
	case IPV4_FLOW:
	case IPV6_FLOW:
		if (info->data & RXH_L2DA)
			hash_opts |= MVPP22_CLS_HEK_OPT_MAC_DA;
		if (info->data & RXH_VLAN)
			hash_opts |= MVPP22_CLS_HEK_OPT_VLAN;
		if (info->data & RXH_L3_PROTO)
			hash_opts |= MVPP22_CLS_HEK_OPT_L3_PROTO;
		if (info->data & RXH_IP_SRC)
			hash_opts |= (MVPP22_CLS_HEK_OPT_IP4SA |
				     MVPP22_CLS_HEK_OPT_IP6SA);
		if (info->data & RXH_IP_DST)
			hash_opts |= (MVPP22_CLS_HEK_OPT_IP4DA |
				     MVPP22_CLS_HEK_OPT_IP6DA);
		break;
	default: return -EOPNOTSUPP;
	}

	return mvpp2_port_rss_hash_opts_set(port, info->flow_type, hash_opts);
}

int mvpp2_ethtool_rxfh_get(struct mvpp2_port *port, struct ethtool_rxnfc *info)
{
	unsigned long hash_opts;
	int i;

	hash_opts = mvpp2_port_rss_hash_opts_get(port, info->flow_type);
	info->data = 0;

	for_each_set_bit(i, &hash_opts, MVPP22_CLS_HEK_N_FIELDS) {
		switch (BIT(i)) {
		case MVPP22_CLS_HEK_OPT_MAC_DA:
			info->data |= RXH_L2DA;
			break;
		case MVPP22_CLS_HEK_OPT_VLAN:
			info->data |= RXH_VLAN;
			break;
		case MVPP22_CLS_HEK_OPT_L3_PROTO:
			info->data |= RXH_L3_PROTO;
			break;
		case MVPP22_CLS_HEK_OPT_IP4SA:
		case MVPP22_CLS_HEK_OPT_IP6SA:
			info->data |= RXH_IP_SRC;
			break;
		case MVPP22_CLS_HEK_OPT_IP4DA:
		case MVPP22_CLS_HEK_OPT_IP6DA:
			info->data |= RXH_IP_DST;
			break;
		case MVPP22_CLS_HEK_OPT_L4SIP:
			info->data |= RXH_L4_B_0_1;
			break;
		case MVPP22_CLS_HEK_OPT_L4DIP:
			info->data |= RXH_L4_B_2_3;
			break;
		default:
			return -EINVAL;
		}
	}
	return 0;
}

void mvpp22_port_rss_init(struct mvpp2_port *port)
{
	struct mvpp2 *priv = port->priv;
	int i;

	/* Set the table width: replace the whole classifier Rx queue number
	 * with the ones configured in RSS table entries.
	 */
	mvpp2_write(priv, MVPP22_RSS_INDEX, MVPP22_RSS_INDEX_TABLE(port->id));
	mvpp2_write(priv, MVPP22_RSS_WIDTH, 8);

	/* The default RxQ is used as a key to select the RSS table to use.
	 * We use one RSS table per port.
	 */
	mvpp2_write(priv, MVPP22_RSS_INDEX,
		    MVPP22_RSS_INDEX_QUEUE(port->first_rxq));
	mvpp2_write(priv, MVPP22_RXQ2RSS_TABLE,
		    MVPP22_RSS_TABLE_POINTER(port->id));

	/* Configure the first table to evenly distribute the packets across
	 * real Rx Queues. The table entries map a hash to a port Rx Queue.
	 */
	for (i = 0; i < MVPP22_RSS_TABLE_ENTRIES; i++)
		port->indir[i] = ethtool_rxfh_indir_default(i, port->nrxqs);

	mvpp22_rss_fill_table(port, port->id);

	/* Configure default flows */
	mvpp2_port_rss_hash_opts_set(port, IPV4_FLOW, MVPP22_CLS_HEK_IP4_2T);
	mvpp2_port_rss_hash_opts_set(port, IPV6_FLOW, MVPP22_CLS_HEK_IP6_2T);
	mvpp2_port_rss_hash_opts_set(port, TCP_V4_FLOW, MVPP22_CLS_HEK_IP4_5T);
	mvpp2_port_rss_hash_opts_set(port, TCP_V6_FLOW, MVPP22_CLS_HEK_IP6_5T);
	mvpp2_port_rss_hash_opts_set(port, UDP_V4_FLOW, MVPP22_CLS_HEK_IP4_5T);
	mvpp2_port_rss_hash_opts_set(port, UDP_V6_FLOW, MVPP22_CLS_HEK_IP6_5T);
}
