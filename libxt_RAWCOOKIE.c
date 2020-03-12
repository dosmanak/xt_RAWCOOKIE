
/*
 * Based on code cretaed by Patrick McHardy <kaber@trash.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtables.h>
//#include <linux/netfilter/xt_SYNPROXY.h>
#include "xt_RAWCOOKIE.h"

enum
{
	O_SACK_PERM = 0,
	O_TIMESTAMP,
	O_WSCALE,
	O_MSS,
	O_ECN,
	O_SENDLOCAL,
	O_SENDDIRECT,
	O_TXMAC,
};

static void RAWCOOKIE_help(void)
{
	printf(
"RAWCOOKIE target options:\n"
"  --sack-perm                        Set SACK_PERM\n"
"  --timestamp                        Set TIMESTAMP\n"
"  --wscale value                     Set window scaling factor\n"
"  --mss value                        Set MSS value\n"
"  --ecn                              Set ECN\n"
"  --sendlocal                        Set send to localout\n"
"  --senddirect                       Set send to direct\n"
"  --txmac                            Set tx mac address\n");
}

static const struct xt_option_entry RAWCOOKIE_opts[] = {
	{.name = "sack-perm", .id = O_SACK_PERM, .type = XTTYPE_NONE, },
	{.name = "timestamp", .id = O_TIMESTAMP, .type = XTTYPE_NONE, },
	{.name = "wscale",    .id = O_WSCALE,    .type = XTTYPE_UINT32, },
	{.name = "mss",       .id = O_MSS,       .type = XTTYPE_UINT32, },
	{.name = "ecn",       .id = O_ECN,	 .type = XTTYPE_NONE, },
	{.name = "sendlocal",   .id = O_SENDLOCAL,	 .type = XTTYPE_NONE, },
	{.name = "senddirect",   .id = O_SENDDIRECT,	 .type = XTTYPE_NONE, },
	{.name = "txmac",     .id = O_TXMAC,	 .type = XTTYPE_STRING, },
	XTOPT_TABLEEND,
};

static const unsigned char* parse_mac(char *char_mac, unsigned char *int_mac)
{
	if (char_mac == NULL) {
		return NULL;
	}

	unsigned int tmp_mac[6];
	int i;

	sscanf(char_mac, "%x:%x:%x:%x:%x:%x", &tmp_mac[0], &tmp_mac[1], &tmp_mac[2], &tmp_mac[3], &tmp_mac[4], &tmp_mac[5]);
	for (i = 0; i < 6; i++)
		int_mac[i] = (unsigned char)tmp_mac[i];

	return int_mac;
}

static void RAWCOOKIE_parse(struct xt_option_call *cb)
{
	struct xt_rawcookie_info *info = cb->data;

	xtables_option_parse(cb);
	switch (cb->entry->id) {
	case O_SACK_PERM:
		info->options |= XT_RAWCOOKIE_OPT_SACK_PERM;
		break;
	case O_TIMESTAMP:
		info->options |= XT_RAWCOOKIE_OPT_TIMESTAMP;
		break;
	case O_WSCALE:
		info->options |= XT_RAWCOOKIE_OPT_WSCALE;
		info->wscale = cb->val.u32;
		break;
	case O_MSS:
		info->options |= XT_RAWCOOKIE_OPT_MSS;
		info->mss = cb->val.u32;
		break;
	case O_ECN:
		info->options |= XT_RAWCOOKIE_OPT_ECN;
		break;
	case O_SENDLOCAL:
		info->options |= XT_RAWCOOKIE_OPT_SENDLOCAL;
		break;
	case O_SENDDIRECT:
		info->options |= XT_RAWCOOKIE_OPT_SENDDIRECT;
		break;
	case O_TXMAC:
		info->options |= XT_RAWCOOKIE_OPT_TXMAC;
		//parse_mac(cb->data, info->txmac);
		memcpy(info->txmac, cb->val.ethermac, ETH_ALEN);
		break;
	}
}

static void RAWCOOKIE_check(struct xt_fcheck_call *cb)
{
}

static void RAWCOOKIE_print(const void *ip, const struct xt_entry_target *target,
                           int numeric)
{
	const struct xt_rawcookie_info *info =
		(const struct xt_rawcookie_info *)target->data;

	printf(" RAWCOOKIE ");
	if (info->options & XT_RAWCOOKIE_OPT_SACK_PERM)
		printf("sack-perm ");
	if (info->options & XT_RAWCOOKIE_OPT_TIMESTAMP)
		printf("timestamp ");
	if (info->options & XT_RAWCOOKIE_OPT_WSCALE)
		printf("wscale %u ", info->wscale);
	if (info->options & XT_RAWCOOKIE_OPT_MSS)
		printf("mss %u ", info->mss);
	if (info->options & XT_RAWCOOKIE_OPT_ECN)
		printf("ecn ");
	if (info->options & XT_RAWCOOKIE_OPT_SENDLOCAL)
		printf("sendlocal ");
	if (info->options & XT_RAWCOOKIE_OPT_SENDDIRECT)
		printf("senddirect ");
	if (info->options & XT_RAWCOOKIE_OPT_TXMAC)
		printf("txmac %s ", info->txmac);
}

static void RAWCOOKIE_save(const void *ip, const struct xt_entry_target *target)
{
	const struct xt_rawcookie_info *info =
		(const struct xt_rawcookie_info *)target->data;

	if (info->options & XT_RAWCOOKIE_OPT_SACK_PERM)
		printf(" --sack-perm");
	if (info->options & XT_RAWCOOKIE_OPT_TIMESTAMP)
		printf(" --timestamp");
	if (info->options & XT_RAWCOOKIE_OPT_WSCALE)
		printf(" --wscale %u", info->wscale);
	if (info->options & XT_RAWCOOKIE_OPT_MSS)
		printf(" --mss %u", info->mss);
	if (info->options & XT_RAWCOOKIE_OPT_ECN)
		printf(" --ecn");
	if (info->options & XT_RAWCOOKIE_OPT_SENDLOCAL)
		printf(" --sendlocal");
	if (info->options & XT_RAWCOOKIE_OPT_SENDDIRECT)
		printf(" --senddirect");
	if (info->options & XT_RAWCOOKIE_OPT_TXMAC)
		printf(" --txmac %s", info->txmac);
}

/*
static int RAWCOOKIE_xlate(struct xt_xlate *xl,
		          const struct xt_xlate_tg_params *params)
{
	const struct xt_rawcookie_info *info =
		(const struct xt_rawcookie_info *)params->target->data;

	xt_xlate_add(xl, " rawcookie ");

	if (info->options & XT_RAWCOOKIE_OPT_SACK_PERM)
		xt_xlate_add(xl, "sack-perm ");
	if (info->options & XT_RAWCOOKIE_OPT_TIMESTAMP)
		xt_xlate_add(xl, "timestamp ");
	if (info->options & XT_RAWCOOKIE_OPT_WSCALE)
		xt_xlate_add(xl, "wscale %u ", info->wscale);
	if (info->options & XT_RAWCOOKIE_OPT_MSS)
		xt_xlate_add(xl, "mss %u ", info->mss);
	if (info->options & XT_RAWCOOKIE_OPT_ECN)
		xt_xlate_add(xl, "ecn ");

	return 1;
}
*/

static struct xtables_target rawcookie_tg_reg = {
	.family        = NFPROTO_UNSPEC,
	.name          = "RAWCOOKIE",
	.version       = XTABLES_VERSION,
	.revision      = 0,
	.size          = XT_ALIGN(sizeof(struct xt_rawcookie_info)),
	.userspacesize = XT_ALIGN(sizeof(struct xt_rawcookie_info)),
	.help          = RAWCOOKIE_help,
	.print         = RAWCOOKIE_print,
	.save          = RAWCOOKIE_save,
	.x6_parse      = RAWCOOKIE_parse,
	.x6_fcheck     = RAWCOOKIE_check,
	.x6_options    = RAWCOOKIE_opts,
//	.xlate         = RAWCOOKIE_xlate,
};

void _init(void)
{
	xtables_register_target(&rawcookie_tg_reg);
}
