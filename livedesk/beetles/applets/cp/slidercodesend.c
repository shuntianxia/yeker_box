#include <apps.h>
#include "scrolling_i.h"
#include "slidercodesend.h"
#include "zu_code_demo.h"
#include "tongxin_demo.h"

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
void MoveSliderSendCode(ButtonCode_handle*handle,int old_value,int new_value,int step,int ten_percent)
{
   int date; 
   if(old_value > new_value){
   	   date = old_value - new_value;
   }else{
	   date = new_value - old_value;
   }
   
   if(date >= ten_percent*3){
	   More_30_Percent(handle,old_value,new_value,step,1);//一次移动大于30%
   }else if((date > ten_percent) ||(date < ten_percent*3)){
       if(date > ten_percent){
		   date = old_value - old_value % step;
		   if(old_value > new_value){
			   Down_SliderSendCode(handle,step,date);
		   }else{
			   Up_SliderSendCode(handle,step,date);
		   }
	   }else{
		   //LogMI("Move less than step don't send slider code");
	   }
   }else{
       //移动过小不发码
	   //LogMI("Move less than 10 percent don't send slider code");
   }
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
		LogMI("buttoncode->up_down %s",buttoncode->up_down);
		if(buttoncode->datas){
			if(SLIB_strcmp(buttoncode->up_down,"up") == 0){
				Send_ButtonCode_Slider(buttoncode->port,buttoncode->baurate,buttoncode->datas,
						 buttoncode->delay, date);
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
	date = date - date % step;
	while(buttoncode){
		LogMI("buttoncode->up_down %s",buttoncode->up_down);
		if(buttoncode->datas){
			if(SLIB_strcmp(buttoncode->up_down,"down") == 0){
				Send_ButtonCode_Slider(buttoncode->port,buttoncode->baurate,buttoncode->datas,
						 buttoncode->delay, date);
			}
			buttoncode = buttoncode->next;
		}else{
			buttoncode = NULL;
			break;
		}
	}
}

