/*
 * nrf24l01.c
 *
 * Created: 21-12-2018 22:42:04
 *  Author: Andreas
 */ 

#include "nrf24l01.h"

uint32_t nrf24l01_spi_write(uint8_t *data,uint16_t length)
{
	//spi_select_slave(&spi_master_instance,&nrf24l01_slave_instance,true);
	uint32_t status_code = (uint32_t) spi_write_buffer_wait(&spi_master_instance,data,length);
	//spi_select_slave(&spi_master_instance,&nrf24l01_slave_instance,false);
	return status_code;
}

uint32_t nrf24l01_spi_write_single(uint8_t *data)
{
	return nrf24l01_spi_write(data,1);
}

void nrf24l01_cmd2(uint32_t cmd, uint8_t length, uint8_t data_input)
{
	uint8_t data[length];
	data[0] = (uint8_t) cmd;
	data[1] = data_input;
	nrf24l01_spi_write(&data,2);
}

void nrf24l01_cmd(uint32_t cmd, uint8_t data_input)
{
	nrf24l01_cmd2(cmd,2,data_input);	
}

void nrf24l01_spi_read(uint8_t *data,uint16_t length,uint16_t reg)
{
	//uint16_t dummy = reg | 0x80;
	spi_select_slave(&spi_master_instance,&nrf24l01_slave_instance,true);
	spi_write_buffer_wait(&spi_master_instance,&reg,1);
	spi_read_buffer_wait(&spi_master_instance,data,length,reg);
	//spi_transceive_wait(&spi_master_instance,reg,data);
	spi_select_slave(&spi_master_instance,&nrf24l01_slave_instance,false);
}

void nrf24l01_init(void)
{

	char config = 0;
	char adr[5] = {0x2A,0x2A,0x2A,0x2A,0};//{0x42,0x42,0x42,0x42,0x00};
	
	
	char hello_world[] = "Hello World! Prinsesse";
	
	
	config = NRF_CONFIG_PWR_UP;
	nrf24l01_cmd(NRF_W_REGISTER(NRF_CONFIG_REG),config);
	config = NRF_EN_AA_ENAA_P0;
	nrf24l01_cmd(NRF_W_REGISTER(NRF_EN_AA_REG),config);
	config = NRF_SETUP_AW_4BYTES;
	nrf24l01_cmd(NRF_W_REGISTER(NRF_SETUP_AW_REG),config);
	config = NRF_FEATURE_EN_DPL;
	nrf24l01_cmd(NRF_W_REGISTER(NRF_FEATURE_REG),config);
	config = (1<<0);
	nrf24l01_cmd(NRF_W_REGISTER(NRF_DYNPD_REG),config);
	config = NRF_SETUP_RETR_ARC_RT15 | NRF_SETUP_RETR_ARD_W4000;
	nrf24l01_cmd(NRF_W_REGISTER(NRF_SETUP_RETR_REG),config);
	nrf24l01_cmd2(NRF_W_REGISTER(NRF_TX_ADDR_REG),5,adr);
	nrf24l01_cmd2(NRF_W_REGISTER(NRF_RX_ADDR_P0_REG),5,adr);
	nrf24l01_cmd2(NRF_W_TX_PAYLOAD,strlen(hello_world),hello_world);
}





void NRF_CS_Set(void)
{
	spi_select_slave(&spi_master_instance,&nrf24l01_slave_instance,true);
	//port_pin_set_output_level(NRF24L01_CS,true);
	//PORT_IOBUS->Group[0].OUTSET.bit.OUTSET = PORT_PA10;
}

void NRF_CS_Clear(void)
{
	spi_select_slave(&spi_master_instance,&nrf24l01_slave_instance,false);
	//port_pin_set_output_level(NRF24L01_CS,false);
	//PORT_IOBUS->Group[0].OUTCLR.bit.OUTCLR = PORT_PA10;
}

void NRF_CE_Set(void)
{
	port_pin_set_output_level(NRF24L01_CE,true);
	//PORT_IOBUS->Group[1].OUTSET.bit.OUTSET = PORT_PB11;
}

void NRF_CE_Clear(void)
{
	port_pin_set_output_level(NRF24L01_CE,false);
	//PORT_IOBUS->Group[1].OUTCLR.bit.OUTCLR = PORT_PB11;
}




