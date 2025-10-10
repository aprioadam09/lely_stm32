#include <lely/co/sdev.h>

#define CO_SDEV_STRING(s)	NULL

const struct co_sdev slave_sdev = {
	.id = 0x02,
	.name = NULL,
	.vendor_name = CO_SDEV_STRING("Lely Industries N.V."),
	.vendor_id = 0x00000360,
	.product_name = NULL,
	.product_code = 0x00000000,
	.revision = 0x00000000,
	.order_code = NULL,
	.baud = 0
		| CO_BAUD_125,
	.rate = 125,
	.lss = 0,
	.dummy = 0x000000fe,
	.nobj = 14,
	.objs = (const struct co_sobj[]){{
#if !LELY_NO_CO_OBJ_NAME
		.name = CO_SDEV_STRING("Device type"),
#endif
		.idx = 0x1000,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("Device type"),
#endif
			.subidx = 0x00,
			.type = CO_DEFTYPE_UNSIGNED32,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u32 = CO_UNSIGNED32_MIN },
#endif
			.val = { .u32 = CO_UNSIGNED32_MIN },
			.access = CO_ACCESS_RO,
			.pdo_mapping = 0,
			.flags = 0
		}}
	}, {
#if !LELY_NO_CO_OBJ_NAME
		.name = CO_SDEV_STRING("Error register"),
#endif
		.idx = 0x1001,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("Error register"),
#endif
			.subidx = 0x00,
			.type = CO_DEFTYPE_UNSIGNED8,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u8 = CO_UNSIGNED8_MIN },
			.max = { .u8 = CO_UNSIGNED8_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u8 = CO_UNSIGNED8_MIN },
#endif
			.val = { .u8 = CO_UNSIGNED8_MIN },
			.access = CO_ACCESS_RO,
			.pdo_mapping = 0,
			.flags = 0
		}}
	}, {
#if !LELY_NO_CO_OBJ_NAME
		.name = CO_SDEV_STRING("COB-ID time stamp object"),
#endif
		.idx = 0x1012,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("COB-ID time stamp object"),
#endif
			.subidx = 0x00,
			.type = CO_DEFTYPE_UNSIGNED32,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u32 = 0x80000100lu },
#endif
			.val = { .u32 = 0x80000100lu },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}}
	}, {
#if !LELY_NO_CO_OBJ_NAME
		.name = CO_SDEV_STRING("Producer heartbeat time"),
#endif
		.idx = 0x1017,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("Producer heartbeat time"),
#endif
			.subidx = 0x00,
			.type = CO_DEFTYPE_UNSIGNED16,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u16 = CO_UNSIGNED16_MIN },
			.max = { .u16 = CO_UNSIGNED16_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u16 = CO_UNSIGNED16_MIN },
#endif
			.val = { .u16 = CO_UNSIGNED16_MIN },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}}
	}, {
#if !LELY_NO_CO_OBJ_NAME
		.name = CO_SDEV_STRING("Identity object"),
#endif
		.idx = 0x1018,
		.code = CO_OBJECT_RECORD,
		.nsub = 5,
		.subs = (const struct co_ssub[]){{
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("Highest sub-index supported"),
#endif
			.subidx = 0x00,
			.type = CO_DEFTYPE_UNSIGNED8,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u8 = CO_UNSIGNED8_MIN },
			.max = { .u8 = CO_UNSIGNED8_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u8 = 0x04 },
#endif
			.val = { .u8 = 0x04 },
			.access = CO_ACCESS_CONST,
			.pdo_mapping = 0,
			.flags = 0
		}, {
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("Vendor-ID"),
#endif
			.subidx = 0x01,
			.type = CO_DEFTYPE_UNSIGNED32,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u32 = 0x00000360lu },
#endif
			.val = { .u32 = 0x00000360lu },
			.access = CO_ACCESS_RO,
			.pdo_mapping = 0,
			.flags = 0
		}, {
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("Product code"),
#endif
			.subidx = 0x02,
			.type = CO_DEFTYPE_UNSIGNED32,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u32 = CO_UNSIGNED32_MIN },
#endif
			.val = { .u32 = CO_UNSIGNED32_MIN },
			.access = CO_ACCESS_RO,
			.pdo_mapping = 0,
			.flags = 0
		}, {
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("Revision number"),
#endif
			.subidx = 0x03,
			.type = CO_DEFTYPE_UNSIGNED32,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u32 = CO_UNSIGNED32_MIN },
