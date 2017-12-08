/*tasks:
	Criar o header+função externa para as funções da eeprom
	Imprementar defines das constantes que estao no codigo do livro
	Rever codigo de outros exerccios ate agora feitos para estarem realmente reutilizaveis
	Github push
	Aplicar a formatação das bibliotecas da ST
	Tratar dos warnings todos (tb de tutoriais anteriores)
*/

#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <spi.h>

uint8_t txbuf[4], rxbuf[4];
uint16_t txbuf16[4], rxbuf16[4];

//-----------------------depois passar todo codigo relativo a eeprom para uma função externa
enum eepromCMD { cmdREAD = 0x03, cmdWRITE = 0x02,
		cmdWREN = 0x06, cmdWRDI = 0x04,
		cmdRDSR = 0x05, cmdWRSR = 0x01 };
#define EEPROM_PORT GPIOC
#define EEPROM_CS GPIO_Pin_3
#define EEPROM_SPEED SPI_SLOW
#define EEPROM_SPI SPI2
#define WIP(x) ((x) & 1)

void eepromInit();	//prototipo dado
void eepromWriteEnable();	//prototipo dado
void eepromWriteDisable();	//prototipo dado
uint8_t eepromReadStatus();	//prototipo dado
void eepromWriteStatus(uint8_t status);	//prototipo dado
int eepromWrite(uint8_t *buf, uint8_t cnt, uint16_t offset);	//prototipo dado
int eepromRead(uint8_t *buf, uint8_t cnt, uint16_t offset);	//prototipo dado

//Timer code
void Delay(uint32_t nTime);

void main()
{
	int i, j, StatusRegister=0, aux=0;
	//csInit();
	GPIO_InitTypeDef GPIO_InitStructure;
	//Enable Peripheral Clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//Configure Pins
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//LEDInit(); optional for this tutorial
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	spiInit(SPI2);

	//Configure SysTick Timer
	if (SysTick_Config(SystemCoreClock/1000)) while(1);

	//Program execution-------------------------------------------------------
uint8_t txbufa[16]={0x0F,0x0E,0x0D,0x0C,0x0B,0x0A,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00};
uint8_t rxbufa[16]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

	eepromWrite(&txbufa, 16, 0x0020);

	Delay(1000); //monte de tempo

	eepromRead(&rxbufa, 1, 0x0020); //500==0x01F4

	if((rxbufa[0]==0x0F)){	//End of execution indicator
		GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_SET);
	}else{
		GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_RESET);
	}


}

//Assert code
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
/*Infinite loop*/
/*Use GDB to find out why we're here*/
}
#endif

//Timer code
static __IO uint32_t TimingDelay;

void Delay(uint32_t nTime){
	TimingDelay = nTime;
	while(TimingDelay != 0);
}

void SysTick_Handler(void){
	if(TimingDelay !=0x00)
	TimingDelay--;
}



//-----------------------depois passar todo codigo relativo a eeprom para uma função externa
/**
  * @}
  */
uint8_t eepromReadStatus() {
	uint8_t cmd[] = {cmdRDSR , 0xff};
	uint8_t res[2];
	GPIO_WriteBit(EEPROM_PORT , EEPROM_CS , 0);
	spiReadWrite(EEPROM_SPI , res, cmd, 2, EEPROM_SPEED);
	GPIO_WriteBit(EEPROM_PORT , EEPROM_CS , 1);
	return res[1];
}

/**
  * @}
  */
void eepromWriteEnable(){
	uint8_t cmd = cmdWREN;
	while (WIP(eepromReadStatus()));	//So se pode escrever quando a flag WIP do estatus register for 0
	GPIO_WriteBit(EEPROM_PORT , EEPROM_CS , 0);
	spiReadWrite(EEPROM_SPI , 0, &cmd, 1, EEPROM_SPEED);
	GPIO_WriteBit(EEPROM_PORT , EEPROM_CS , 1);
}

/**
  * @brief  Writes on eeprom memory
  * @note   For now writes only one byte without end of page measures.
  * @param  *buf: ponteiro para vector de dados a escrever.
  * @param  cnt: nº de bytes a escrever (tamanho de buf[]). 
  * @param  offset: adress da eprom a escrever.   
  * @retval ? 
  */
int eepromWrite(uint8_t *buf, uint8_t cnt, uint16_t offset){

	uint8_t cmd[19] = {cmdWRITE , (offset>>8), (offset&0x00FF), *buf};
	//page: [xxxx xxxx xxxx 0000; xxxx xxxx xxxx 1111] => implementar algum metodo de segurança para 		verificar fim de cada pagina ou passar automaticamente para a proxima pagina

memcpy(&cmd[3],buf,16); //usar cnt!!!
	
	eepromWriteEnable();
	GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 0);
	spiReadWrite(SPI2 , 0, cmd, (3+cnt), SPI_SLOW);
	GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 1);
	return 0;
}

/**
  * @}
  */
int eepromRead(uint8_t *buf, uint8_t cnt, uint16_t offset){  // o read nao tem aquele limite de 16 bites??
	uint8_t cmd[] = {cmdREAD, (offset>>8), (offset&0x00FF)};	
	//uint8_t res[(3+cnt)];
	uint8_t res[]={0,0,0,0};

cnt=1;

	GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 0);
	spiReadWrite(SPI2 , res, cmd, (3+cnt), SPI_SLOW);  //podia-se mandar directamente buf para aqui sem criar o res?
	GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 1);
*buf=res[3];	
//buf=&res[3];
	return 0;
}

/*
void eepromInit(){ 
	//??basicamente inicializar o SPI?
}

void eepromWriteDisable(){ 
	//basear-se na função spiwrite enable
}

void eepromWriteStatus(uint8_t status){
}
*/



/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