uint32_t NRF_TX(uint32_t Type, uint32_t Command)
{
	uint32_t Temp=0;
	static char Count;
	Count++;
	if (Count>255)
	{
		Count=0;
	}
	//char Package_Data[4] = {(char)((Type>>8)&0xFF),(char)((Type>>0)&0xFF),(char)((Command>>8)&0xFF),(char)((Command>>0)&0xFF)};
	//char Package_Data[16] = {0x00,0x00,0x00,0x05,Count,0x00,0x00,0x00,0x01,(char)(Type&0x00),(char)((Command>>24)&0xFF),(char)((Command>>16)&0xFF)}(char)((Command>>8)&0xFF),(char)((Command>>0)&0xFF),0x2A,0x42};
	char Package_Data[16] = {0x00,0x00,0x00,0x05,Count,0x00,0x00,0x00,0x01,(char)(Type&0xFF),(char)((Command>>24)&0xFF),(char)((Command>>16)&0xFF),(char)((Command>>8)&0xFF),(char)((Command>>0)&0xFF),0x2A,0x42};
	
	NRF_Write_1(NRF_FLUSH_TX);
	NRF_Write(NRF_W_TX_PAYLOAD,16,&Package_Data);
	
	NRF_CE_Set();
	delay_ms(1);
	do
	{
		Temp = NRF_Read_single(NRF_R_REGISTER(NRF_STATUS_REG));
	}
	while (!(Temp & (NRF_STATUS_TX_DS | NRF_STATUS_MAX_RT)));
	
	
	if (Temp & NRF_STATUS_TX_DS )
	{
		NRF_Write_2(NRF_W_REGISTER(NRF_STATUS_REG),(NRF_STATUS_TX_DS | NRF_STATUS_MAX_RT));
		return NRF_SUCCES;
	}
	else if (Temp & NRF_STATUS_MAX_RT)
	{
		delay_ms(4);
		Temp = NRF_Read_single(NRF_R_REGISTER(NRF_STATUS_REG));
		if (Temp & NRF_STATUS_TX_DS )
		{
			NRF_Write_2(NRF_W_REGISTER(NRF_STATUS_REG),(NRF_STATUS_TX_DS | NRF_STATUS_MAX_RT));
			return NRF_SUCCES;
		}
		else
		{
			NRF_Write_2(NRF_W_REGISTER(NRF_STATUS_REG),(NRF_STATUS_TX_DS | NRF_STATUS_MAX_RT));
			return NRF_FAILURE;
		}
		
	}
	return NRF_FAILURE;
}

void NRF_Enter_TX_Mode(char To_Addr[],char From_Addr[])
{
	NRF_CE_Clear();
	delay_ms(1);
	NRF_Write_2(NRF_W_REGISTER(NRF_CONFIG_REG),NRF_CONFIG_PWR_UP);

	NRF_Write_2(NRF_W_REGISTER(NRF_EN_AA_REG),NRF_EN_AA_ENAA_P0);

	NRF_Write_2(NRF_W_REGISTER(NRF_SETUP_AW_REG),NRF_SETUP_AW_4BYTES); //NRF_SETUP_AW_4BYTES

	NRF_Write_2(NRF_W_REGISTER(NRF_FEATURE_REG),NRF_FEATURE_EN_DPL);
	
	NRF_Write_2(NRF_W_REGISTER(NRF_DYNPD_REG),NRF_DYNPD_DPL_P0);

	NRF_Write_2(NRF_W_REGISTER(NRF_SETUP_RETR_REG),(NRF_SETUP_RETR_ARC_RT15 | NRF_SETUP_RETR_ARD_W4000));
	//To_Addr[4] = 0xA1;
	NRF_Write(NRF_W_REGISTER(NRF_TX_ADDR_REG),5,To_Addr);
	NRF_Write(NRF_W_REGISTER(NRF_RX_ADDR_P0_REG),5,From_Addr);
}



void NRF_Enter_RX_Mode(char Addr[])
{
	
	NRF_CE_Clear();
	delay_ms(1);
	NRF_Write_2(NRF_W_REGISTER(NRF_CONFIG_REG), (NRF_CONFIG_PRIM_RX | NRF_CONFIG_PWR_UP));
	
	NRF_Write_2(NRF_W_REGISTER(NRF_EN_RXADDR_REG), (NRF_EN_RXADDR_ERX_P0 | NRF_EN_RXADDR_ERX_P1));
	NRF_Write_2(NRF_W_REGISTER(NRF_EN_AA_REG), (NRF_EN_AA_ENAA_P0 | NRF_EN_AA_ENAA_P1));
	NRF_Write_2(NRF_W_REGISTER(NRF_SETUP_AW_REG), (NRF_SETUP_AW_4BYTES));
	
	
	NRF_Write(NRF_W_REGISTER(NRF_RX_ADDR_P0_REG),5, Addr);
	
	
	NRF_Write_2(NRF_W_REGISTER(NRF_FEATURE_REG),(NRF_FEATURE_EN_DPL));
	NRF_Write_2(NRF_W_REGISTER(NRF_DYNPD_REG),(NRF_DYNPD_DPL_P0));
	//NRF_Write(NRF_W_REGISTER(NRF_RX_PW_P0_REG),13);
	//NRF_Write(NRF_W_REGISTER(NRF_RX_PW_P1_REG),13);
	
	NRF_CE_Set();
}



