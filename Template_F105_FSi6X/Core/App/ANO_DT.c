#include "ANO_DT.h"
#include "usart.h"

host_computer_debugg hc_debugg;
// 数据发送缓冲区
uint8_t DataToSend[100]={0}; 

void ANODT_Send40(host_computer_debugg* hc_debugg)  //小端模式  
{
	uint8_t _cnt = 0;
	DataToSend[_cnt++] = 0xAA;    // 帧头
	DataToSend[_cnt++] = 0xFF;   	//  帧头
	DataToSend[_cnt++] = 0x40;//0xF1;  	//功能码
	DataToSend[_cnt++] = 20;// 9;     	//数据长度

	DataToSend[_cnt++] = BYTE0(hc_debugg->rc.ROL);
	DataToSend[_cnt++] = BYTE1(hc_debugg->rc.ROL);

	DataToSend[_cnt++] = BYTE0(hc_debugg->rc.PIT);
	DataToSend[_cnt++] = BYTE1(hc_debugg->rc.PIT);

	
	DataToSend[_cnt++] = BYTE0(hc_debugg->rc.throttle);
	DataToSend[_cnt++] = BYTE1(hc_debugg->rc.throttle);
	
	DataToSend[_cnt++] = BYTE0(hc_debugg->rc.steering);
	DataToSend[_cnt++] = BYTE1(hc_debugg->rc.steering);
	
	DataToSend[_cnt++] = BYTE0(hc_debugg->rc.aux1);
	DataToSend[_cnt++] = BYTE1(hc_debugg->rc.aux1);

	DataToSend[_cnt++] = BYTE0(hc_debugg->rc.aux2);
	DataToSend[_cnt++] = BYTE1(hc_debugg->rc.aux2);

	DataToSend[_cnt++] = BYTE0(hc_debugg->rc.aux3);
	DataToSend[_cnt++] = BYTE1(hc_debugg->rc.aux3);

	DataToSend[_cnt++] = BYTE0(hc_debugg->rc.aux4);
	DataToSend[_cnt++] = BYTE1(hc_debugg->rc.aux4);

	DataToSend[_cnt++] = BYTE0(hc_debugg->rc.var[0]);
	DataToSend[_cnt++] = BYTE1(hc_debugg->rc.var[0]);
	
	DataToSend[_cnt++] = BYTE0(hc_debugg->rc.var[1]);
	DataToSend[_cnt++] = BYTE1(hc_debugg->rc.var[1]);
	
	uint8_t sc = 0; //和校验
	uint8_t ac = 0; //附加校验
	
	for(uint8_t i=0; i<DataToSend[3]+4; i++)
	{
		sc +=DataToSend[i];
		ac +=sc;
	}
	
	DataToSend[_cnt++] = sc;
	DataToSend[_cnt++] = ac;
	
	HAL_UART_Transmit_DMA(&huart2,DataToSend,_cnt);	
}


void ANODT_SendF1(host_computer_debugg* hc_debugg)  //小端模式
{
	uint8_t _cnt = 0;
	DataToSend[_cnt++] = 0xAA;    // 帧头
	DataToSend[_cnt++] = 0xFF;   	//  帧头
	DataToSend[_cnt++] = 0xF1;  	//功能码
	DataToSend[_cnt++] = 14;     	//数据长度

	DataToSend[_cnt++] = BYTE0(hc_debugg->VCOut.rpm[0]);
	DataToSend[_cnt++] = BYTE1(hc_debugg->VCOut.rpm[0]);
	DataToSend[_cnt++] = BYTE0(hc_debugg->VCOut.gear[0]);

	DataToSend[_cnt++] = BYTE0(hc_debugg->VCOut.rpm[1]);
	DataToSend[_cnt++] = BYTE1(hc_debugg->VCOut.rpm[1]);
	DataToSend[_cnt++] = BYTE0(hc_debugg->VCOut.gear[1]);
	
	DataToSend[_cnt++] = BYTE0(hc_debugg->VCOut.steering[0]);
	DataToSend[_cnt++] = BYTE1(hc_debugg->VCOut.steering[0]);
	
	DataToSend[_cnt++] = BYTE0(hc_debugg->VCOut.steering[1]);
	DataToSend[_cnt++] = BYTE1(hc_debugg->VCOut.steering[1]);
	
	DataToSend[_cnt++] = BYTE0(hc_debugg->VCOut.brake);
	
	DataToSend[_cnt++] = BYTE0(hc_debugg->VCOut.sw[0]);
	DataToSend[_cnt++] = BYTE0(hc_debugg->VCOut.sw[1]);
	DataToSend[_cnt++] = BYTE0(hc_debugg->VCOut.sw[2]);
		
	uint8_t sc = 0; //和校验
	uint8_t ac = 0; //附加校验
	
	for(uint8_t i=0; i<DataToSend[3]+4; i++)
	{
		sc +=DataToSend[i];
		ac +=sc;
	}
	
	DataToSend[_cnt++] = sc;
	DataToSend[_cnt++] = ac;
	
	HAL_UART_Transmit_DMA(&huart2,DataToSend,_cnt);	
}



