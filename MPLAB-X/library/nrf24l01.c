#include "nrf24l01.h"
#include "macro.h"

#define     _XTAL_FREQ        _XTAL_FREQ_125K       // ��ǰ����ʹ��Ƶ�ʶ���

// NRF24L01 ���I/O����

#define     NRF_CE_DIR          TRISA0
#define     NRF_CE              RA0         // 24L01Ƭѡ�ź� ���ߵ�ƽ ���ͽ�������
#define     NRF_CSN_DIR         TRISA2         
#define     NRF_CSN             RA2         // SPI SS Ƭѡ

#define     NRF_SCK_DIR         TRISC0
#define     NRF_SCK             RC0
#define     NRF_MOSI_DIR        TRISC1
#define     NRF_MOSI            RC1
#define     NRF_MISO_DIR        TRISC2
#define     NRF_MISO            RC2

#define     NRF_IRQ_DIR         TRISA4      // �е�û�����ô˽ӿ�
#define     NRF_IRQ             RA4


// NRF24L01 ���� 0���ϵ磬1���ϵ�  �������� MH Pad ��·��
#define     NRF_EN_DIR          TRISA5
#define     NRF_EN              RA5


/*
 * д��ַʱ��д����ֽڣ���д����ֽ�
 * ͨ��2-5�ĵ�ַ��ǰ4���ֽ�[39:8]��ͨ��1�ĵ�ַͨ�ã����һλ���ֽڲ�ͬ
 */
unsigned char TX_ADDRESS[5] = {0x0F, 0x0E, 0x0D, 0x0C, 0x0B};           //���÷��ͽ��յ�ַ

unsigned char RX_ADDRESS_P0[5] = {0x0F, 0x0E, 0x0D, 0x0C, 0x0B};        // ͨ��0���յ�ַ
unsigned char RX_ADDRESS_P1[5] = {0x0A, 0x10, 0x09, 0x08, 0x07};        // ͨ��1���յ�ַ
unsigned char RX_ADDRESS_P2[5] = {0x0B, 0x10, 0x09, 0x08, 0x07};        // ͨ��2���յ�ַ
unsigned char RX_ADDRESS_P3[5] = {0x0C, 0x10, 0x09, 0x08, 0x07};        // ͨ��3���յ�ַ
unsigned char RX_ADDRESS_P4[5] = {0x0D, 0x10, 0x09, 0x08, 0x07};        // ͨ��4���յ�ַ
unsigned char RX_ADDRESS_P5[5] = {0x0E, 0x10, 0x09, 0x08, 0x07};        // ͨ��5���յ�ַ


/*
 * ��ض˿�����
 */
void NRF_Port_Init(void) {  
    NRF_MOSI_DIR = 1;
    NRF_MISO_DIR = 0;
    NRF_SCK_DIR = 0;
    NRF_CSN_DIR = 0;
    NRF_CE_DIR  = 0;
    
//    NRF_IRQ_DIR = 1;
//    NRF_EN_DIR = 0;  
    
    NRF_CE  = 0;            // ����
    NRF_CSN = 1;            // Ƭѡ
}

void NRF_On(void) {
    NRF_EN = 0;             // �����ܿ����ϵ�
}

void NRF_Down(void) {
    NRF_EN = 1;             // �����ܿ��Ƶ���
}


/*
 * SPIͨ������
 */
void NRF_SPI_Init(void) {  
    SSP1STAT = 0x00;
    SSP1STATbits.CKE = 1;           // ʱ��״̬�ӿ���ת������Чʱ����

    SSP1BUF = 0;                    // �������
    SSP1IF = 0;                     // ����жϱ�־λ          
    SSP1CON1 = 0x20;                // ʹ��SPI����ͨ��      
}

unsigned char NRF_SPI_RW(unsigned char dat) {
    SSP1IF = 0;                     // 
    SSP1BUF = dat;
    while(!SSP1IF);                 // д��ɺ�BF��1����ȡSSP1BUF��BF�ᱻ����
    
    return(SSP1BUF);
}  

