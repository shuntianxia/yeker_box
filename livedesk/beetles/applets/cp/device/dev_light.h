#ifndef __DEV_LIGHT_H__
#define __DEV_LIGHT_H__

typedef struct dev_light {
	__s32 dev_id;
	__s32 relay_id;
	__s32 location;
	__s32 button_id;
	__s32 status;
}dev_light_t;

#endif
