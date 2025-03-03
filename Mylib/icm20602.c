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
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOBʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);//ʹ��SPI1ʱ��
 
  //GPIOFB3,4,5��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_12;//PB3~5���ù������	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_SPI4); //PB3����Ϊ SPI1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource6,GPIO_AF_SPI4); //PB4����Ϊ SPI1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_SPI4); //PB5����Ϊ SPI1
 
	//����ֻ���SPI�ڳ�ʼ��
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI4,ENABLE);//��λSPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI4,DISABLE);//ֹͣ��λSPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low ;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 10;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI4, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI4, ENABLE); //ʹ��SPI����

}   

//SPI1�ٶ����ú���
//SPI�ٶ�=fAPB2/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2ʱ��һ��Ϊ84Mhz��
void SPI4_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
	SPI4->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
	SPI4->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI4,ENABLE); //ʹ��SPI1
} 

//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�

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
//  @brief      ICM20602�Լ캯��
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void icm20602_self3_check(void)
{
    uint8_t dat=0;
    while(0x12 != dat)   //��ȡICM20602 ID
    {
        icm_spi_r_reg_byte(ICM20602_WHO_AM_I,&dat);
       	vTaskDelay(10);  //�ϵ���ʱ
        //��������ԭ�������¼���
        //1 MPU6050���ˣ�������µ������ĸ��ʼ���
        //2 ���ߴ������û�нӺ�
        //3 ��������Ҫ����������裬������3.3V
    }

}

