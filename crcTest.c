#include <stdio.h>
#include <stdint.h>

uint16_t crc_init;
uint16_t Value;
uint8_t Dt[6] = {0x58 /*(Group)*/, 0x14 /*HDR*/, 0x03/*length*/, /*(payload)*/0x23, 0x17, 0x26};
int cont;
uint8_t lcrc;
uint8_t rcrc;

// Function to calculate crc-16 update, in this case the CRC-16 is the classical IBM reverse polynomial 
// 0xA001
uint16_t crc16_update(uint16_t crc, uint8_t a)
{
  int i;
  crc ^= a;
  for(i = 0; i < 8; ++i)
  {
    if(crc & 1)
      crc = (crc >> 1) ^ 0xA001;
    else
      crc = (crc >> 1);
  }
  return crc;
}


int main()
{
  crc_init = ~0;
  for(cont = 0; cont < 6; cont++)
  {
    crc_init = crc16_update(crc_init, Dt[cont]);
  }
  printf("Init value of crc = %04X \n", crc_init);
  // To get a final value of cero, we need the init_crc = ~0 and pass
  // the frame from group (0x58) to the final payload. The final
  // CRC was 0xA442 and the reverse is first use 0x42 and then 0xA4
  lcrc = (uint8_t)crc_init;
  printf("Init value of lcrc = %02X \n", lcrc);
  rcrc = (crc_init >> 8);
  printf("Init value of rcrc = %02X \n", rcrc);
  crc_init = crc16_update(crc_init, lcrc);
  crc_init = crc16_update(crc_init, rcrc);
  // The final CRC must be 0x0000
  printf("Final crc = %04X \n", crc_init);
  return 0;
  //(..., lcrc, rcrc) is the final values in CRC-16 place
}
