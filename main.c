#include <16f877a.h>
#include<string.h>
#fuses hs, nowdt, noprotect, nolvp
#use delay(clock=20m)
//////////////////////////////////////////////////////////////////I2C LCD
#use i2c(Master,Fast=100000, sda=PIN_D6, scl=PIN_D7,force_sw)
#include <i2c_Flex_LCD.c> 
//////////////////////////////////////////////////////////////DEFINE KEYPAD LIBRARY
#DEFINE USE_PORTB_KBD 
#INCLUDE <KBD4x4_1.C>
#define KEYHIT_DELAY 200
//////////////////////////////////////////////////////////////SETUP SPI MEM FRC522
#define MFRC522_CS PIN_C6
#define MFRC522_SCK PIN_C5
#define MFRC522_SI PIN_C4
#define MFRC522_SO PIN_D3
#define MFRC522_RST PIN_D2
#include<Built_in.h>
/////////////////////////////////////////////////////////////////DEFINE PIN
#define add_card_pin pin_d1
#define delete_card_pin pin_d0
/////////////////////////////////////////////////////////////////KHAI BAO BIEN;
int1 tt1=0,tt2=0, tt4=0, tt5=0, tt3=0;
int1 ttthe=0;
int1 tam0=0,tam1=0;
int8 k=0, vt=0, buzzer=0;
UNSIGNED int TagType; 
CHAR UID[5];
char str[5];
char str2[5], str3[5];
//!char str3[5];  
char testt[]={0x17, 0x37, 0x39, 0x3d, 0xcc};
char test1[]={0xca, 0xa6, 0xc7, 0x3a, 0x91}; 
char master[]={0x4a, 0xd8, 0xd1, 0x80, 0xc3}; 
char delete[]={"0000"};
/////////////////////////////////////////////////////////////////
CHAR h;
int16 pass=9999;
int8 save=0,buzzer1;
int8 val=0,ngan=0,tram=0,chuc=0,dv=0;
int1 lock=0, ok=0;
////////////////////////////
//!int8 time=0,bdt=0;
/////////////////////////////////////////////WARNING
void bao_dong(int8 hoi, int8 rong_am)
{
int8 i, j;
for(i=0;i<hoi;i++)
 {
  for(j=0;j<rong_am;j++)
   {
   output_high(pin_e1);
   delay_ms(30);
   }
   output_low(pin_e1);
   delay_ms(30);
 }
}
//!////////////////////////////////////////////////////////////////////////////////////ADDITIONAL PROGRAMS RFID
void eeprom_write_string(unsigned int8 addr, unsigned char* str)
{
while(*str)
 {
 write_eeprom(addr,*str);
 addr++;
 str++;
 }
}
void eeprom_read_string(unsigned int8 addr,unsigned char *str,unsigned int8 len)
{
for(int8 i=0;i<len;i++)
 {
 str[i]=read_eeprom(addr++);
 }
 str[len]=0;
}
void clear_string()
{
for(int8 u=0;u<6;u++)
 {
 uid[u]='\0';
 str[u]='\0';
 str2[u]='\0';
 str3[u]='\0';
 }
}
void add_card()
{
if(!input(add_card_pin))
 {
 delay_ms(20);
  if(!input(add_card_pin))
   {
 lcd_clear();
 bao_dong(1,3);
 lcd_gotoxy(0,1);
 lcd_putc("MOI QUET THE:");
 k=1;
 tam1=0;
   }
   while(!input(add_card_pin));
 }
}
void delete_card()
{
if(!input(delete_card_pin))
 {
 delay_ms(20);
  if(!input(delete_card_pin))
   {
 bao_dong(1,3);
 lcd_clear();
 lcd_gotoxy(0,1);
 lcd_putc("MOI QUET THE:");
 k=2;
 tam1=0;
   }
   while(!input(delete_card_pin));
 }
}
void read_data()
{
 eeprom_read_string(0,str2,5); 
 eeprom_read_string(6,str3,5);
 }
int check_the(char uidd[],char data[])
 {
 if(strncmp(uidd,data,5)==0)
 {
 ttthe=1;
 }
    else
      {
         ttthe=0; break;
      }
 return ttthe;
 }
