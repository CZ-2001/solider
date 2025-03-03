#include "icm20602.h"

int16_t icm_gyro_x,icm_gyro_y,icm_gyro_z;
int16_t icm_acc_x,icm_acc_y,icm_acc_z;

void icm20602_cs_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
	
}

void SPI4_Init(void)
{	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOB时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);//使能SPI1时钟
 
  //GPIOFB3,4,5初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_12;//PB3~5复用功能输出	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_SPI4); //PB3复用为 SPI1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource6,GPIO_AF_SPI4); //PB4复用为 SPI1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_SPI4); //PB5复用为 SPI1
 
	//这里只针对SPI口初始化
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI4,ENABLE);//复位SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI4,DISABLE);//停止复位SPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low ;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 10;	//CRC值计算的多项式
	SPI_Init(SPI4, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI4, ENABLE); //使能SPI外设

}   

//SPI1速度设置函数
//SPI速度=fAPB2/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2时钟一般为84Mhz：
void SPI4_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
	SPI4->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPI4->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI4,ENABLE); //使能SPI1
} 

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节

static uint8_t icm_SPI_read_write_byte(uint8_t TxData)
{
    uint8_t retry = 0;
    while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET)
    {
        retry++;
        if (retry > 200)
        {
            return 0;
        }
    }
    SPI_I2S_SendData(SPI4, TxData);

    retry = 0;
    while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_RXNE) == RESET)
    {
        retry++;
        if (retry > 200)
        {
            return 0;
        }
    }
    return SPI_I2S_ReceiveData(SPI4);
}

void icm_read_muli_reg(uint8_t reg, uint8_t *buf, uint8_t len)
{
    GPIO_ResetBits(GPIOE, GPIO_Pin_4);
    icm_SPI_read_write_byte(reg);
    if (len != 0)
    {
        uint8_t i;
        for (i = 0; i < (len-1); i++)
        {
            *buf = icm_SPI_read_write_byte(0xFF);
            buf++;
        }
    }
    GPIO_SetBits(GPIOE, GPIO_Pin_4);
}



void icm_spi_w_reg_byte(uint8_t cmd, uint8_t val)
{
    uint8_t dat[2];
    GPIO_ResetBits(GPIOE, GPIO_Pin_4);

    dat[0] = cmd | ICM20602_SPI_W;
	  icm_SPI_read_write_byte(dat[0]);
    dat[1] = val;
	  icm_SPI_read_write_byte(dat[1]);

    GPIO_SetBits(GPIOE, GPIO_Pin_4);
}




