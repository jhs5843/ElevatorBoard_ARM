#define LCD_PORT GPIOB
#define LCD_RS_Pin GPIO_Pin_8
#define LCD_RW_Pin GPIO_Pin_9
#define LCD_E_Pin GPIO_Pin_10
#define LCD_DATA4_Pin GPIO_Pin_12
#define LCD_DATA5_Pin GPIO_Pin_13
#define LCD_DATA6_Pin GPIO_Pin_14
#define LCD_DATA7_Pin GPIO_Pin_15

void lcd_busycheck(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Pin = LCD_E_Pin | LCD_RS_Pin | LCD_RW_Pin;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;

  GPIO_Init(LCD_PORT, & GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = LCD_DATA4_Pin | LCD_DATA5_Pin | LCD_DATA6_Pin | LCD_DATA7_Pin;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(LCD_PORT, & GPIO_InitStruct);
  LCD_PORT -> ODR &= ~(LCD_RS_Pin | LCD_DATA4_Pin | LCD_DATA5_Pin | LCD_DATA6_Pin | LCD_DATA7_Pin);
  LCD_PORT -> ODR |= (LCD_RW_Pin | LCD_E_Pin);
  while (GPIO_ReadInputDataBit(LCD_PORT, LCD_DATA7_Pin));
  LCD_PORT -> ODR &= ~LCD_E_Pin;
  GPIO_InitStruct.GPIO_Pin = LCD_E_Pin | LCD_DATA4_Pin | LCD_DATA5_Pin | LCD_DATA6_Pin | LCD_DATA7_Pin | LCD_RS_Pin | LCD_RW_Pin;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;

  GPIO_Init(LCD_PORT, & GPIO_InitStruct);
}

void lcd_delay_us(u32 nCount) {
 int i,tmp;
 for(; nCount !=0; nCount--)
    {
       for(i=0; i<12; i++) tmp += i;
    }
}

void lcd_delay_ms(u32 nCount)
{
	//delay(nCount);
   for(; nCount !=0; nCount--)
      lcd_delay_us(1000);
}

void lcd_command(unsigned char command); // write a command(instruction) to text
void lcd_data(unsigned char data); // display a character on text LCD
void lcd_init(void); // initialize text LCD module
void lcd_string(unsigned char command, char *string);

//CLCD Control Port set
#define CLCD_PORT GPIOB //CLCD CONTROL PORT SET
#define Rs(x) CLCD_PORT->ODR=((CLCD_PORT->ODR&~(GPIO_Pin_8))|(x<<8)) //CLCD Rs : PE8
#define E(x) CLCD_PORT->ODR=((CLCD_PORT->ODR&~(GPIO_Pin_10))|(x<<10)) //CLCD E : PE10
#define CLCD_data(x) CLCD_PORT->ODR=((CLCD_PORT->ODR&(~(GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15)))|(x<<12 )) //CLCD Data bus : PE12~PE15

void lcd_init(void) { // initialize text LCD module (4BIT)
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.GPIO_Pin = LCD_E_Pin | LCD_RS_Pin | LCD_RW_Pin | LCD_DATA4_Pin | LCD_DATA5_Pin | LCD_DATA6_Pin | LCD_DATA7_Pin;
	  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;
	  GPIO_Init(LCD_PORT, & GPIO_InitStruct);
	  lcd_command(0x20);
	  lcd_delay_ms(30);
	  lcd_command(0x20);
	  lcd_delay_ms(30);
	  lcd_command(0x20);
	  lcd_delay_ms(30);
	  lcd_command(0x28);
	  lcd_command(0x08);
	  lcd_command(0x01);
	  lcd_command(0x06);
	  lcd_delay_ms(10);
	  lcd_command(0x0C);

	  /*
   int Cont;
   GPIO_InitTypeDef GPIO_PORTE; //GPIO 설정관련 구조체 선언
   GPIO_PORTE.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_PORTE.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_PORTE.GPIO_OType = GPIO_OType_PP;
   GPIO_PORTE.GPIO_Pin = GPIO_Pin_All;
   GPIO_Init(CLCD_PORT, &GPIO_PORTE);  //PORTE 설정 메모리번지를 구조체 포인터 지시
   lcd_delay_ms(30); //30ms
   E(0), Rs(0); CLCD_data(0x30); //Functon set
   lcd_delay_ms(6); //4.1ms
   E(1);
   for(Cont=0;Cont<2;Cont++) { //Functon set loop
      E(0);
      lcd_delay_us(100); // 100us
      E(1);
   }
   lcd_delay_us(50); //50

   CLCD_data(0x20);
   E(1);
   lcd_delay_us(150); //150
   E(0);
   lcd_delay_us(50); //50
   lcd_command(0x28); // function set(4 bit, 2 line, 5x7 dot)
   lcd_command(0x0C); // display control(display ON, cursor OFF)
   lcd_command(0x06); // entry mode set(increment, not shift)
   lcd_command(0x01); // clear display
   lcd_delay_ms(50); //4*/
}

void lcd_command(unsigned char command) { // write a command(instruction) to text

	  // lcd_busycheck();
//상위 4BIT 전송
   Rs(0),E(0);
   lcd_delay_us(1); //tas : Min 40ns
   CLCD_data(command>>4);
   E(1);
   lcd_delay_us(1);   //PW_eh min 230ns
   E(0);
   lcd_delay_us(50);   //100
//  하위 4BIT 전송
   CLCD_data((command&0x0F)); // output command
   E(1); lcd_delay_us(1); //1
   E(0); lcd_delay_us(50); //50
}
void lcd_data(unsigned char data) { // display a character on text LCD //상위 4BIT 전송

	  // lcd_busycheck();
   E(0), Rs(1); CLCD_data(data>>4); // output data
   E(1); lcd_delay_us(1); // 1
   E(0); lcd_delay_us(50); //50
//하위 4BIT 전송
   CLCD_data((data&0x0F));
   E(1); lcd_delay_us(1);  //   1
   E(0); lcd_delay_us(50); //50
   lcd_delay_ms(4); //4
}

void lcd_string(unsigned char command, char *string)  { // display a string on LCD
   lcd_command(command); // start position of string
   while(*string != '\0') { // display string
      lcd_data(*string); string++;
   }
}

void lcd_gotoxy(int x, int y)
{
   lcd_command(0x80 | (0x40 * y) | x);

}

void lcd_clear()
{
	lcd_command(0x01); // clear display
	lcd_delay_ms(4); //4

}

void lcd_printxy(int x, int y, char *string)  { // display a string on LCD
   lcd_gotoxy(x, y);
   while(*string != '\0') { // display string
      lcd_data(*string);
      string++;
   }
}
