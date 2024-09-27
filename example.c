 
/* lfs for STM32Flash ********************************************************/
lfs_t lfs_Stm32Flash; 
lfs_file_t file_Stm32Flash;
 
// configuration of the filesystem is provided by this struct
const struct lfs_config cfg_Stm32Flash = {
    // block device operations
    .read  = stm32flash_readLittlefs,
    .prog  = stm32flash_writeLittlefs,
    .erase = stm32flash_eraseLittlefs,
    .sync  = stm32flash_syncLittlefs,
 
    // block device configuration
    .read_size = 128,
    .prog_size = 128,
    .block_size = STM32Flash_ERASE_GRAN,
    .block_count = STM32Flash_NUM_GRAN/2,
    .cache_size = 512,
    .lookahead_size = 512,
    .block_cycles = 500,
};
 
 
 
extern void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);  
 
 
int main(void)
{
	int err = -1;
	
	
	
	delay_init();	    	 //ÑÓÊ±º¯Êý³õÊ¼»¯	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//ÉèÖÃÖÐ¶ÏÓÅÏÈ¼¶·Ö×éÎª×é2£º2Î»ÇÀÕ¼ÓÅÏÈ¼¶£¬2Î»ÏìÓ¦ÓÅÏÈ¼¶
	uart_init(115200);	 	//´®¿Ú³õÊ¼»¯Îª115200
	LED_Init();		  		//³õÊ¼»¯ÓëLEDÁ¬½ÓµÄÓ²¼þ½Ó¿Ú
	KEY_Init();				//°´¼ü³õÊ¼»¯		 	 	
	W25QXX_Init();			//W25QXX³õÊ¼»¯
    
	
	
	
 
	while(W25QXX_ReadID()!=W25Q128)								//¼ì²â²»µ½W25Q128
	{
		printf("W25Q128 Check Failed!\r\n");
		delay_ms(500);
		printf("Please Check!        \r\n");
		delay_ms(500);
		LED0=!LED0;//DS0ÉÁË¸
	}
	printf("W25Q128 Ready!\r\n");  
	
	
	
	
 
//    err =  lfs_mount(&lfs, &cfg);
//	
//	if(err )
//	{
//		lfs_format(&lfs, &cfg);
//        lfs_mount(&lfs, &cfg);
//	}
//	
	
	err = lfs_mount(&lfs_Stm32Flash, &cfg_Stm32Flash);
	
	if( err )
	{
		lfs_format(&lfs_Stm32Flash, &cfg_Stm32Flash);
		lfs_mount(&lfs_Stm32Flash, &cfg_Stm32Flash);
	}
	
	
	
	
	
	while(1)
	{
		LED0=!LED0;
 
		
		uint32_t boot_count = 0;
		lfs_file_open(&lfs_Stm32Flash, &file_Stm32Flash, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
		lfs_file_read(&lfs_Stm32Flash, &file_Stm32Flash, &boot_count, sizeof(boot_count));
 
		
		// update boot count
		boot_count += 1;
		lfs_file_rewind(&lfs_Stm32Flash, &file_Stm32Flash);  // seek the file to begin
		lfs_file_write(&lfs_Stm32Flash, &file_Stm32Flash, &boot_count, sizeof(boot_count));
 
		// remember the storage is not updated until the file is closed successfully
		lfs_file_close(&lfs_Stm32Flash, &file_Stm32Flash);
 
//		// release any resources we were using
//		lfs_unmount(&lfs);
 
		// print the boot count
		printf("boot_count: %d\n", boot_count);
 
		delay_ms(20000);
	}
}