unsigned char NRF_Read_Reg(unsigned char reg) {
	unsigned char status;
  	NRF_CSN = 0;                        // Ƭѡ��Ч
  	NRF_SPI_RW(reg);                    // д��Ĵ�����ַ,
  	status = NRF_SPI_RW(NRF_NOP);    	// ���д��Ĵ�������,�����Ĵ�������
  	NRF_CSN = 1;                        // ƬѡʧЧ
  	return(status);                     // ���ؼĴ�������
}
//---------------------------------------
//����: �Ĵ������ʺ���
//---------------------------------------
//����������
//�Ĵ������ʺ���:��������24L01�ļĴ�����ֵ,����˼·����ͨ��WRITE_REG����(Ҳ����
//0x20+�Ĵ�����ַ)��Ҫ�趨��ֵд����Ӧ�ļĴ�����ַ����ȥ,����ȡ����ֵ.���ں���
//��˵Ҳ���ǰ�valueֵд��reg�Ĵ����С���Ҫע�����,����NRF24L01֮ǰ����Ҫʹ��
//оƬ(CSN=0),�������Ժ�ʧ��оƬ(CSN=1)
unsigned char NRF_Write_Reg(unsigned char reg, unsigned char value) {
	unsigned char status;
  	NRF_CSN = 0;                   	// 24L01Ƭѡ��Ч
  	status = NRF_SPI_RW(reg);       // д��Ĵ�����ַ,ͬʱ����״̬�Ĵ�������
  	NRF_SPI_RW(value);              // д��Ĵ�������
  	NRF_CSN = 1;                   	// 24L01ƬѡʧЧ
  	return(status);                 // ����״̬�Ĵ�������
}


//---------------------------------------
//����: ���ջ��������ʺ���
//---------------------------------------
//����������
//�����ڽ���ʱ��ȡFIFO �������е�ֵ,����˼·����ͨ��READ_REG��������ݴӽ���FIF(RD_RX_PLOA)
//�ж������浽��������ȥ��
unsigned char NRF_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char len)
{
	unsigned char status,i;
  	NRF_CSN = 0;                        // 24L01Ƭѡ��Ч
  	status = NRF_SPI_RW(reg);               // д��Ĵ�����ַ
  	for(i = 0; i < len; i++)            // ��ȡI������
	{
   		pBuf[i] = NRF_SPI_RW(NRF_NOP);    	// �����Ĵ������ݷ�������
  	}
	NRF_CSN = 1;                        // 24L01ƬѡʧЧ
  	return(status);                     // ����״̬�Ĵ�������
}


//---------------------------------------
//����: ���仺�������ʺ���--------------
//����������
//Ҫ����������������ŵ�����FIFO��������,����˼·����ͨ��WRITE_REG��������ݴ浽����
//FIFO(WR_TX_PLOAD)��ȥ��
unsigned char NRF_Write_Buf(unsigned char reg, unsigned char *pBuf, unsigned char len)
{
	unsigned char status, i;
  	NRF_CSN = 0;                   	// 24L01Ƭѡ��Ч
  	status = NRF_SPI_RW(reg);    		// д��Ĵ�����ַ
  	for(i = 0; i < len; i++) {      // д��I������
    	NRF_SPI_RW(*pBuf++);            // ����������д��Ĵ���
	}
  	NRF_CSN = 1;                 		// 24L01ƬѡʧЧ
  	return(status);                 // ����״̬�Ĵ�������
}

//---------------------------------------
//����: 24L01���纯�� �����ܹر� ֹͣ����
//---------------------------------------
void NRF_Power_off() {
    NRF_CE = 0;
    NRF_Write_Reg(NRF_W_REGISTER + NRF_CONFIG, 0x00);  
}

char NRF_Check(void) {
    unsigned char tuf[5] = {0xE1, 0xE2, 0xE3, 0xE4, 0x05};
	unsigned char buf[5] = {0, 0, 0, 0, 0};
	unsigned char i;
    NRF_Write_Buf(NRF_W_REGISTER + NRF_TX_ADDR, tuf, 5);        // д��5���ֽڵĵ�ַ
	NRF_Read_Buf(NRF_TX_ADDR, buf, 5);                          // ����д��ĵ�ַ  
	for(i = 0; i < 5; i++) if(tuf[i] != buf[i]) break;	 							   
	if(i != 5) return 1;                // ���24L01����	
	return 0;                           // ��⵽24L01
}	

