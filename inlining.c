//still working on getting this functions to compile
//into the kernel with inline assembly

uint8_t inb(uint16_t port) {
	 uint8_t out;
	 __asm__ volatile  		//volatile = don't move/optimize this code
	   ( ".intel_syntax noprefix;" //presonal preference
		 "mov dx, [esp + 4];"
		 "in al, dx;"
	   : "=r"(out)            //output vars (none)
	   : "r"(port)		      //input vars
	   : "%eax");             //tell GCC not to use eax since I screw it up
	   return out;
}


void outb(uint16_t port, uint8_t data) {
	 __asm__ volatile  		//volatile = don't move/optimize this code
	   ( ".intel_syntax noprefix;" //presonal preference
		 "mov al, [esp + 8];"
		 "mov dx, [esp + 4];"
		 "out dx, al\n\t;"
	   :                      //output vars (none)
	   : "r"(port)		      //input vars
	   : "%eax", "%edx");             //tell GCC not to use eax since I screw it up
}