void ANODT_SendF2(host_computer_debugg* hc_debugg)  //小端模式
{
	uint8_t _cnt = 0;
	DataToSend[_cnt++] = 0xAA;    // 帧头
	DataToSend[_cnt++] = 0xFF;   	//  帧头
	DataToSend[_cnt++] = 0xF2;  	//功能码
	DataToSend[_cnt++] = 8;     	//数据长度

	DataToSend[_cnt++] = BYTE0(hc_debugg->host_c_down.speed);
	DataToSend[_cnt++] = BYTE1(hc_debugg->host_c_down.speed);
	
	DataToSend[_cnt++] = BYTE0(hc_debugg->host_c_down.gear);
	
	
	DataToSend[_cnt++] = BYTE0(hc_debugg->host_c_down.steering);
	DataToSend[_cnt++] = BYTE1(hc_debugg->host_c_down.steering);
	
	DataToSend[_cnt++] = BYTE0(hc_debugg->host_c_down.brake);
	
	DataToSend[_cnt++] = BYTE0(hc_debugg->host_c_down.sw);
	DataToSend[_cnt++] = BYTE1(hc_debugg->host_c_down.sw);
		
	uint8_t sc = 0; //和校验
	uint8_t ac = 0; //附加校验
	
	for(uint8_t i=0; i<DataToSend[3]+4; i++)
	{
		sc +=DataToSend[i];
		ac +=sc;
	}
	
	DataToSend[_cnt++] = sc;
	DataToSend[_cnt++] = ac;
	
	HAL_UART_Transmit_DMA(&huart2,DataToSend,_cnt);	
}

void ANODT_SendF3(host_computer_debugg* hc_debugg)  //小端模式
{
	uint8_t _cnt = 0;
	DataToSend[_cnt++] = 0xAA;    // 帧头
	DataToSend[_cnt++] = 0xFF;   	//  帧头
	DataToSend[_cnt++] = 0xF3;  	//功能码
	DataToSend[_cnt++] = 12;     	//数据长度

	DataToSend[_cnt++] = BYTE0(hc_debugg->vehicle_status.rpm);
	DataToSend[_cnt++] = BYTE1(hc_debugg->vehicle_status.rpm);
	
	DataToSend[_cnt++] = BYTE0(hc_debugg->vehicle_status.steering);
	DataToSend[_cnt++] = BYTE1(hc_debugg->vehicle_status.steering);

	DataToSend[_cnt++] = BYTE0(hc_debugg->vehicle_status.speed);
	DataToSend[_cnt++] = BYTE1(hc_debugg->vehicle_status.speed);
	
	DataToSend[_cnt++] = BYTE0(hc_debugg->vehicle_status.angle);
	DataToSend[_cnt++] = BYTE1(hc_debugg->vehicle_status.angle);
	
	DataToSend[_cnt++] = BYTE0(hc_debugg->vehicle_status.gear);
	
	DataToSend[_cnt++] = BYTE0(hc_debugg->vehicle_status.brake);
	
	DataToSend[_cnt++] = BYTE0(hc_debugg->vehicle_status.Rover);
	
	DataToSend[_cnt++] = BYTE0(hc_debugg->vehicle_status.mode);
		
	uint8_t sc = 0; //和校验
	uint8_t ac = 0; //附加校验
	
	for(uint8_t i=0; i<DataToSend[3]+4; i++)
	{
		sc +=DataToSend[i];
		ac +=sc;
	}
	
	DataToSend[_cnt++] = sc;
	DataToSend[_cnt++] = ac;
	
	HAL_UART_Transmit_DMA(&huart2,DataToSend,_cnt);	
}