void icm20602_init_spi(void)
{
    uint8_t val = 0x0;

   	delay_ms(10);  //�ϵ���ʱ

    icm20602_self3_check();//���

    icm_spi_w_reg_byte(ICM20602_PWR_MGMT_1,0x80);//��λ�豸
    delay_ms(2);
    do
    {   //�ȴ���λ�ɹ�
        icm_spi_r_reg_byte(ICM20602_PWR_MGMT_1,&val);
    } while(0x41 != val);

    icm_spi_w_reg_byte(ICM20602_PWR_MGMT_1,     0x01);            //ʱ������
    icm_spi_w_reg_byte(ICM20602_PWR_MGMT_2,     0x00);            //���������Ǻͼ��ٶȼ�
    icm_spi_w_reg_byte(ICM20602_CONFIG,         0x01);            //176HZ 1KHZ
    icm_spi_w_reg_byte(ICM20602_SMPLRT_DIV,     0x07);            //�������� SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
    icm_spi_w_reg_byte(ICM20602_GYRO_CONFIG,    0x18);            //��2000 dps
    icm_spi_w_reg_byte(ICM20602_ACCEL_CONFIG,   0x10);            //��8g
    icm_spi_w_reg_byte(ICM20602_ACCEL_CONFIG_2, 0x03);            //Average 4 samples   44.8HZ   //0x23 Average 16 samples
	//ICM20602_GYRO_CONFIG�Ĵ���
    //����Ϊ:0x00 ����������Ϊ:��250 dps     ��ȡ�������������ݳ���131           ����ת��Ϊ������λ�����ݣ� ��λΪ����/s
    //����Ϊ:0x08 ����������Ϊ:��500 dps     ��ȡ�������������ݳ���65.5          ����ת��Ϊ������λ�����ݣ���λΪ����/s
    //����Ϊ:0x10 ����������Ϊ:��1000dps     ��ȡ�������������ݳ���32.8          ����ת��Ϊ������λ�����ݣ���λΪ����/s
    //����Ϊ:0x18 ����������Ϊ:��2000dps     ��ȡ�������������ݳ���16.4          ����ת��Ϊ������λ�����ݣ���λΪ����/s

    //ICM20602_ACCEL_CONFIG�Ĵ���
    //����Ϊ:0x00 ���ٶȼ�����Ϊ:��2g          ��ȡ���ļ��ٶȼ����� ����16384      ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
    //����Ϊ:0x08 ���ٶȼ�����Ϊ:��4g          ��ȡ���ļ��ٶȼ����� ����8192       ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
    //����Ϊ:0x10 ���ٶȼ�����Ϊ:��8g          ��ȡ���ļ��ٶȼ����� ����4096       ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
    //����Ϊ:0x18 ���ٶȼ�����Ϊ:��16g         ��ȡ���ļ��ٶȼ����� ����2048       ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
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
//  @brief      ��ȡICM20602���ٶȼ�����
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
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
//  @brief      ��ICM20602����ת��Ϊ��������λ������
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:               ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
#include "user_lib.h"
#include "math.h"

gyro_param_t GyroOffset;               // ������У׼ֵ 
/**
 * @brief ��������Ư��ʼ��
 * ͨ���ɼ�һ���������ֵ�������������ƫ��ֵ��
 * ���� �����Ƕ�ȡ������ - ��Ʈֵ������ȥ�����ƫ������
 */
void gyroOffsetInit(void)     
{
    GyroOffset.Xdata = 0;
    GyroOffset.Ydata = 0;
    GyroOffset.Zdata = 0;
    for (uint16_t i = 0; i < 100; ++i) 
	{
        get_icm20602_gyro_spi();    // ��ȡ�����ǽ��ٶ�
        GyroOffset.Xdata += icm_gyro_x;
        GyroOffset.Ydata += icm_gyro_y;
        GyroOffset.Zdata += icm_gyro_z;
        vTaskDelay(5);    // ��� 1Khz
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
    //һ�׵�ͨ�˲�����λg
    icm_data.acc_x = (((float) icm_acc_x) * alpha) / 4096 + icm_data.acc_x * (1 - alpha);
    icm_data.acc_y = (((float) icm_acc_y) * alpha) / 4096 + icm_data.acc_y * (1 - alpha);
    icm_data.acc_z = (((float) icm_acc_z) * alpha) / 4096 + icm_data.acc_z * (1 - alpha);

    //! �����ǽ��ٶȱ���ת��Ϊ�����ƽ��ٶ�: deg/s -> rad/s
    icm_data.gyro_x = ((float) icm_gyro_x - GyroOffset.Xdata) * PI / 180.f / 16.4f; 
    icm_data.gyro_y = ((float) icm_gyro_y - GyroOffset.Ydata) * PI / 180.f / 16.4f;
    icm_data.gyro_z = ((float) icm_gyro_z - GyroOffset.Zdata) * PI / 180.f / 16.4f;
}

#define delta_T     0.001f  // ��������1ms ��Ƶ��1KHZ

float I_ex, I_ey, I_ez;  // ������
quater_param_t Q_info = {1, 0, 0, 0};  // ��Ԫ����ʼ��
euler_param_t eulerAngle;              // ŷ����
icm_param_t icm_data;                  // ICM20602�ɼ���������ֵ

float icm_kp= 2.f;    // ���ٶȼƵ��������ʱ������� 0.17
float icm_ki= 0.004;   // �������������ʵĻ������� 0.004

/**
 * @brief �û����˲��㷨������������̬(�����ü��ٶȼ����������ǵĻ������)
 * ���ٶȼƶ���֮��������Ƚ����У��������ݼ��������̬���ţ������Ƕ������������У��������ݿ��ţ�������ʹ�û����������(�ڲ������㷨�Ŵ�̬���)��
 * ���ʹ����̬�����˲����������������ǣ��������ż��ٶȼơ�
 * @tips: n - ��������ϵ�� b - ��������ϵ
 */
void icmAHRSupdate(icm_param_t* icm)            
{
    float halfT = 0.5f * delta_T;    // ��������һ��
    float vx, vy, vz;               // ��ǰ��̬��������������������ϵķ���
    float ex, ey, ez;               // ��ǰ���ټƲ�õ��������ٶ��������ϵķ������õ�ǰ��̬��������������������ϵķ��������
    
    float q0 = Q_info.q0;  //��Ԫ��
    float q1 = Q_info.q1;
    float q2 = Q_info.q2;
    float q3 = Q_info.q3;
    
    float q0q0 = q0 * q0;  //����ˣ������������
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;

    // ������ֹ״̬Ϊ-g ����������
    if(icm->acc_x * icm->acc_y * icm->acc_z == 0) // �Ӽƴ�����������״̬ʱ(��ʱg = 0)��������̬���㣬��Ϊ�������ĸ���������
        return;

    // �Լ��ٶ����ݽ��й�һ�� �õ���λ���ٶ� (a^b -> ��������ϵ�µļ��ٶ�)
    float norm = invSqrt(icm->acc_x * icm->acc_x + icm->acc_y * icm->acc_y + icm->acc_z * icm->acc_z); 
    icm->acc_x = icm->acc_x * norm;
    icm->acc_y = icm->acc_y * norm;
    icm->acc_z = icm->acc_z * norm;

    // ��������ϵ���������������ϵķ���
    vx = 2 * (q1q3 - q0q2);
    vy = 2 * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;

    // g^b �� a^b ��������ˣ��õ������ǵ�У����������e��ϵ��
    ex = icm->acc_y * vz - icm->acc_z * vy;
    ey = icm->acc_z * vx - icm->acc_x * vz;
    ez = icm->acc_x * vy - icm->acc_y * vx;

    // ����ۼ�
    I_ex += halfT * ex;  
    I_ey += halfT * ey;
    I_ez += halfT * ez;

    // ʹ��PI�������������������(������Ư�����)
    icm->gyro_x = icm->gyro_x + icm_kp* ex + icm_ki* I_ex;
    icm->gyro_y = icm->gyro_y + icm_kp* ey + icm_ki* I_ey;
    icm->gyro_z = icm->gyro_z + icm_kp* ez + icm_ki* I_ez;

    // һ����������������Ԫ��΢�ַ��̣�����halfTΪ�������ڵ�1/2��gx gy gzΪbϵ�����ǽ��ٶȡ�
    q0 = q0 + (-q1 * icm->gyro_x - q2 * icm->gyro_y - q3 * icm->gyro_z) * halfT;
    q1 = q1 + (q0 * icm->gyro_x + q2 * icm->gyro_z - q3 * icm->gyro_y) * halfT;
    q2 = q2 + (q0 * icm->gyro_y - q1 * icm->gyro_z + q3 * icm->gyro_x) * halfT;
    q3 = q3 + (q0 * icm->gyro_z + q1 * icm->gyro_y - q2 * icm->gyro_x) * halfT;

    // ��λ����Ԫ���ڿռ���תʱ�������죬������ת�Ƕȣ������㷨�������Դ�����������任
    norm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    Q_info.q0 = q0 * norm;
    Q_info.q1 = q1 * norm;
    Q_info.q2 = q2 * norm;
    Q_info.q3 = q3 * norm;  // ��ȫ�ֱ�����¼��һ�μ������Ԫ��ֵ
		
    // atan2�������������������ԭ��������X�������μнǵĻ���ֵ
    eulerAngle.pitch = asin(2 * Q_info.q0 * Q_info.q2 - 2 * Q_info.q1 * Q_info.q3) * 180 / PI; 
    eulerAngle.roll = atan2(2 * Q_info.q2 * Q_info.q3 + 2 * Q_info.q0 * Q_info.q1, -2 * Q_info.q1 * Q_info.q1 - 2 * Q_info.q2 * Q_info.q2 + 1) * 180 / PI; 
    eulerAngle.yaw = atan2(2 * Q_info.q1 * Q_info.q2 + 2 * Q_info.q0 * Q_info.q3, -2 * Q_info.q2 * Q_info.q2 - 2 * Q_info.q3 * Q_info.q3 + 1) * 180 / PI;  
		
}