uint32_t NRF_Read(uint32_t Command,uint32_t Length, char *buffer)
{
	uint32_t temp = 0x00;
	uint32_t Status_Reg=0;
	uint32_t i = 0;
	NRF_CS_Set();
	Status_Reg = nrf24l01_spi_write_single(&Command);
	for (i = 0; i < Length; i++)
	{
		buffer[i] = nrf24l01_spi_write_single(&temp);
	}
	NRF_CS_Clear();
	
	return Status_Reg;
}

uint32_t NRF_Read_single(uint32_t Command)
{
	char buffer;
	//NRF_Read(Command,1,&buffer);
	nrf24l01_spi_read(&buffer,1,(uint16_t)Command);
	return buffer;
}


uint32_t NRF_Write(uint32_t Command, uint32_t Length, char *Input)
{
	uint32_t Temp=0;
	char *buffer = Input;
	uint32_t i = 0;
	NRF_CS_Set();
	Temp = nrf24l01_spi_write_single(&Command);
	for (i = 0;i<Length;i++)
	{
		Temp = nrf24l01_spi_write_single(buffer);
		buffer++;
	}
	NRF_CS_Clear();
	
	return Temp;
}

uint32_t NRF_Write_2(uint32_t Command, char Input)
{
	return NRF_Write(Command,1,&Input);
}

uint32_t NRF_Write_1(uint32_t Command)
{
	return NRF_Write(Command,0,'\0');
}


uint32_t NRF_TX_data(uint8_t *data, uint32_t length)
{
	if (length > 16)
	{
		return NRF_FAILURE;
	}
	
	uint32_t Temp=0;
	static char Count;
	Count++;
	if (Count>255)
	{
		Count=0;
	}
	//char Package_Data[4] = {(char)((Type>>8)&0xFF),(char)((Type>>0)&0xFF),(char)((Command>>8)&0xFF),(char)((Command>>0)&0xFF)};
	//char Package_Data[16] = {0x00,0x00,0x00,0x05,Count,0x00,0x00,0x00,0x01,(char)(Type&0x00),(char)((Command>>24)&0xFF),(char)((Command>>16)&0xFF)}(char)((Command>>8)&0xFF),(char)((Command>>0)&0xFF),0x2A,0x42};
	char Package_Data[20] = {0x2A,0x42,0x05,Count,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x2A,0x42};
	for (int i=0;i<length;i++)
	{
		Package_Data[i+4] = data[i];
	}
	
	
	NRF_Write_1(NRF_FLUSH_TX);
	NRF_Write(NRF_W_TX_PAYLOAD,20,&Package_Data);
	
	NRF_CE_Set();
	delay_ms(1);
	do
	{
		Temp = NRF_Read_single(NRF_R_REGISTER(NRF_STATUS_REG));
	}
	while (!(Temp & (NRF_STATUS_TX_DS | NRF_STATUS_MAX_RT)));
	
	
	if (Temp & NRF_STATUS_TX_DS )
	{
		NRF_Write_2(NRF_W_REGISTER(NRF_STATUS_REG),(NRF_STATUS_TX_DS | NRF_STATUS_MAX_RT));
		return NRF_SUCCES;
	}
	else if (Temp & NRF_STATUS_MAX_RT)
	{
		delay_ms(4);
		Temp = NRF_Read_single(NRF_R_REGISTER(NRF_STATUS_REG));
		if (Temp & NRF_STATUS_TX_DS )
		{
			NRF_Write_2(NRF_W_REGISTER(NRF_STATUS_REG),(NRF_STATUS_TX_DS | NRF_STATUS_MAX_RT));
			return NRF_SUCCES;
		}
		else
		{
			NRF_Write_2(NRF_W_REGISTER(NRF_STATUS_REG),(NRF_STATUS_TX_DS | NRF_STATUS_MAX_RT));
			return NRF_FAILURE;
		}
		
	}
	return NRF_FAILURE;
}