//---------------------------------------
//����: �趨24L01Ϊ���ͷ�ʽ����------------
void NRF_TX_Mode(unsigned char *txbuf) {

    NRF_CE = 0;
    
    NRF_Write_Buf(NRF_W_REGISTER + NRF_TX_ADDR, RX_ADDRESS_P0, 5);     // Writes TX_Address to nRF24L01 д���͵�ַ
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P0, RX_ADDRESS_P0, 5);  // RX_Addr0 same as TX_Adr for Auto.Ack ͨ��0��Ϊ����Ӧ���źţ��뷢�͵�ַҪһ��
    NRF_Write_Buf(NRF_W_TX_PAYLOAD, txbuf, TX_PLOAD_WIDTH);         // Writes data to TX payload д��������

    NRF_Write_Reg(NRF_W_REGISTER + NRF_EN_AA, 0x01);            // Enable Auto.Ack:Pipe0
    NRF_Write_Reg(NRF_W_REGISTER + NRF_EN_RXADDR, 0x01);        // Enable Pipe0
    NRF_Write_Reg(NRF_W_REGISTER + NRF_SETUP_RETR, 0x1a);       // 500?s + 86?s, 10 retrans...
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RF_CH, 40);              // Select RF channel 40
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RF_SETUP, 0x0f);         // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
    NRF_Write_Reg(NRF_W_REGISTER + NRF_CONFIG, 0x0e);           // Set PWR_UP bit, enable CRC(2 bytes) & Prim:TX. MAX_RT & TX_DS enabled..

    NRF_CE = 1;

}

//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
unsigned char NRF_TxPacket(unsigned char *txbuf) {
    
	unsigned char status = TX_DS;
    
	NRF_TX_Mode(txbuf);

    do {
        status = NRF_Read_Reg(NRF_STATUS);                      // ��ȡ״̬�Ĵ�����ֵ	 
        if(status & MAX_RT || status & TX_DS) break;
    }while(1);
    
    if(status & MAX_RT) {
        status = MAX_RT;
    }
    if(status & TX_DS) {
        status = TX_DS;
    }

    NRF_CE = 0;
    NRF_Write_Reg(NRF_W_REGISTER + NRF_STATUS, 0xff);               // ���TX_DS��MAX_RT�жϱ�־
    NRF_Write_Reg(NRF_FLUSH_TX, 0);                              // ���RX FIFO�Ĵ��� 
    
    return status;
}

/*
 * ����: �趨24L01Ϊ���շ�ʽ����
 * ��������ͨ����������
 */
void NRF_RX_Mode() {
    
    unsigned char enaa = 0x00;
    
    NRF_CE = 0;	  
    

    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P0, RX_ADDRESS_P0, 5);       // Use the same address on the RX device as the TX device
                                                                                // Select same RX payload width as TX Payload width
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P1, RX_ADDRESS_P1, 5);       
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P2, RX_ADDRESS_P2, 1);
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P3, RX_ADDRESS_P3, 1);
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P4, RX_ADDRESS_P4, 1);
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P5, RX_ADDRESS_P5, 1);

  
    NRF_Write_Reg(NRF_W_REGISTER + NRF_EN_AA, 0x3F);                            // Enable Auto.Ack:Pipe0 ʹ������ͨ���Զ�Ӧ��
    NRF_Write_Reg(NRF_W_REGISTER + NRF_EN_RXADDR, 0x3F);                        // Enable Pipe0 ʹ������ͨ����������
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RF_CH, 40);                              // Select RF channel 40
     
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RX_PW_P0, RX_PLOAD_WIDTH); 
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RX_PW_P1, RX_PLOAD_WIDTH); 
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RX_PW_P2, RX_PLOAD_WIDTH); 
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RX_PW_P3, RX_PLOAD_WIDTH); 
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RX_PW_P4, RX_PLOAD_WIDTH); 
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RX_PW_P5, RX_PLOAD_WIDTH); 
      
      
    
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RF_SETUP, 0x0f);                         // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
    NRF_Write_Reg(NRF_W_REGISTER + NRF_CONFIG, 0x0f);                           // Set PWR_UP bit, enable CRC(2 bytes) & Prim:RX. RX_DR enabled..   

    NRF_CE = 1;                                                                 // CEΪ��,�������ģʽ   
    
    __delay_us(200);
}

/*
 * �������ݺ���
 * txbuf:�����������׵�ַ
 * ����ֵ:0��������ɣ��������������
 */
unsigned char NRF_RxPacket(unsigned char *rxbuf)
{
	unsigned char status = 0; 

    if(NRF_Get_Status() & RX_DR) {                                        // ���յ�����     ;;;
        status = NRF_Read_RX_Pload(rxbuf);  
        NRF_Write_Reg(NRF_W_REGISTER + NRF_STATUS, 0xff);           // ���TX_DS��MAX_RT�жϱ�־
        NRF_Flush_RX();                                         // ���RX FIFO�Ĵ��� 
        status = status + 1;
    }else {
        status = 0;
    } 
       
	return status;                                                   // û�յ��κ�����
}	