void read_card()
{
IF (MFRC522_isCard (&TagType)) //Check any card
      {                                           
       IF (MFRC522_ReadCardSerial (&UID))
       {
    ///////////////////////////////////////////////////////////////CHECK_CARD
    if(k!=2)
    {      
           if(check_the(uid,test1)) tt1=1;
            else if(check_the(uid,master)) tt2=1;
             else if(check_the(uid,str2)) tt4=1;
              else if(check_the(uid,str3)) tt5=1;    
           else
           {
          if(k!=1)
          {
           IF(Buzzer==2)
         {
         buzzer=0;
         tt3=1; break;
         }
         ELSE
         {
         buzzer++;
         bao_dong(5,2);
         LCD_CLEAR();
         LCD_GOTOXY ( 3,1 ) ;
         LCD_PUTC ( "UID ERROR!!!!" ) ; delay_ms(500); 
         LCD_CLEAR();
         LCD_GOTOXY ( 2,1 ) ;
         LCD_PUTC ( "VUI LONG THAO" );
         LCD_GOTOXY ( 4,2) ;
         LCD_PUTC ( "TAC LAI!!!" ); DELAY_MS(1000);
         lcd_clear();
         }
           }
           }
    }
    ///////////////////////////////////////////////////////////////ADD_CARD
    if(k==1)
     {
      tam0=0;
      tam1=0;
      k=0;
      bao_dong(1,3);
      vt++; 
      if(vt>=2) vt=0;
        switch(vt)
        {
       case 0:  eeprom_write_string(0,uid);break;
       case 1:  eeprom_write_string(6,uid);break;
        }
    
    delay_ms(200);
    bao_dong(1,3);
    lcd_putc("\fDONE...");
    DELAY_MS(500);
    }
    //////////////////////////////////////////////////////////////DELETE_CARD
    if(k==2)
    { 
      clear_string();
      tam0=0;
      tam1=0;
      k=0;
      bao_dong(1,3);
      IF(check_the(uid,str2)) eeprom_write_string(0,delete);
      if(check_the(uid,str3)) eeprom_write_string(6,delete);
 
      bao_dong(1,3);
      lcd_putc("\fDONE...");
      DELAY_MS(500);
      tam0=0;                                              
      tam1=0;
      k=0;
    }
    /////////////////////////////
    lcd_clear();
        }
   mfrc522_halt();
   }
   }
///////////////////////////////////////////////////////////////////////////////////////ADDITIONAL PROGRAMS KEYPAD
char key_time (  )
{
char c = '\0' ;
unsigned int16 timeout;
timeout = 0;
//!c = kbd_getc () ;
while ( c =='\0' && ( ++timeout<(KEYHIT_DELAY*100 ))) 
//!while ( c =='\0') 
{
delay_us ( 10 ) ;
c = kbd_getc () ; // Capture keyboard value
}
return ( c ) ;
}
void key_pad()
{
h=key_time();
while(h=='\0')
 {
 h=key_time();
 }
 if((h!='\0'))
  {
  if(h=='0') val=0;
  else if(h=='1') val=1;
  else if(h=='2') val=2;
  else if(h=='3') val=3;
  else if(h=='4') val=4;
  else if(h=='5') val=5;
  else if(h=='6') val=6;
  else if(h=='7') val=7;
  else if(h=='8') val=8;
  else if(h=='9') val=9;
  else val=0;
  }
}
void key_pad_re()
{
key_pad();
if((h!='#')&&(h!='*'))
 {
 printf(lcd_putc,"*");
 bao_dong(1,3);
 ngan=val;
 }
 if((h=='*')||(h=='#')) return;
 ////////////////////////////////////
 key_pad();
 if((h!='#')&&(h!='*'))
 {
 printf(lcd_putc,"*");
 bao_dong(1,3);
 tram=val;
 }
 if((h=='*')||(h=='#')) return;
 ///////////////////////////////////
 key_pad();
 if((h!='#')&&(h!='*'))
 {
 printf(lcd_putc,"*");
 bao_dong(1,3);
 chuc=val;
 }
 if((h=='*')||(h=='#')) return;
 ///////////////////////////////////
 key_pad();
 if((h!='#')&&(h!='*'))
 {
 printf(lcd_putc,"*");
 bao_dong(1,3);
 dv=val;
 }
 if((h=='*')||(h=='#')) return;
}
int1 val_key()
{
int16 key=0, m=0,c=0,d=0,u=0;
m=ngan*1000;
c=(int16)tram*100;
d=chuc*10;
u=dv;
key=m+c+d+u;
ngan=0;tram=0;chuc=0;dv=0;
if(key==pass) return(1);
else return(2);
}
void key_change()
{
int1 mode=0;
lock=1;
bao_dong(1,3);
lcd_clear();
LCD_GOTOXY (1,1);
LCD_PUTC ("NHAP PASS CU:");
LCD_GOTOXY (1,2) ;
key_pad_re();
while(h!='*')
{
key_pad();
}
mode=val_key();
 if(mode==1)
  {
bao_dong(1,3);
lcd_clear();
LCD_GOTOXY (1,1);
LCD_PUTC ("NEW PASS:") ;
LCD_GOTOXY (1,2);
key_pad_re();
while(h!='*')
{
key_pad();
}
bao_dong(2,3);
int16 m=0,c=0,d=0,u=0, key=0;
m=ngan*1000;
c=(int16)tram*100;
d=chuc*10;
u=dv;
key=m+c+d+u;
//////////////////
pass=key;
WRITE_EEPROM ( 7,ngan ) ;
WRITE_EEPROM ( 8,tram ) ;
WRITE_EEPROM ( 9,chuc ) ;
WRITE_EEPROM ( 10,dv ) ;
WRITE_EEPROM ( 11,12 );
ngan=0;tram=0;chuc=0;dv=0;
  }
  else
  {
lcd_clear();
LCD_GOTOXY ( 1,1 ) ;
LCD_PUTC ( "WRONG PASSWORD.." );
bao_dong(25,5);
  }
}
void read_eeprom_key()
{
int8 m1=0,t1=0,c1=0,dv1=0;
int16 m0=0,t0=0,c0=0,dv0=0;
save=read_eeprom(11);
if(save!=12) pass=9999;
else
{
m1=read_eeprom(7);
t1=read_eeprom(8);
c1=read_eeprom(9);
dv1=read_eeprom(10);
//!save=read_eeprom(11);
m0=m1*1000;
t0=(int16)t1*100;
c0=c1*10;
dv0=dv1;
pass=m0+t0+c0+dv0;
delay_ms(50);
LCD_GOTOXY ( 0,1 ) ;
LCD_PUTC ( "\fDONE!!!" );
delay_ms(500);
}
}
//////////////////////////////////////////////////////////////////MAIN PROGRAMS

