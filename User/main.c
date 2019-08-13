/*
*********************************************************************************************************
*
*	ģ������ : ������ģ��
*	�ļ����� : main.c
*	��    �� : V1.0
*	˵    �� : minilzoѹ���㷨����ֲ
*              ʵ��Ŀ�ģ�
*                1. ѧϰminilzoѹ���㷨����ֲ��
*              ʵ�����ݣ�
*                1. ����ѹ���ͽ�ѹ���ԣ������س������¸������ϵ磬�ϵ���Զ���ӡ��
*              ע�����
*                1. ��ʵ���Ƽ�ʹ�ô������SecureCRT�鿴��ӡ��Ϣ��������115200������λ8����żУ��λ�ޣ�ֹͣλ1��
*                2. ��ؽ��༭��������������TAB����Ϊ4���Ķ����ļ���Ҫ��������ʾ�����롣
*
*	�޸ļ�¼ :
*		�汾��   ����         ����        ˵��
*		V1.0    2019-08-12   Eric2013     1. CMSIS����汾 V5.6.0
*                                         2. HAL��汾 V1.3.0
*
*	Copyright (C), 2018-2030, ���������� www.armfly.com
*
*********************************************************************************************************
*/	
#include "bsp.h"			/* �ײ�Ӳ������ */
#include "minilzo.h"


/* ���������������̷������� */
#define EXAMPLE_NAME	"V7-MiniLZO����ѹ���㷨��ֲ��ʹ��"
#define EXAMPLE_DATE	"2019-08-12"
#define DEMO_VER		"1.0"


static void PrintfLogo(void);
static void PrintfHelp(void);
static uint8_t TestMiniLZO(uint32_t _Len);


/* ��������õ������������ʹ�С */
#define IN_LEN      (20*1024ul)
#define OUT_LEN     (IN_LEN + IN_LEN / 16 + 64 + 3)
static unsigned char __LZO_MMODEL in  [ IN_LEN ];
static unsigned char __LZO_MMODEL out [ OUT_LEN ];

/* ѹ����Ҫ�ڴ��С */
#define HEAP_ALLOC(var,size) \
    lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]

static HEAP_ALLOC(wrkmem, LZO1X_1_MEM_COMPRESS);
	
/* ��������������⾯�� */
#if defined ( __CC_ARM )
__attribute__((section (".RAM_D1"))) uint8_t  InTempData[IN_LEN];
__attribute__((section (".RAM_D1"))) uint16_t TempValues1;
__attribute__((section (".RAM_D2"))) uint16_t TempValues2;
__attribute__((section (".RAM_D3"))) uint16_t TempValues3;
#endif

