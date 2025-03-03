#include "rc_task.h"


//获取遥控器信息
 RC_ctrl_t ALLRC;
const RC_ctrl_t*rccc;
void rc_choose_task(void *pvParameters)
{
	rccc=get_remote_control_point();
    while (1)
    {
			ALLRC =*get_remote_control_point();//获取遥控器指针变量
			
			//系统延时
      vTaskDelay(1);
		
    }
}