void main()
{
vt=0;
tt1=0,tt2=0, tt4=0, tt5=0;
lcd_init(0x4E,16,2);

mfrc522_init();
lcd_clear();
clear_string();
///////////////////////////////
port_b_pullups ( 0xff ) ;
SET_TRIS_B (0xff) ;  
lcd_init(0x4E,16,2);
KBD_INIT ();
lcd_clear();
read_eeprom_key();
//////////////////////////////
//!setup_timer_1(t1_internal|t1_div_by_8);
//!enable_interrupts(global);
//!set_timer1(3036);
//!enable_interrupts(int_timer1);
//!///////////////////////////////////////////////VONG LAP VO HAN WHILE
 while(true)
 {
h=key_time();
if(h!='D' && k!=1 && k!=2)
{
LCD_GOTOXY ( 0,1 ) ;
LCD_PUTC ( "\fNHAN PHIM D HOAC" ) ;
LCD_GOTOXY ( 4,2 ) ;
LCD_PUTC ( "SCAN UID:" );
}
if(h=='D')
{
lock=0;
bao_dong(1,3);
LCD_CLEAR();
LCD_GOTOXY ( 0,1 ) ;
LCD_PUTC ( "NHAP PASS:" ) ;
LCD_GOTOXY ( 0,2 ) ;
key_pad_re();
if(h=='#') key_change();
else
{
while((h!='*'))
{
key_pad();
if(h=='#') key_change();
}
}

if(val_key()==1)
  {
ok=1;
val_key()==0;
buzzer1=0;
 }
else if(val_key()==2)
{
val_key()==0;
if(buzzer1==2)
{
buzzer1=0;
LCD_GOTOXY ( 0,1 ) ;
LCD_PUTC ( "\fCANH BAO TRY CAP" );
LCD_GOTOXY ( 2,2 ) ;
LCD_PUTC ( "SAI NHIEU LAN" );
bao_dong(25,5);
lcd_clear();
}
else
{
buzzer1++;
if(lock!=1)
{
LCD_CLEAR();
LCD_GOTOXY ( 2,1 ) ;
bao_dong(5,2);
LCD_PUTC ( "MAT KHAU SAI." ); DELAY_MS(1000);
LCD_CLEAR();
LCD_GOTOXY ( 2,1 ) ;
LCD_PUTC ( "VUI LONG THAO" );
LCD_GOTOXY ( 4,2) ;
LCD_PUTC ( "TAC LAI!!!" ); DELAY_MS(1000);
}
}
}
}
   /////////////////////////////////////
else
   {
 read_data();
 read_card();
 if(tt4==1||tt5==1||tt1==1)
   {
  ok=1;
  tt4=0;
  tt5=0;
  tt1=0;
  buzzer=0;
  lcd_clear();
  }
 if(tt3==1)
 {
LCD_GOTOXY ( 0,1 ) ;
LCD_PUTC ( "\fCANH BAO TRY CAP" );
LCD_GOTOXY ( 2,2 ) ;
LCD_PUTC ( "SAI NHIEU LAN" );
bao_dong(25,5);
  tt3=0;
  lcd_clear();
 }
 if(tt2==1)
  {
 buzzer=0;
 tt2=0;
 do
 {
 tam1=1;
 tam0=1;
 bao_dong(1,3);
 lcd_gotoxy(0,1);
 lcd_putc("MOI CHON MODE:");
 lcd_gotoxy(0,2);
 lcd_putc("1.ADD   2.DELETE");
 }
 while(tam0==0);
  while(tam1==1)
    {
    add_card();
    delete_card();
   }
   }
  }
  ////////////////////////DK MO CUA.
if(ok==1)
{
  ok=0;
  LCD_CLEAR();
  LCD_GOTOXY ( 2,1 ) ;
  LCD_PUTC ( "XAC NHAN INFO" );
  LCD_GOTOXY ( 4,2 ) ;
  LCD_PUTC ( "CHINH XAC" );
  DELAY_MS(1000);
  bao_dong(1,4);
  LCD_CLEAR();
  LCD_GOTOXY ( 4,1 ) ;
  LCD_PUTC ( "CUA DA MO!!!" );
  LCD_GOTOXY ( 5,2 ) ;
  LCD_PUTC ( "MOI VAO..." );
  output_high(pin_e0);
   delay_ms(2000);
  output_low(pin_e0);
}
}
}


