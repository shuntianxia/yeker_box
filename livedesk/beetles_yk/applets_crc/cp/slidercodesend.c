#include <apps.h>
#include "scrolling_i.h"
#include "slidercodesend.h"
#include "zu_code_demo.h"
#include "tongxin_demo.h"
extern int	R_ing;// 1: vga板正忙0:vga板空闲
static void More_30_Percent(ButtonCode_handle*handle,int old_value,int new_value,int step,int times)
{
	ButtonCode_handle * buttoncode; 
	int date; 
	buttoncode = handle;
    if(old_value > new_value){
		if(times < 3){
	   	    date = old_value - ((old_value - new_value) / 3) * times;
			date = date - date % step;
			//LogMI("Buttonhandle_Search buttoncode true");
			Down_SliderSendCode(handle,step,date);
				
			More_30_Percent(handle,old_value,new_value,step,times + 1);
		}else if(times == 3){
	   	    date = old_value - new_value;
			date = date - date % step;
			Down_SliderSendCode(handle,step,date);
			
			More_30_Percent(handle,old_value,new_value,step,times + 1);
		}else if(times > 3){
			//LogMI("only send 3 times");
		}
    }else{
        if(times < 3){
			date = new_value - ((new_value - old_value) / 3) * times;
			date = date - date % step;
			Up_SliderSendCode(handle,step,date);
			
			More_30_Percent(handle,old_value,new_value,step,times + 1);
		}else if(times == 3){
	   	    date = new_value - old_value;
			date = date - date % step;
			Up_SliderSendCode(handle,step,date);
			
			More_30_Percent(handle,old_value,new_value,step,times + 1);
		}else if(times > 3){
			//LogMI("only send 3 times");
		}
    }
}

//手势移动发码
void MoveSliderSendCode(SCR_ScrollingButtonDescriptor *pwnt)
{
   ButtonCode_handle *handle = pwnt->code_handle;
   __bool flag;
   int step = pwnt->lampStep;
   
   if(pwnt->lampSlide>0){
		flag = EPDK_TRUE;
   }else if(pwnt->lampSlide<0){
		flag = EPDK_FALSE;
   }else{
		if(pwnt->value==pwnt->lampLimit_max){
			flag = EPDK_TRUE;
		}else{
			flag = EPDK_FALSE;
		}
   }
   
   if(flag == EPDK_FALSE){
	   Down_SliderSendCode(handle,step,pwnt->value);
   }else{
	   Up_SliderSendCode(handle,step,pwnt->value);
   }
   pwnt->old_value = pwnt->value;   
}

//手势结束发码
void Up_SliderSendCode(ButtonCode_handle*handle,int step,int value)
{
	ButtonCode_handle * buttoncode; 
	int date;
	buttoncode = handle;
	date = value;
	date = date - date % step;
	while(buttoncode){
		//LogMI("buttoncode->up_down %s",buttoncode->up_down);
		if(buttoncode->datas){
			if(SLIB_strcmp(buttoncode->up_down,"up") == 0){
				Send_ButtonCode_Slider(buttoncode->port,buttoncode->baurate,buttoncode->datas,
						 buttoncode->delay, date,buttoncode->Gbutton_handle);
			}
			buttoncode = buttoncode->next;
		}else{
			buttoncode = NULL;
			break;
		}
	}
}
void Up_SliderReadCode(ButtonCode_handle*handle,int step,int value)
{
	ButtonCode_handle * buttoncode; 
	int date;
	buttoncode = handle;
	date = value;
	date = date - date % step;
	while(buttoncode){
		//LogMI("buttoncode->up_down %s",buttoncode->up_down);
		if(buttoncode->datas){
			if(SLIB_strcmp(buttoncode->up_down,"up") == 0){
				Send_ButtonCode_Slider(buttoncode->port,buttoncode->baurate,buttoncode->datas,
						 buttoncode->delay, date,buttoncode->Gbutton_handle);
				buttoncode = NULL;
				break;
			}
			buttoncode = buttoncode->next;
		}else{
			buttoncode = NULL;
			break;
		}
	}
}

void Down_SliderSendCode(ButtonCode_handle*handle,int step,int value)
{
	ButtonCode_handle * buttoncode; 
	int date;
	buttoncode = handle;
	date = value;
	//date = date - date % step;
	while(buttoncode){
		//LogMI("buttoncode->up_down %s",buttoncode->up_down);
		if(buttoncode->datas){
			if(SLIB_strcmp(buttoncode->up_down,"down") == 0){
				Send_ButtonCode_Slider(buttoncode->port,buttoncode->baurate,buttoncode->datas,
						 buttoncode->delay, date,buttoncode->Gbutton_handle);
			}
			buttoncode = buttoncode->next;
		}else{
			buttoncode = NULL;
			break;
		}
	}
}

