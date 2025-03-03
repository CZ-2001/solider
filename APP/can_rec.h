#ifndef __CAN_REC_H
#define __CAN_REC_H
#include "main.h"
#include "stdint.h"
#include "stm32f4xx.h"
#define K_SpdFilter 0.9f
#define CAN_CHASSIS CAN1
#define CAN_GIMBAL CAN2
#define CAN_SHOOT CAN2

#define CAN_YAW CAN1
#define CAN_PITCH CAN2
/* CAN send and receive ID */
typedef enum
{
  // can1:
  CAN_CHASSIS_ALL_ID = 0x200,
  CAN_3508_M1_ID = 0x201,
  CAN_3508_M2_ID = 0x202,
  CAN_3508_M3_ID = 0x203,
  CAN_3508_M4_ID = 0x204,

  CAN_USER_ALL_ID = 0x1FF,

  CAN_SHOOT_ALL_ID = 0x200,
	
  CAN_FRICL_ID = 0x201,
  CAN_FRICR_ID = 0x202,
  CAN_PLATE_MOTOR_ID = 0x203,

  CAN_GIMBAL_ALL_ID = 0x1FF,
  CAN_YAW_MOTOR_ID = 0x205,
  CAN_PIT_MOTOR_ID = 0x206,

  CAN_CAP_SET_ID = 0x210,
  CAN_CAP_FEEDBACK_ID = 0x211,
} can_id_enum;
// RM电机统一数据结构体
#define FRIC_L 1
#define FRIC_R 0
typedef struct
{

  uint16_t ecd;      // 机械位置
  int16_t speed_rpm; // 转速
  int16_t speed_rpm_last;
  int16_t speed_rpm_filter;
  int16_t given_current; // 实际转矩电流
  uint8_t temperate;     // 温度
  int16_t last_ecd;

  // 以下为根据原始数据的推算量
  uint16_t offset_ecd; // 初始机械位置
  int32_t round_cnt;   // 理论可记录524352圈
  int32_t total_ecd;   // 理论可记录4294967295编码值

  int32_t total_ecd_out_2006; // 理论可记录14565圈
  int32_t total_ecd_out_3508; // 理论可记录27305圈
  int32_t total_ecd_out_6020; // 理论可记录524352圈

  float angle_6020;
} MotorMeasureTypeDef;

// rm超级电容数据结构体
typedef struct
{
  long int Vin;
  long int Vcap;
  long int Iin;
  long int Pset;
} CapMeasureTypeDef;
typedef union
{

  uint16_t all;
  struct
  {
    uint16_t rdy : 1;  /*!< bit0    就绪     */
    uint16_t run : 1;  /*!< bit1    运行     */
    uint16_t alm : 1;  /*!< bit2    报警     */
    uint16_t pwr : 1;  /*!< bit3    电源开关 */
    uint16_t load : 1; /*!< bit4    负载开关 */
    uint16_t cc : 1;   /*!< bit5    恒流     */
    uint16_t cv : 1;   /*!< bit6    恒压     */
    uint16_t cw : 1;   /*!< bit7    恒功率   */
    uint16_t revd : 7; /*!< bit8-14 保留     */
    uint16_t flt : 1;  /*!< bit15   故障     */
  } bit;

}csr_t;

typedef struct 
{

  uint16_t ccr;        /*!< 8000H 控制寄存器     */
  uint16_t p_set;      /*!< 8001H 输入功率限制   */
  uint16_t v_set;      /*!< 8002H 输出电压设置   */
  uint16_t i_set;      /*!< 8003H 输出电流限制   */
  csr_t sta_code;      /*!< 8100H 状态标志位     */
  uint16_t err_code;   /*!< 8101H 故障代码       */
  int16_t v_in;        /*!< 8102H 输入电压       */
  int16_t i_in;        /*!< 8103H 输入电流       */
  int16_t p_in;        /*!< 8104H 输入功率       */
  int16_t v_out;       /*!< 8105H 输出电压       */
  int16_t i_out;       /*!< 8106H 输出电流       */
  int16_t p_out;       /*!< 8107H 输出功率       */
  int16_t temp;        /*!< 8108H 温度           */
  uint16_t total_time; /*!< 8109H 累计时间       */
  uint16_t run_time;   /*!< 810AH 运行时间       */
} pm01_od_t;

typedef enum
{
  pm01errorID = 0x610,
  pm01inID = 0x611,
  pm01outID = 0x612,
} pm01_e;
extern MotorMeasureTypeDef Myaw, Mpitch, Mchassis[4], Mfricl[2], Mplate;
extern CapMeasureTypeDef SupercapStu;
extern pm01_od_t pm01_od;
void can_cmd_gimbal(int16_t yaw, int16_t pitch, int16_t rev1, int16_t rev2);
void can_cmd_chassis(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void CAN_CMD_CHASSIS_RESET_ID(void);
void can_cmd_shoot(int16_t User_Motor1, int16_t User_Motor2, int16_t User_Motor3, int16_t User_Motor4);
extern void pm01_cmd_send(uint16_t new_cmd, uint8_t save_flg);
extern void pm01_voltage_set(uint16_t new_voltage, uint8_t save_flg);
extern void pm01_current_set(uint16_t new_voltage, uint8_t save_flg);
extern void pm01_power_set(uint16_t new_power, uint8_t save_flg);

extern void pm01_read(uint32_t id);
#endif
