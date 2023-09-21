#define TAPC_IDCODE	(uint8_t*) 0x31130000  /* Silicon Revision */ 

#define BITM_SI_REVID	0xF0000000 /* Revision ID */
#define BITM_JTAGID	0x0FFFF000 /* JTAG ID */
#define BITM_MNFID	0x00000FFE /* Manufacturer ID */
#define BITM_LSB	0x00000001 /* IDCODE LSB */

#define ADI_MNFID	0x65
#define ADI_LSB		0x01

#define GRFN_SC58X_JTAGID   0x2808	// Device ID
#define GRFN_SC58X_SI_REVID_0_0    0x00    // Rev 0.0 silicon for SC58X     (IDCODE value is 0x028080CB)
#define GRFN_SC58X_SI_REVID_0_1    0x01    // Rev 0.1 silicon for SC58X     (IDCODE value is 0x128080CB)
#define GRFN_SC58X_SI_REVID_1_0    0x02    // Rev 1.0 silicon for SC58X     (IDCODE value is 0x228080CB)
#define GRFN_SC58X_SI_REVID_1_2    0x04    // Rev 1.2 silicon for SC58X     (IDCODE value is 0x428080CB)

#define GRFN_SC59X_JTAGID	0x280B	// Device ID
#define GRFN_SC59X_SI_REVID_0_0	0x00 	// Rev 0.0 Silicon for SC59X

enum adsp_family { 
       unknown,	
	sc58x,  
	sc59x
};