#endif
			.val = { .u32 = CO_UNSIGNED32_MIN },
			.access = CO_ACCESS_RO,
			.pdo_mapping = 0,
			.flags = 0
		}, {
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("Serial number"),
#endif
			.subidx = 0x04,
			.type = CO_DEFTYPE_UNSIGNED32,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u32 = CO_UNSIGNED32_MIN },
#endif
			.val = { .u32 = CO_UNSIGNED32_MIN },
			.access = CO_ACCESS_RO,
			.pdo_mapping = 0,
			.flags = 0
		}}
	}, {
#if !LELY_NO_CO_OBJ_NAME
		.name = CO_SDEV_STRING("RPDO 1 Communication Parameter"),
#endif
		.idx = 0x1400,
		.code = CO_OBJECT_RECORD,
		.nsub = 3,
		.subs = (const struct co_ssub[]){{
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("highest sub-index supported"),
#endif
			.subidx = 0x00,
			.type = CO_DEFTYPE_UNSIGNED8,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u8 = CO_UNSIGNED8_MIN },
			.max = { .u8 = CO_UNSIGNED8_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u8 = 0x02 },
#endif
			.val = { .u8 = 0x02 },
			.access = CO_ACCESS_RO,
			.pdo_mapping = 0,
			.flags = 0
		}, {
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("COB-ID used by RPDO"),
#endif
			.subidx = 0x01,
			.type = CO_DEFTYPE_UNSIGNED32,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u32 = 0x00000202lu },
#endif
			.val = { .u32 = 0x00000202lu },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}, {
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("transmission type"),
#endif
			.subidx = 0x02,
			.type = CO_DEFTYPE_UNSIGNED8,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u8 = CO_UNSIGNED8_MIN },
			.max = { .u8 = CO_UNSIGNED8_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u8 = CO_UNSIGNED8_MAX },
#endif
			.val = { .u8 = CO_UNSIGNED8_MAX },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}}
	}, {
#if !LELY_NO_CO_OBJ_NAME
		.name = CO_SDEV_STRING("RPDO 1 Mapping Parameter"),
#endif
		.idx = 0x1600,
		.code = CO_OBJECT_RECORD,
		.nsub = 2,
		.subs = (const struct co_ssub[]){{
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("number of mapped objects"),
#endif
			.subidx = 0x00,
			.type = CO_DEFTYPE_UNSIGNED8,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u8 = CO_UNSIGNED8_MIN },
			.max = { .u8 = CO_UNSIGNED8_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u8 = 0x01 },
#endif
			.val = { .u8 = 0x01 },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}, {
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("1st mapped object"),
#endif
			.subidx = 0x01,
			.type = CO_DEFTYPE_UNSIGNED32,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u32 = 0x21000008lu },
#endif
			.val = { .u32 = 0x21000008lu },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}}
	}, {
#if !LELY_NO_CO_OBJ_NAME
		.name = CO_SDEV_STRING("TPDO 1 Communication Parameter"),
#endif
		.idx = 0x1800,
		.code = CO_OBJECT_RECORD,
		.nsub = 4,
		.subs = (const struct co_ssub[]){{
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("highest sub-index supported"),
#endif
			.subidx = 0x00,
			.type = CO_DEFTYPE_UNSIGNED8,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u8 = CO_UNSIGNED8_MIN },
			.max = { .u8 = CO_UNSIGNED8_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u8 = 0x05 },
#endif
			.val = { .u8 = 0x05 },
			.access = CO_ACCESS_RO,
			.pdo_mapping = 0,
			.flags = 0
		}, {
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("COB-ID used by TPDO"),
#endif
			.subidx = 0x01,
			.type = CO_DEFTYPE_UNSIGNED32,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u32 = 0x00000182lu },
#endif
			.val = { .u32 = 0x00000182lu },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}, {
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("transmission type"),
#endif
			.subidx = 0x02,
			.type = CO_DEFTYPE_UNSIGNED8,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u8 = CO_UNSIGNED8_MIN },
			.max = { .u8 = CO_UNSIGNED8_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u8 = 0xfe },