/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: c�������
*	��    ��: ��
*	�� �� ֵ: �������(���账��)
*********************************************************************************************************
*/
int main(void)
{
	uint8_t ucKeyCode;		/* �������� */
	
#if defined ( __CC_ARM )	
	TempValues1 = 0; /* ����MDK���� */  
	TempValues2 = 0;
	TempValues3 = 0;		
#endif

	
	bsp_Init();		/* Ӳ����ʼ�� */
	
	PrintfLogo();	/* ��ӡ�������ƺͰ汾����Ϣ */
	PrintfHelp();	/* ��ӡ������ʾ */

	bsp_StartAutoTimer(0, 100);	/* ����1��100ms���Զ���װ�Ķ�ʱ�� */
	
	
	/* ��ʼ�� LZO */
	if (lzo_init() != LZO_E_OK)
	{
		printf("LZO��ʼ��ʧ�� \r\n");
		printf("һ���Ǳ�����ɵĲ������������±��룬�����ò����κ��Ż���ͬʱʹ��-DLZO_DEBUG���е���\r\n");
	}

	/* �������ݵ�ѹ���ͽ�ѹ */
	TestMiniLZO(1024*1);
	TestMiniLZO(1024*2); 
	TestMiniLZO(1024*3);
	TestMiniLZO(1024*4);  
	TestMiniLZO(1024*5);
	TestMiniLZO(1024*6);  
	TestMiniLZO(1024*7);
	TestMiniLZO(1024*8);
	TestMiniLZO(1024*9);
	TestMiniLZO(1024*10); 
	TestMiniLZO(1024*11);
	TestMiniLZO(1024*12); 
	TestMiniLZO(1024*13);
	TestMiniLZO(1024*14);  
	TestMiniLZO(1024*15);
	TestMiniLZO(1024*16);  
	TestMiniLZO(1024*17);
	TestMiniLZO(1024*18);
	TestMiniLZO(1024*19);
	TestMiniLZO(1024*20); 	
	
	/* �������ѭ�� */
	while (1)
	{
		/* CPU����ʱִ�еĺ������� bsp.c */
		bsp_Idle();		
		
		/* �ж϶�ʱ����ʱʱ�� */
		if (bsp_CheckTimer(0))	
		{
			/* ÿ��100ms ����һ�� */
			/* ��תLED2��״̬ */
			bsp_LedToggle(2);	
		}

		/* �������¼� */
		ucKeyCode = bsp_GetKey();
		if (ucKeyCode > 0)
		{
			/* �м����� */
			switch (ucKeyCode)
			{
				case KEY_DOWN_K1:			   /* K1������ */
					break;

				default:
					break;
			}
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: TestMiniLZO
*	����˵��: _Len Ҫѹ������������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static uint8_t TestMiniLZO(uint32_t _Len)
{
	uint32_t i;
	int r;
	lzo_uint in_len;
	lzo_uint out_len;
	lzo_uint new_len;
	uint32_t start, end, cnt;

	if(_Len > IN_LEN)
	{
		printf("��������_Len������������ֵ%d\r\n", IN_LEN);
	}
	
	/* ׼��Ҫѹ�������� */
	in_len = _Len;
	
	
	lzo_memset(in,0,in_len);
	
	for(i=0; i< in_len; i++)
	{
		in[i] = rand()%256;
		InTempData[i] = in[i];
	}

	printf("-----------------------------------\r\n");
	printf("-----------------------------------\r\n");
	
	/* ��in�����е�����ѹ����out���� */
	start = DWT_CYCCNT;
	r = lzo1x_1_compress(in,in_len,out,&out_len,wrkmem);
	end = DWT_CYCCNT;
	cnt = end - start;
	
	if (r == LZO_E_OK)
		printf("ѹ�� %lu �ֽ�Ϊ %lu �ֽڣ���ʱ%dus\r\n",
		(unsigned long) in_len, (unsigned long) out_len, cnt/400);
	else
	{
		printf("�ڲ����� - ѹ��ʧ��: %d\r\n", r);
		return 2;
	}
	
	/* ����Ƿ���λ��ѹ������ */
	if (out_len >= in_len)
	{
		printf(" ��δ��ѹ������ \r\n");
		return 0;
	}

	/* ��ѹ���������out����ѹ��in���� */
	start = DWT_CYCCNT;
	r = lzo1x_decompress(out,out_len,in,&new_len,NULL);
	end = DWT_CYCCNT;
	cnt = end - start;
	if (r == LZO_E_OK && new_len == in_len)
		printf("��ѹ %lu �ֽ�Ϊ %lu �ֽڣ���ʱ%dus\r\n",
		(unsigned long) out_len, (unsigned long) in_len, cnt/400);
	else
	{
		printf("�ڲ����� - ѹ��ʧ��: %d\r\n", r);
		return 1;
	}
	
	/* У��ѹ���Ƿ�ɹ� */
	for(i = 0; i < in_len; i++)
	{
		if(InTempData[i] != in[i])
		{
			printf("����ѹ��ʧ��\r\n");
		}
	}
	
	return 0;
}


/*
*********************************************************************************************************
*	�� �� ��: PrintfHelp
*	����˵��: ��ӡ������ʾ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void PrintfHelp(void)
{
	printf("������ʾ:\r\n");
	printf("1. ����ѹ���ͽ�ѹ���ԣ������س������¸������ϵ磬�ϵ���Զ���ӡ\r\n");
}

/*
*********************************************************************************************************
*	�� �� ��: PrintfLogo
*	����˵��: ��ӡ�������ƺ����̷�������, ���ϴ����ߺ󣬴�PC���ĳ����ն�������Թ۲���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	printf("*************************************************************\n\r");
	
	/* ���CPU ID */
	{
		uint32_t CPU_Sn0, CPU_Sn1, CPU_Sn2;
		
		CPU_Sn0 = *(__IO uint32_t*)(0x1FF1E800);
		CPU_Sn1 = *(__IO uint32_t*)(0x1FF1E800 + 4);
		CPU_Sn2 = *(__IO uint32_t*)(0x1FF1E800 + 8);

		printf("\r\nCPU : STM32H743XIH6, BGA240, ��Ƶ: %dMHz\r\n", SystemCoreClock / 1000000);
		printf("UID = %08X %08X %08X\n\r", CPU_Sn2, CPU_Sn1, CPU_Sn0);
	}

	printf("\n\r");
	printf("*************************************************************\n\r");
	printf("* ��������   : %s\r\n", EXAMPLE_NAME);	/* ��ӡ�������� */
	printf("* ���̰汾   : %s\r\n", DEMO_VER);		/* ��ӡ���̰汾 */
	printf("* ��������   : %s\r\n", EXAMPLE_DATE);	/* ��ӡ�������� */

	/* ��ӡST��HAL��汾 */
	printf("* HAL��汾  : V1.3.0 (STM32H7xx HAL Driver)\r\n");
	printf("* \r\n");	/* ��ӡһ�пո� */
	printf("* QQ    : 1295744630 \r\n");
	printf("* ����  : armfly\r\n");
	printf("* Email : armfly@qq.com \r\n");
	printf("* ΢�Ź��ں�: armfly_com \r\n");
	printf("* �Ա���: armfly.taobao.com\r\n");
	printf("* Copyright www.armfly.com ����������\r\n");
	printf("*************************************************************\n\r");
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
