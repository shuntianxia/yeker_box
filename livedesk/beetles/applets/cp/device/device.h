#ifndef __DEVICE_H__
#define __DEVICE_H__

typedef struct device {
	__s32 dev_id;
	__s32 relay_id;
	__s32 location;
	__s32 button_id;
	__s32 status;
}device_t;

#endif