void icm_spi_r_reg_byte(uint8_t cmd, uint8_t *val)
{
    uint8_t dat[2];

    GPIO_ResetBits(GPIOE, GPIO_Pin_4);
    dat[0] = cmd | ICM20602_SPI_R;
		icm_SPI_read_write_byte(dat[0]);
    dat[1] = *val;
	  dat[1] = icm_SPI_read_write_byte(dat[1]);

    GPIO_SetBits(GPIOE, GPIO_Pin_4);

    *val = dat[1];
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602自检函数
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void icm20602_self3_check(void)
{
    uint8_t dat=0;
    while(0x12 != dat)   //读取ICM20602 ID
    {
        icm_spi_r_reg_byte(ICM20602_WHO_AM_I,&dat);
       	vTaskDelay(10);  //上电延时
        //卡在这里原因有以下几点
        //1 MPU6050坏了，如果是新的这样的概率极低
        //2 接线错误或者没有接好
        //3 可能你需要外接上拉电阻，上拉到3.3V
    }

}

void icm20602_init_spi(void)
{
    uint8_t val = 0x0;

   	delay_ms(10);  //上电延时

    icm20602_self3_check();//检测

    icm_spi_w_reg_byte(ICM20602_PWR_MGMT_1,0x80);//复位设备
    delay_ms(2);
    do
    {   //等待复位成功
        icm_spi_r_reg_byte(ICM20602_PWR_MGMT_1,&val);
    } while(0x41 != val);

    icm_spi_w_reg_byte(ICM20602_PWR_MGMT_1,     0x01);            //时钟设置
    icm_spi_w_reg_byte(ICM20602_PWR_MGMT_2,     0x00);            //开启陀螺仪和加速度计
    icm_spi_w_reg_byte(ICM20602_CONFIG,         0x01);            //176HZ 1KHZ
    icm_spi_w_reg_byte(ICM20602_SMPLRT_DIV,     0x07);            //采样速率 SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
    icm_spi_w_reg_byte(ICM20602_GYRO_CONFIG,    0x18);            //±2000 dps
    icm_spi_w_reg_byte(ICM20602_ACCEL_CONFIG,   0x10);            //±8g
    icm_spi_w_reg_byte(ICM20602_ACCEL_CONFIG_2, 0x03);            //Average 4 samples   44.8HZ   //0x23 Average 16 samples
	//ICM20602_GYRO_CONFIG寄存器
    //设置为:0x00 陀螺仪量程为:±250 dps     获取到的陀螺仪数据除以131           可以转化为带物理单位的数据， 单位为：°/s
    //设置为:0x08 陀螺仪量程为:±500 dps     获取到的陀螺仪数据除以65.5          可以转化为带物理单位的数据，单位为：°/s
    //设置为:0x10 陀螺仪量程为:±1000dps     获取到的陀螺仪数据除以32.8          可以转化为带物理单位的数据，单位为：°/s
    //设置为:0x18 陀螺仪量程为:±2000dps     获取到的陀螺仪数据除以16.4          可以转化为带物理单位的数据，单位为：°/s

    //ICM20602_ACCEL_CONFIG寄存器
    //设置为:0x00 加速度计量程为:±2g          获取到的加速度计数据 除以16384      可以转化为带物理单位的数据，单位：g(m/s^2)
    //设置为:0x08 加速度计量程为:±4g          获取到的加速度计数据 除以8192       可以转化为带物理单位的数据，单位：g(m/s^2)
    //设置为:0x10 加速度计量程为:±8g          获取到的加速度计数据 除以4096       可以转化为带物理单位的数据，单位：g(m/s^2)
    //设置为:0x18 加速度计量程为:±16g         获取到的加速度计数据 除以2048       可以转化为带物理单位的数据，单位：g(m/s^2)
}

void get_icm20602_gyro_spi(void)
{
    struct
    {
        uint8_t reg;
        uint8_t dat[6];
    } buf;

    buf.reg = ICM20602_GYRO_XOUT_H | ICM20602_SPI_R;

    icm_read_muli_reg(buf.reg , &buf.dat[0], 7);
    icm_gyro_x = (int16_t)(((uint16_t)buf.dat[0]<<8 | buf.dat[1]));
    icm_gyro_y = (int16_t)(((uint16_t)buf.dat[2]<<8 | buf.dat[3]));
    icm_gyro_z = (int16_t)(((uint16_t)buf.dat[4]<<8 | buf.dat[5]));
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取ICM20602加速度计数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void get_icm20602_accdata_spi(void)
{
    struct
    {
        uint8_t reg;
        uint8_t dat[6];
    } buf;

    buf.reg = ICM20602_ACCEL_XOUT_H | ICM20602_SPI_R;

    icm_read_muli_reg( buf.reg, &buf.dat[0], 7);
    icm_acc_x = (int16_t)(((uint16_t)buf.dat[0]<<8 | buf.dat[1]));
    icm_acc_y = (int16_t)(((uint16_t)buf.dat[2]<<8 | buf.dat[3]));
    icm_acc_z = (int16_t)(((uint16_t)buf.dat[4]<<8 | buf.dat[5]));
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      将ICM20602数据转化为带有物理单位的数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:               执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
#include "user_lib.h"
#include "math.h"

gyro_param_t GyroOffset;               // 陀螺仪校准值 
/**
 * @brief 陀螺仪零漂初始化
 * 通过采集一定数据求均值计算陀螺仪零点偏移值。
 * 后续 陀螺仪读取的数据 - 零飘值，即可去除零点偏移量。
 */
void gyroOffsetInit(void)     
{
    GyroOffset.Xdata = 0;
    GyroOffset.Ydata = 0;
    GyroOffset.Zdata = 0;
    for (uint16_t i = 0; i < 100; ++i) 
	{
        get_icm20602_gyro_spi();    // 获取陀螺仪角速度
        GyroOffset.Xdata += icm_gyro_x;
        GyroOffset.Ydata += icm_gyro_y;
        GyroOffset.Zdata += icm_gyro_z;
        vTaskDelay(5);    // 最大 1Khz
    }

    GyroOffset.Xdata /= 100;
    GyroOffset.Ydata /= 100;
    GyroOffset.Zdata /= 100;
//	GyroOffset.Xdata=0.59999998;
//	GyroOffset.Ydata=2.6;
//	GyroOffset.Zdata=13.1499996;
	GyroOffset.Xdata=-19.159998f;
	GyroOffset.Ydata=23.489998f;
	GyroOffset.Zdata=7.3400015f;
}

void icmGetValues(void) 
{
    //float alpha = 0.3;
		float alpha = 0.9;
    //一阶低通滤波，单位g
    icm_data.acc_x = (((float) icm_acc_x) * alpha) / 4096 + icm_data.acc_x * (1 - alpha);
    icm_data.acc_y = (((float) icm_acc_y) * alpha) / 4096 + icm_data.acc_y * (1 - alpha);
    icm_data.acc_z = (((float) icm_acc_z) * alpha) / 4096 + icm_data.acc_z * (1 - alpha);

    //! 陀螺仪角速度必须转换为弧度制角速度: deg/s -> rad/s
    icm_data.gyro_x = ((float) icm_gyro_x - GyroOffset.Xdata) * PI / 180.f / 16.4f; 
    icm_data.gyro_y = ((float) icm_gyro_y - GyroOffset.Ydata) * PI / 180.f / 16.4f;
    icm_data.gyro_z = ((float) icm_gyro_z - GyroOffset.Zdata) * PI / 180.f / 16.4f;
}

#define delta_T     0.001f  // 采样周期1ms 即频率1KHZ

float I_ex, I_ey, I_ez;  // 误差积分
quater_param_t Q_info = {1, 0, 0, 0};  // 四元数初始化
euler_param_t eulerAngle;              // 欧拉角
icm_param_t icm_data;                  // ICM20602采集的六轴数值

float icm_kp= 2.f;    // 加速度计的收敛速率比例增益 0.17
float icm_ki= 0.004;   // 陀螺仪收敛速率的积分增益 0.004

/**
 * @brief 用互补滤波算法解算陀螺仪姿态(即利用加速度计修正陀螺仪的积分误差)
 * 加速度计对振动之类的噪声比较敏感，长期数据计算出的姿态可信；陀螺仪对振动噪声不敏感，短期数据可信，但长期使用积分误差严重(内部积分算法放大静态误差)。
 * 因此使用姿态互补滤波，短期相信陀螺仪，长期相信加速度计。
 * @tips: n - 导航坐标系； b - 载体坐标系
 */
void icmAHRSupdate(icm_param_t* icm)            
{
    float halfT = 0.5f * delta_T;    // 采样周期一半
    float vx, vy, vz;               // 当前姿态计算得来的重力在三轴上的分量
    float ex, ey, ez;               // 当前加速计测得的重力加速度在三轴上的分量与用当前姿态计算得来的重力在三轴上的分量的误差
    
    float q0 = Q_info.q0;  //四元数
    float q1 = Q_info.q1;
    float q2 = Q_info.q2;
    float q3 = Q_info.q3;
    
    float q0q0 = q0 * q0;  //先相乘，方便后续计算
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;

    // 正常静止状态为-g 反作用力。
    if(icm->acc_x * icm->acc_y * icm->acc_z == 0) // 加计处于自由落体状态时(此时g = 0)不进行姿态解算，因为会产生分母无穷大的情况
        return;

    // 对加速度数据进行归一化 得到单位加速度 (a^b -> 载体坐标系下的加速度)
    float norm = invSqrt(icm->acc_x * icm->acc_x + icm->acc_y * icm->acc_y + icm->acc_z * icm->acc_z); 
    icm->acc_x = icm->acc_x * norm;
    icm->acc_y = icm->acc_y * norm;
    icm->acc_z = icm->acc_z * norm;

    // 载体坐标系下重力在三个轴上的分量
    vx = 2 * (q1q3 - q0q2);
    vy = 2 * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;

    // g^b 与 a^b 做向量叉乘，得到陀螺仪的校正补偿向量e的系数
    ex = icm->acc_y * vz - icm->acc_z * vy;
    ey = icm->acc_z * vx - icm->acc_x * vz;
    ez = icm->acc_x * vy - icm->acc_y * vx;

    // 误差累加
    I_ex += halfT * ex;  
    I_ey += halfT * ey;
    I_ez += halfT * ez;

    // 使用PI控制器消除向量积误差(陀螺仪漂移误差)
    icm->gyro_x = icm->gyro_x + icm_kp* ex + icm_ki* I_ex;
    icm->gyro_y = icm->gyro_y + icm_kp* ey + icm_ki* I_ey;
    icm->gyro_z = icm->gyro_z + icm_kp* ez + icm_ki* I_ez;

    // 一阶龙格库塔法求解四元数微分方程，其中halfT为测量周期的1/2，gx gy gz为b系陀螺仪角速度。
    q0 = q0 + (-q1 * icm->gyro_x - q2 * icm->gyro_y - q3 * icm->gyro_z) * halfT;
    q1 = q1 + (q0 * icm->gyro_x + q2 * icm->gyro_z - q3 * icm->gyro_y) * halfT;
    q2 = q2 + (q0 * icm->gyro_y - q1 * icm->gyro_z + q3 * icm->gyro_x) * halfT;
    q3 = q3 + (q0 * icm->gyro_z + q1 * icm->gyro_y - q2 * icm->gyro_x) * halfT;

    // 单位化四元数在空间旋转时不会拉伸，仅有旋转角度，下面算法类似线性代数里的正交变换
    norm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    Q_info.q0 = q0 * norm;
    Q_info.q1 = q1 * norm;
    Q_info.q2 = q2 * norm;
    Q_info.q3 = q3 * norm;  // 用全局变量记录上一次计算的四元数值
		
    // atan2返回输入坐标点与坐标原点连线与X轴正方形夹角的弧度值
    eulerAngle.pitch = asin(2 * Q_info.q0 * Q_info.q2 - 2 * Q_info.q1 * Q_info.q3) * 180 / PI; 
    eulerAngle.roll = atan2(2 * Q_info.q2 * Q_info.q3 + 2 * Q_info.q0 * Q_info.q1, -2 * Q_info.q1 * Q_info.q1 - 2 * Q_info.q2 * Q_info.q2 + 1) * 180 / PI; 
    eulerAngle.yaw = atan2(2 * Q_info.q1 * Q_info.q2 + 2 * Q_info.q0 * Q_info.q3, -2 * Q_info.q2 * Q_info.q2 - 2 * Q_info.q3 * Q_info.q3 + 1) * 180 / PI;  
		
}