/*
 * Get current pipe#
 * ��ǰ��������ͨ����0-5��6��δʹ�� 7��RX FIFO �Ĵ���Ϊ�� 
 */
unsigned char NRF_Get_Current_Pipenum(void) {
  return ((NRF_Get_Status() & RX_P_NO) >> 1);
}

/**
 * ��ȡ���ܵ�������
 * @param pBuf
 * @return 
 */
unsigned char NRF_Read_RX_Pload(unsigned char *pBuf) {
    unsigned char pipe, width;
    pipe = NRF_Get_Current_Pipenum();
    width = NRF_Read_Reg(NRF_RX_PW_P0 + pipe);
    NRF_Read_Buf(NRF_R_RX_PAYLOAD, pBuf, width);   
    return pipe;
}

/*
 * ��ȡ״̬�Ĵ���
 */
unsigned char NRF_Get_Status(void) {
    return NRF_Read_Reg(NRF_STATUS);
}

/*
 * ��� TX FIFO �Ĵ���
 */
void NRF_Flush_TX(void) {
    NRF_Write_Reg(NRF_FLUSH_TX, 0);
}

/*
 * ��� RX FIFO �Ĵ���
 */
void NRF_Flush_RX(void) {
    NRF_Write_Reg(NRF_FLUSH_RX, 0);
}

unsigned char NRF_Clear_IRQ(void) {
    
    unsigned char IRQ_FLAG;             // �ж�״̬��־λ
    
    unsigned char temp, rx_pw;
    unsigned char* buf;
    
    temp = NRF_Write_Reg(NRF_W_REGISTER + NRF_STATUS, 0x70);    // Read STATUS byte and clear IRQ flag's(nRF24L01)

    if(temp & MAX_RT) IRQ_FLAG = MAX_RT;                      // Indicates max #of retransmit interrupt
    if(temp & TX_DS)  IRQ_FLAG = TX_DS;                       // Indicates TX data succsessfully sent

    if(temp & RX_DR)  // In RX mode, check for data received
    {
        // Data received, so find out which datapipe the data was received on:
//        temp = (0x07 & (temp > 1)); // Shift bits in status byte one bit to LSB and mask 'Data Pipe Number'
//        rx_pw = NRF_Read_Reg(NRF_RX_PW_P0 + temp); // Read current RX_PW_Pn register, where Pn is the pipe the data was received on..
//        NRF_Read_Buf(NRF_R_RX_PAYLOAD, buf, rx_pw); // Data from RX Payload register is now copied to SPI_Buffer[].

        IRQ_FLAG = RX_DR; // Indicates RX data received
    }    
    
    return IRQ_FLAG;
}

/*
 ********************************** �·��� *************************************
 */

/*
 * ����ģʽ
 * NRF_Tx_Set ���������͵�ַ���Ƿ������Զ�Ӧ��
 * 1�����÷��͸���ַ�������Զ�Ӧ���Զ�Ӧ����յ�ַ������Ӧ��ͨ��0
 * 2�������Զ��ط�������Ƶ�ʣ����÷���ģʽ
 * 
 * NRF_Tx_Send �������������ݣ����ݳ��ȣ�
 * 1��CE �øߣ���������
 * 2����֤�Զ�Ӧ������ź�
 */
void NRF_Tx_Set(unsigned char *addr, unsigned char ack) {
    NRF_CE = 0;
    NRF_Write_Buf(NRF_W_REGISTER + NRF_TX_ADDR, addr, 5);       // д���͵�ַ
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P0, addr, 5);    // �Զ�Ӧ��ʱʹ��ͨ��0Ϊ ���յ�ַ���뷢�͵�ַҪһ��
    NRF_Write_Reg(NRF_W_REGISTER + NRF_EN_AA, ack);             // Enable Auto.Ack:Pipe0
    
    NRF_Write_Reg(NRF_W_REGISTER + NRF_EN_RXADDR, 0x01);        // Enable Pipe0
    NRF_Write_Reg(NRF_W_REGISTER + NRF_SETUP_RETR, 0x1a);       // 500?s + 86?s, 10 retrans... ��ʱ�Զ��ط�
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RF_CH, 40);              // Select RF channel 40
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RF_SETUP, 0x0f);         // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
    NRF_Write_Reg(NRF_W_REGISTER + NRF_CONFIG, 0x0e);           // Set PWR_UP bit, enable CRC(2 bytes) & Prim:TX. MAX_RT & TX_DS enabled.. 
}

