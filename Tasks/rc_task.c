#include "rc_task.h"


//��ȡң������Ϣ
 RC_ctrl_t ALLRC;
const RC_ctrl_t*rccc;
void rc_choose_task(void *pvParameters)
{
	rccc=get_remote_control_point();
    while (1)
    {
			ALLRC =*get_remote_control_point();//��ȡң����ָ�����
			
			//ϵͳ��ʱ
      vTaskDelay(1);
		
    }
}