#endif
			.val = { .u8 = 0xfe },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}, {
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("event timer"),
#endif
			.subidx = 0x05,
			.type = CO_DEFTYPE_UNSIGNED16,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u16 = CO_UNSIGNED16_MIN },
			.max = { .u16 = CO_UNSIGNED16_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u16 = CO_UNSIGNED16_MIN },
#endif
			.val = { .u16 = CO_UNSIGNED16_MIN },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}}
	}, {
#if !LELY_NO_CO_OBJ_NAME
		.name = CO_SDEV_STRING("TPDO 1 Mapping Parameter"),
#endif
		.idx = 0x1a00,
		.code = CO_OBJECT_RECORD,
		.nsub = 2,
		.subs = (const struct co_ssub[]){{
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("number of mapped objects"),
#endif
			.subidx = 0x00,
			.type = CO_DEFTYPE_UNSIGNED8,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u8 = CO_UNSIGNED8_MIN },
			.max = { .u8 = CO_UNSIGNED8_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u8 = 0x01 },
#endif
			.val = { .u8 = 0x01 },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}, {
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("1st mapped object"),
#endif
			.subidx = 0x01,
			.type = CO_DEFTYPE_UNSIGNED32,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u32 = 0x21100008lu },
#endif
			.val = { .u32 = 0x21100008lu },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}}
	}, {
#if !LELY_NO_CO_OBJ_NAME
		.name = CO_SDEV_STRING("NMT startup"),
#endif
		.idx = 0x1f80,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("NMT startup"),
#endif
			.subidx = 0x00,
			.type = CO_DEFTYPE_UNSIGNED32,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u32 = CO_UNSIGNED32_MIN },
#endif
			.val = { .u32 = 0x00000004lu },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
				| CO_OBJ_FLAGS_PARAMETER_VALUE
		}}
	}, {
#if !LELY_NO_CO_OBJ_NAME
		.name = CO_SDEV_STRING("Object with custom SDO download callback"),
#endif
		.idx = 0x2000,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("Object with custom SDO download callback"),
#endif
			.subidx = 0x00,
			.type = CO_DEFTYPE_UNSIGNED32,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u32 = CO_UNSIGNED32_MIN },
#endif
			.val = { .u32 = CO_UNSIGNED32_MIN },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}}
	}, {
#if !LELY_NO_CO_OBJ_NAME
		.name = CO_SDEV_STRING("Object with custom SDO upload callback"),
#endif
		.idx = 0x2001,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("Object with custom SDO upload callback"),
#endif
			.subidx = 0x00,
			.type = CO_DEFTYPE_UNSIGNED32,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u32 = CO_UNSIGNED32_MIN },
#endif
			.val = { .u32 = CO_UNSIGNED32_MIN },
			.access = CO_ACCESS_RO,
			.pdo_mapping = 0,
			.flags = 0
		}}
	}, {
#if !LELY_NO_CO_OBJ_NAME
		.name = CO_SDEV_STRING("LED Object with custom PDO callback"),
#endif
		.idx = 0x2100,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("LED Object with custom PDO callback"),
#endif
			.subidx = 0x00,
			.type = CO_DEFTYPE_UNSIGNED8,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u8 = CO_UNSIGNED8_MIN },
			.max = { .u8 = CO_UNSIGNED8_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u8 = CO_UNSIGNED8_MIN },
#endif
			.val = { .u8 = CO_UNSIGNED8_MIN },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 1,
			.flags = 0
		}}
	}, {
#if !LELY_NO_CO_OBJ_NAME
		.name = CO_SDEV_STRING("Button Object with custom PDO callback"),
#endif
		.idx = 0x2110,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
#if !LELY_NO_CO_OBJ_NAME
			.name = CO_SDEV_STRING("Button Object with custom PDO callback"),
#endif
			.subidx = 0x00,
			.type = CO_DEFTYPE_UNSIGNED8,
#if !LELY_NO_CO_OBJ_LIMITS
			.min = { .u8 = CO_UNSIGNED8_MIN },
			.max = { .u8 = CO_UNSIGNED8_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
			.def = { .u8 = CO_UNSIGNED8_MIN },
#endif
			.val = { .u8 = CO_UNSIGNED8_MIN },
			.access = CO_ACCESS_RO,
			.pdo_mapping = 1,
			.flags = 0
		}}
	}}
};

