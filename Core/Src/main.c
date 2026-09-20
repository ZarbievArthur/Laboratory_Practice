#include <stdint.h>
int main(void) {
    *(uint32_t*)(0x40023800UL + 0x30UL) |= 0x06; //Power on B and C ports
    *(uint32_t*)(0x40020400UL + 0x00UL) |= 0x4000; // Output Mode 
    *(uint32_t*)(0x40020400UL + 0x04UL) |= 0x00; // Push-pull register
    *(uint32_t*)(0x40020400UL + 0x08UL) |= 0x4000;  // Message's speed
    *(uint32_t*)(0x40020400UL + 0x0CUL) |= 0x00; // No pull-up, pull-down
 
    *(uint32_t*)(0x40020800UL + 0x00UL) |= 0x00;   // Need setting Input/Output mode
    *(uint32_t*)(0x40020800UL + 0x04UL) |= 0x00;  // push-pull register
    *(uint32_t*)(0x40020800UL + 0x08UL) |= 0x08000000;    // Message's speed
    *(uint32_t*)(0x40020800UL + 0x0CUL) |= 0x00;    // Pull-up or pull-down

    while(1){
        if((*(uint32_t*)(0x40020800UL + 0x10UL) & 0x2000UL) != 0){
            *(uint32_t*)(0x40020400UL + 0x18UL) = 0x80; 
        }
        else{
            *(uint32_t*)(0x40020400UL + 0x18UL) = 0x00800000; 
        }
    }
}