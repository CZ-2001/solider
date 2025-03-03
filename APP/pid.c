#include "pid.h"
#include "user_lib.h"
// �궨���޷�����
#define LimitMax(input, max)   \
    {                          \
        if (input > max)       \
            input = max;       \
        else if (input < -max) \
            input = -max;      \
    }

void PID_Init(PidTypeDef *pid, uint8_t mode, fp32 *PID)
{
    if (pid == NULL)
    {
        return;
    }
    pid->mode = mode;
    pid->Kp = PID[0];
    pid->Ki = PID[1];
    pid->Kd = PID[2];
    pid->max_iout = PID[3];
    pid->max_out = PID[4];

    //    pid->diff_max = diff_max;
    //    pid->diff_min = diff_min;
    pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
    pid->error[0] = pid->error[1] = pid->error[2] = pid->Pout = pid->Iout = pid->Dout = pid->out = 0.0f;
}

void PID_diff_init(PidTypeDef *pid, fp32 diff_max, fp32 diff_min)
{
    pid->diff_max = diff_max;
    pid->diff_min = diff_min;
}
void Handle_Over_Zero(float *set, float *ref, float T)
{
    if (*set - *ref > (T / 2)) // 4096 ����Ȧ��е�Ƕ�
    {
        *ref += T;
    }
    else if (*set - *ref < -(T / 2))
    {
        *ref = *ref - T;
    }
}

fp32 PID_Calc(PidTypeDef *pid, fp32 ref, fp32 set)
{

    uint8_t index;
    if (pid == NULL)
    {
        return 0.0f;
    }

    pid->error[2] = pid->error[1];
    pid->error[1] = pid->error[0];
    pid->set = set;
    pid->fdb = ref;
    pid->error[0] = set - ref;

    if (pid->mode == PID_POSITION)
    {
        pid->Pout = pid->Kp * pid->error[0];
        pid->Iout += pid->Ki * pid->error[0];
        pid->Dbuf[2] = pid->Dbuf[1];
        pid->Dbuf[1] = pid->Dbuf[0];
        pid->Dbuf[0] = (pid->error[0] - pid->error[1]);
        pid->Dout = pid->Kd * pid->Dbuf[0];
        LimitMax(pid->Iout, pid->max_iout);
        pid->out = pid->Pout + pid->Iout + pid->Dout;
        LimitMax(pid->out, pid->max_out);
    }
    else if (pid->mode == PID_DELTA)
    {
        pid->Pout = pid->Kp * (pid->error[0] - pid->error[1]);
        pid->Iout = pid->Ki * pid->error[0];
        pid->Dbuf[2] = pid->Dbuf[1];
        pid->Dbuf[1] = pid->Dbuf[0];
        pid->Dbuf[0] = (pid->error[0] - 2.0f * pid->error[1] + pid->error[2]);
        pid->Dout = pid->Kd * pid->Dbuf[0];
        pid->out += pid->Pout + pid->Iout + pid->Dout;
        LimitMax(pid->out, pid->max_out);
    }
    else if (pid->mode == PID_DIFF)
    {
        if (ABS(pid->error[0]) > pid->diff_max)
            index = 0;
        else if (ABS(pid->error[0]) < pid->diff_min)
            index = 1;
        else
            index = (pid->diff_max - ABS(pid->error[0])) / (pid->diff_max - pid->diff_min);
        pid->Iout += pid->Ki * pid->error[0];
        LimitMax(pid->Iout, pid->max_iout);
        pid->out = pid->Kp * pid->error[0] + index * pid->Iout + pid->Kd * (pid->error[0] - pid->error[1]);
        LimitMax(pid->out, pid->max_out);
    }
    return pid->out;
}

fp32 PID_Calc_add_limit(PidTypeDef *pid, fp32 ref, fp32 set, float T)
{
    uint8_t index;
    if (pid == NULL)
    {
        return 0.0f;
    }
    Handle_Over_Zero(&set, &ref, T);
    pid->error[2] = pid->error[1];
    pid->error[1] = pid->error[0];
    pid->set = set;
    pid->error[0] = set - ref;
    if (pid->mode == PID_POSITION)
    {
        pid->Pout = pid->Kp * pid->error[0];
        pid->Iout += pid->Ki * pid->error[0];
        pid->Dbuf[2] = pid->Dbuf[1];
        pid->Dbuf[1] = pid->Dbuf[0];
        pid->Dbuf[0] = (pid->error[0] - pid->error[1]);
        pid->Dout = pid->Kd * pid->Dbuf[0];
        LimitMax(pid->Iout, pid->max_iout);
        pid->out = pid->Pout + pid->Iout + pid->Dout;
        LimitMax(pid->out, pid->max_out);
    }
    else if (pid->mode == PID_DELTA)
    {
        pid->Pout = pid->Kp * (pid->error[0] - pid->error[1]);
        pid->Iout = pid->Ki * pid->error[0];
        pid->Dbuf[2] = pid->Dbuf[1];
        pid->Dbuf[1] = pid->Dbuf[0];
        pid->Dbuf[0] = (pid->error[0] - 2.0f * pid->error[1] + pid->error[2]);
        pid->Dout = pid->Kd * pid->Dbuf[0];
        pid->out += pid->Pout + pid->Iout + pid->Dout;
        LimitMax(pid->out, pid->max_out);
    }
    else if (pid->mode == PID_DIFF)
    {
        if (ABS(pid->error[0]) > pid->diff_max)
            index = 0;
        else if (ABS(pid->error[0]) < pid->diff_min)
            index = 1;
        else
            index = (pid->diff_max - ABS(pid->error[0])) / (pid->diff_max - pid->diff_min);
        pid->Iout += pid->Ki * pid->error[0];
        pid->out = pid->Kp * pid->error[0] + index * pid->Iout + pid->Kd * (pid->error[0] - pid->error[1]);
        LimitMax(pid->out, pid->max_out);
    }

    return pid->out;
}

void PID_clear(PidTypeDef *pid)
{
    if (pid == NULL)
    {
        return;
    }

    pid->error[0] = pid->error[1] = pid->error[2] = 0.0f;
    pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
    pid->out = pid->Pout = pid->Iout = pid->Dout = 0.0f;
    pid->set = 0.0f;
}



fp32 PID_CALCAL(PID_DOUBLE *PID, float angle_target, float angle_feedback, float speed_feedback, float inner_limit)
{
    fp32 inner_output, outer_output;
    inner_output = PID_Calc_add_limit(&PID->inner, angle_feedback, angle_target, inner_limit);
    outer_output = PID_Calc(&PID->outer, speed_feedback, inner_output);
    //	PID->output=outer_output;
    return outer_output;
}