void NRF_Tx_Send(unsigned char *txbuf, unsigned char len) {
    NRF_CE = 0;
    NRF_Write_Buf(NRF_W_TX_PAYLOAD, txbuf, len);        // д�������ݣ����ݳ��ȣ����ܳ���32���ֽ�
    NRF_CE = 1;
    __delay_us(10);
    while(NRF_Tx_CheckACK());
}

unsigned char NRF_Tx_CheckACK(void) {  
    unsigned char sta = 0;

	sta = NRF_Read_Reg(NRF_STATUS);                 // ��ȡ״̬�Ĵ�����ֵ	 
    if(sta & MAX_RT || sta & TX_DS) {
        NRF_Write_Reg(NRF_W_REGISTER + NRF_STATUS, 0xff);    // ����жϱ�־�����򲻻��ط�����
    }
	if(sta & TX_DS) {                               // ��������ж�,�˳�ѭ�����Ƿ������ TX_FIFO?��
	   return(0);
	}
	return(1);
}

/*
 * ����ģʽ
 * NRF_Rx_Set ���������ý���Ӧ�����ý�������ͨ����
 * 1��������Щͨ���Զ�Ӧ��������Щͨ�������ý��գ�
 * 2��������ͨ��0-2Ϊ�������ݣ������� ack ����Ϊ 0x03
 *    Ȼ����� NRF_Rx_Set_P0(--2) 3������ 
 * {
 * NRF_Rx_Set_P0 ������ͨ��0��ַ���������ݳ��ȣ�
 * NRF_Rx_Set_P1 ������ͨ��1��ַ���������ݳ��ȣ�
 * NRF_Rx_Set_P2 ������ͨ��2��ַ���������ݳ��ȣ�
 * NRF_Rx_Set_P3 ������ͨ��3��ַ���������ݳ��ȣ�
 * NRF_Rx_Set_P4 ������ͨ��4��ַ���������ݳ��ȣ�
 * NRF_Rx_Set_P5 ������ͨ��5��ַ���������ݳ��ȣ�
 * }
 * 
 * NRF_Rx_Receive ����
 * 1��CE �ø� �����ý���
 * 
 * NRF_RxPacket ���������ݻ�������
 * 1����ѭ���ڵ��ô˺��������Ͻ�������
 */
// 
void NRF_Rx_Set(unsigned char ack, unsigned char pipe) {
    NRF_CE = 0;
    NRF_Write_Reg(NRF_W_REGISTER + NRF_EN_AA, ack);                            // Enable Auto.Ack:Pipe0 ʹ������ͨ���Զ�Ӧ��
    NRF_Write_Reg(NRF_W_REGISTER + NRF_EN_RXADDR, pipe);                        // Enable Pipe0 ʹ������ͨ����������
    
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RF_CH, 40);                              // Select RF channel 40   
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RF_SETUP, 0x0f);                         // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
}

void NRF_Rx_Set_P0(unsigned char *addr, unsigned char buf_len) {
    NRF_CE = 0;
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P0, addr, 5);       // Use the same address on the RX device as the TX device
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RX_PW_P0, buf_len);  
}

void NRF_Rx_Set_P1(unsigned char *addr, unsigned char buf_len) {
    NRF_CE = 0;
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P1, addr, 5);       // Use the same address on the RX device as the TX device
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RX_PW_P1, buf_len);  
}

void NRF_Rx_Set_P3(unsigned char *addr, unsigned char buf_len) {
    NRF_CE = 0;
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P3, addr, 5);       // Use the same address on the RX device as the TX device
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RX_PW_P3, buf_len);  
}

void NRF_Rx_Set_P4(unsigned char *addr, unsigned char buf_len) { 
    NRF_CE = 0;
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P4, addr, 5);       // Use the same address on the RX device as the TX device
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RX_PW_P4, buf_len);  
}

void NRF_Rx_Set_P5(unsigned char *addr, unsigned char buf_len) {
    NRF_CE = 0;
    NRF_Write_Buf(NRF_W_REGISTER + NRF_RX_ADDR_P5, addr, 5);       // Use the same address on the RX device as the TX device
    NRF_Write_Reg(NRF_W_REGISTER + NRF_RX_PW_P5, buf_len);  
}

void NRF_Rx_Receive(void) {
    NRF_CE = 0;	  
    NRF_Write_Reg(NRF_W_REGISTER + NRF_CONFIG, 0x0f);                           // Set PWR_UP bit, enable CRC(2 bytes) & Prim:RX. RX_DR enabled..   
    NRF_CE = 1;                                                                 // CEΪ��,�������ģʽ    
    __delay_us(200);
}