void ANODT_SendF4(host_computer_debugg* hc_debugg)  //小端模式
{
	uint8_t _cnt = 0;
	DataToSend[_cnt++] = 0xAA;    // 帧头
	DataToSend[_cnt++] = 0xFF;   	//  帧头
	DataToSend[_cnt++] = 0xF4;  	//功能码
	DataToSend[_cnt++] = 16;     	//数据长度

	DataToSend[_cnt++] = BYTE0(hc_debugg->Sensor.Dmotor_current[0]);
	DataToSend[_cnt++] = BYTE1(hc_debugg->Sensor.Dmotor_current[0]);
	
	DataToSend[_cnt++] = BYTE0(hc_debugg->Sensor.Dmotor_current[1]);
	DataToSend[_cnt++] = BYTE1(hc_debugg->Sensor.Dmotor_current[1]);

	DataToSend[_cnt++] = BYTE0(hc_debugg->Sensor.Dmotor_tem[0]);
	DataToSend[_cnt++] = BYTE1(hc_debugg->Sensor.Dmotor_tem[0]);
	
	DataToSend[_cnt++] = BYTE0(hc_debugg->Sensor.Dmotor_tem[1]);
	DataToSend[_cnt++] = BYTE1(hc_debugg->Sensor.Dmotor_tem[1]);
	
	DataToSend[_cnt++] = BYTE0(hc_debugg->Sensor.Smotor_current[0]);
	DataToSend[_cnt++] = BYTE1(hc_debugg->Sensor.Smotor_current[0]);

	DataToSend[_cnt++] = BYTE0(hc_debugg->Sensor.Smotor_current[1]);
	DataToSend[_cnt++] = BYTE1(hc_debugg->Sensor.Smotor_current[1]);

	DataToSend[_cnt++] = BYTE0(hc_debugg->Sensor.Smotor_tem[0]);
	DataToSend[_cnt++] = BYTE1(hc_debugg->Sensor.Smotor_tem[0]);

	DataToSend[_cnt++] = BYTE0(hc_debugg->Sensor.Smotor_tem[1]);
	DataToSend[_cnt++] = BYTE1(hc_debugg->Sensor.Smotor_tem[1]);

	uint8_t sc = 0; //和校验
	uint8_t ac = 0; //附加校验
	
	for(uint8_t i=0; i<DataToSend[3]+4; i++)
	{
		sc +=DataToSend[i];
		ac +=sc;
	}
	
	DataToSend[_cnt++] = sc;
	DataToSend[_cnt++] = ac;
	
	HAL_UART_Transmit_DMA(&huart2,DataToSend,_cnt);	
}

//void ANODT_SendF5(host_computer_debugg* hc_debugg)  //小端模式
//{
//	uint8_t _cnt = 0;
//	DataToSend[_cnt++] = 0xAA;    // 帧头
//	DataToSend[_cnt++] = 0xFF;   	//  帧头
//	DataToSend[_cnt++] = 0xF5;  	//功能码
//	DataToSend[_cnt++] = 17;     	//数据长度

//	DataToSend[_cnt++] = BYTE0(hc_debugg->VCOut.rpm);
//	DataToSend[_cnt++] = BYTE1(hc_debugg->VCOut.rpm);
//	
//	DataToSend[_cnt++] = BYTE0(hc_debugg->VCOut.throttle);
//	DataToSend[_cnt++] = BYTE1(hc_debugg->VCOut.throttle);

//	DataToSend[_cnt++] = BYTE0(hc_debugg->VCOut.steering);
//	DataToSend[_cnt++] = BYTE1(hc_debugg->VCOut.steering);

//	DataToSend[_cnt++] = BYTE0(hc_debugg->VCOut.steering_a[0]);
//	DataToSend[_cnt++] = BYTE1(hc_debugg->VCOut.steering_a[0]);

//	DataToSend[_cnt++] = BYTE0(hc_debugg->VCOut.steering_a[1]);
//	DataToSend[_cnt++] = BYTE1(hc_debugg->VCOut.steering_a[1]);

//	DataToSend[_cnt++] = BYTE0(hc_debugg->VCOut.steering_a[2]);
//	DataToSend[_cnt++] = BYTE1(hc_debugg->VCOut.steering_a[2]);

//	DataToSend[_cnt++] = BYTE0(hc_debugg->VCOut.steering_a[3]);
//	DataToSend[_cnt++] = BYTE1(hc_debugg->VCOut.steering_a[3]);

//	DataToSend[_cnt++] = BYTE0(hc_debugg->VCOut.brake);
//	
//	DataToSend[_cnt++] = BYTE0(hc_debugg->VCOut.sw);
//	DataToSend[_cnt++] = BYTE1(hc_debugg->VCOut.sw);

//	uint8_t sc = 0; //和校验
//	uint8_t ac = 0; //附加校验
//	
//	for(uint8_t i=0; i<DataToSend[3]+4; i++)
//	{
//		sc +=DataToSend[i];
//		ac +=sc;
//	}
//	
//	DataToSend[_cnt++] = sc;
//	DataToSend[_cnt++] = ac;
//	
//	HAL_UART_Transmit_DMA(&huart2,DataToSend,_cnt);	
//}
