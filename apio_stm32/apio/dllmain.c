#define bool 	_Bool
#define false	0
#define true	1

#define BYTES_TO_WRITE	8
#define BYTES_TO_READ	8
#define START_SYM	'#'

#define WIN32_LEAN_AND_MEAN  /* speed up */
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdio.h>
#include <sysinfoapi.h>
#include <time.h>

#define _APIO_
#include "APIO.h"

#define AP_Debug	0
#define AP_EMU		0

#define AP_COM_SPEED		CBR_9600
#define AP_ADAPTER_TYPE		10

#define AP_COM_DELAY		1

int errors_count=0;

unsigned int op_index=0;

time_t	now;

HANDLE			AP_COM_PORT;
bool			AP_COM_CONNECTED=false;

char			ap_com_buff[128];
char			ap_com_buff_rx[128];
short			ap_com_buff_index;
unsigned int 	TimeStamp;
unsigned int 	TimeStampCurrent;
unsigned int 	TimeOut=10;
unsigned int	dwBytesWritten=0;
unsigned int	dwBytesReaded=0;


FILE *__debug_log_file;
char *__debug_file_name="apio.log";
char __debug_log_success_opened=0;
char* AP_GetCurrentTime();

// apio.dll variables
int Adapter_Connection_Type=1;


APIOAPI HRESULT __cdecl DllUnregisterServer(void)
{
	if(__debug_log_success_opened)
	{
		__debug_log_success_opened=0;
		fflush(__debug_log_file);
		fclose(__debug_log_file);
	}

	if(AP_COM_CONNECTED)
	{
		fclose(AP_COM_PORT);
		AP_COM_CONNECTED=false;
	}

	return S_OK;
}

BOOL APIENTRY DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if(AP_Debug)
	{
		if(!__debug_log_file)
		{
			__debug_log_file=fopen(__debug_file_name,"a");
		
			if(__debug_log_file)
			{
				__debug_log_success_opened=1;
			}
		}
	}

    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
			{
			}
            break;

        case DLL_THREAD_ATTACH:
			{
			}
            break;

        case DLL_THREAD_DETACH:
			{
				if(__debug_log_success_opened)
				{
					__debug_log_success_opened=0;
					fflush(__debug_log_file);
					fclose(__debug_log_file);
				}
			}
            break;

        case DLL_PROCESS_DETACH:
			{
				if(__debug_log_success_opened)
				{
					__debug_log_success_opened=0;
					fflush(__debug_log_file);
					fclose(__debug_log_file);
				}
			}
            break;
    }
    return TRUE;
}

APIOAPI int __cdecl ap_write_array(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite)
{
	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"HW: ap_write_array\n");
			fflush(__debug_log_file);
		}
	};
	return 0;	
}


APIOAPI int __cdecl ap_any_ready(void)
{
	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"HW: any ready\n");
			fflush(__debug_log_file);
		}
	};
	return 1;
}

APIOAPI int __cdecl ap_block_write_tx(DWORD BytesReturned, int a2, int a3)
{	
	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"HW: block_write_tx\n");
			fflush(__debug_log_file);
		}
	};
	return 0;
}


APIOAPI int __cdecl ap_flush_rx(DWORD InBuffer)
{
	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"HW: any ready\n");
			fflush(__debug_log_file);
		}
	};
	return 0;
}

APIOAPI int __cdecl ap_get_pciaddr(void)
{
	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"HW: flush rx\n");
			fflush(__debug_log_file);
		}
	};
	return 0x238;
}

APIOAPI int __cdecl ap_getpcitypeandaddress(void)
{
	/*if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"APWIN: get pci type and address\n");
			fflush(__debug_log_file);		
		}
	};*/
	return 10;
}

APIOAPI int __cdecl ap_hv(DWORD BytesReturned, char a2, int a3, int a4)
{
	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"HW: ap_hv\n");
			fflush(__debug_log_file);
		}
	};
	return 1;
}

APIOAPI DWORD __cdecl ap_read_array(LPVOID lpBuffer, DWORD nNumberOfBytesToRead)
{
	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"HW: read array\n");
			fflush(__debug_log_file);
		}
	};
	return 0;
}



APIOAPI int __cdecl ap_read_dsp_addr(int a1, int a2, int a3, int a4, int a5, int a6)
{
	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"HW: read dsp addr\n");
			fflush(__debug_log_file);
		}
	};
	return 1;
}

APIOAPI int __cdecl ap_read_rx(DWORD BytesReturned, int a2, int a3, int a4)
{
	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"HW: read rx\n");
			fflush(__debug_log_file);
		}
	};
	return 0;
}


APIOAPI int __cdecl ap_reset(void *this)
{
	ap_com_buff[0]='<';
	ap_com_buff[1]='w';
	ap_com_buff[2]='>';
	ap_com_buff[3]='7'; //read byte
	ap_com_buff[4]='0'; //address
	ap_com_buff[5]=0;//data 1
	ap_com_buff[6]=0;//data 2
	ap_com_buff[7]=0;//data 3

	for(int i=0;i<BYTES_TO_READ;i++)
	{
		ap_com_buff_rx[i]=0;
	}

	WriteFile(AP_COM_PORT,ap_com_buff,BYTES_TO_WRITE,&dwBytesWritten,NULL);
	ReadFile(AP_COM_PORT,ap_com_buff_rx,BYTES_TO_READ,&dwBytesReaded,NULL);

	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"HW: reset, returned: 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X\n",ap_com_buff_rx[0],ap_com_buff_rx[1],ap_com_buff_rx[2],ap_com_buff_rx[3],0xff & ap_com_buff_rx[4],ap_com_buff_rx[5],ap_com_buff_rx[6],ap_com_buff_rx[7]);
			fflush(__debug_log_file);
		}
	};

	if(ap_com_buff_rx[0]=='<')
	{
		if(ap_com_buff_rx[1]=='r')
		{
			if(ap_com_buff_rx[2]=='>')
			{
				if(ap_com_buff_rx[3]=='7') //read word
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

APIOAPI UINT __cdecl ap_retrievepcitypeandaddress(void)
{
	if(AP_COM_CONNECTED)
	{
		if(AP_Debug)
		{
			if(__debug_log_success_opened)
			{
				fprintf(__debug_log_file,"APWIN: retrieve pci type and address - USB-APIB all ready detected\n");
				fflush(__debug_log_file);
			}
		};
		return AP_ADAPTER_TYPE;
	}
	else
	{
		// search and check port
		char __buff[]="apib";
		char __port[32];
		
		for(int i=0;i<32;i++)
		{
			sprintf(__port,"COM%i",i);
			AP_COM_PORT=CreateFile(__port, GENERIC_READ | GENERIC_WRITE,
                       0,      //  must be opened with exclusive-access
                       NULL,   //  default security attributes
                       OPEN_EXISTING, //  must use OPEN_EXISTING
					   0,
                       //FILE_FLAG_NO_BUFFERING,      //  not overlapped I/O
                       NULL ); //  hTemplate must be NULL for comm devices
			if(AP_COM_PORT!=INVALID_HANDLE_VALUE)
			{
				if(AP_Debug)
				{
					if(__debug_log_success_opened)
					{
						fprintf(__debug_log_file,"APWIN: retrieve pci type and address - COM%i port opened, check device...\n",i);
						fflush(__debug_log_file);
					}
				}
				DCB dcbSerialParams = {0};
				dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
				if (!GetCommState(AP_COM_PORT, &dcbSerialParams))
				{
					fclose(AP_COM_PORT);
    				continue;
				}
				dcbSerialParams.BaudRate=AP_COM_SPEED;
				dcbSerialParams.ByteSize=8;
				dcbSerialParams.StopBits=ONESTOPBIT;
				dcbSerialParams.Parity=NOPARITY;
				if(!SetCommState(AP_COM_PORT, &dcbSerialParams))
				{
					fclose(AP_COM_PORT);
    				continue;
				}

				WriteFile(AP_COM_PORT,__buff,4,&dwBytesWritten,NULL);
				Sleep(AP_COM_DELAY);
				if(AP_Debug)
				{
					if(__debug_log_success_opened)
					{
						fprintf(__debug_log_file,"APWIN: retrieve pci type and address - COM%i write signature\n",i);
						fflush(__debug_log_file);
					}
				}
				TimeStampCurrent=GetTickCount();
				while(GetTickCount()-TimeStampCurrent<TimeOut)
				{
					ReadFile(AP_COM_PORT,ap_com_buff,3,&dwBytesWritten,NULL);
					if(AP_Debug)
					{
							fprintf(__debug_log_file,"APWIN: retrieve pci type and address - COM%i read signature\n",i);
							fflush(__debug_log_file);
						
					}
					if(dwBytesWritten>0)
					{
						if(ap_com_buff[0]=='u')
						{
							if(ap_com_buff[1]=='s')
							{
								if(ap_com_buff[2]=='b')
								{
									if(AP_Debug)
									{
										if(__debug_log_success_opened)
										{
											fprintf(__debug_log_file,"APWIN: retrieve pci type and address - USB-APIB detected at COM%i\n",i);
											fflush(__debug_log_file);
										}
									}
									AP_COM_CONNECTED=true;
									return AP_ADAPTER_TYPE;
								}
							}
						}
					}
				}
				
				if(AP_Debug)
				{
					if(__debug_log_success_opened)
					{
						fprintf(__debug_log_file,"APWIN: retrieve pci type and address - USB-APIB not detected at COM%i\n",i);
						fflush(__debug_log_file);
					}
				}
				return 0;
			}
			else
			{
				if(AP_Debug)
				{
					if(__debug_log_success_opened)
					{
						fprintf(__debug_log_file,"APWIN: retrieve pci type and address - USB-APIB not detected at COM%i - invalid port\n",i);
						fflush(__debug_log_file);
					}
				}
			}
		};
		MessageBox(0,"USB-APIB not found","Ap USB", MB_OK);
		return 0;
	};
	return 0;
}

APIOAPI char __cdecl ap_set_pciaddr(DWORD BytesReturned)
{
	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"APWIN: set pci address: 0x%X\n",BytesReturned);
			fflush(__debug_log_file);
		}
	};
	return BytesReturned+7;
}

APIOAPI char __cdecl ap_setpcitypeandaddress(int a1)
{
	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"APWIN: set pci and type address: 0x%X\n",a1);
			fflush(__debug_log_file);
		}
	};
	return a1;
}

APIOAPI char __cdecl ap_setup_pci(void)
{
	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"APWIN: setup pci\n");
			fflush(__debug_log_file);
		}
	};
	return 3;
}

APIOAPI int __cdecl ap_write_8bit_array(__int64 a1, char a2, char a3, int a4, int a5)
{
	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"HW: write 8bit array\n");
			fflush(__debug_log_file);
		}
	};
	return 0;
}

APIOAPI int __cdecl ap_write_addr(DWORD InBuffer)
{
	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"HW: write address\n");
			fflush(__debug_log_file);
		}
	};
	return InBuffer;
}



APIOAPI int __cdecl ap_write_to_dsp_addr(int a1, int a2, int a3, int a4, int a5, int a6)
{
	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"HW: write to dsp address\n");
			fflush(__debug_log_file);
		}
	};
	return 0;
}

APIOAPI int __cdecl ap_write_tx(DWORD BytesReturned, int a2, int a3, int a4)
{
	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"HW: write tx\n");
			fflush(__debug_log_file);
		}
	};
	return 0;
}


APIOAPI int __cdecl ap_xilinx_block_download(LPCVOID lpBuffer, DWORD BytesReturned, int Buffer)
{
	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"HW: xilinx_block_download\n");
			fflush(__debug_log_file);
		}
	};
	return 0;
}

APIOAPI int __cdecl dsp_any_ready(void)
{
	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"HW: dsp_any_ready\n");
			fflush(__debug_log_file);
		}
	};
	return 0;
}

bool check_signature(char* buff, unsigned int len)
{
	return 0;
}


APIOAPI unsigned int __cdecl ap_check_pfail(void)
{
	ap_com_buff[0]='<';
	ap_com_buff[1]='w';
	ap_com_buff[2]='>';
	ap_com_buff[3]='8'; //fail check
	ap_com_buff[4]='0'; //address
	ap_com_buff[5]=0;//data 1
	ap_com_buff[6]=0;//data 2
	ap_com_buff[7]=0;//data 3

	for(int i=0;i<BYTES_TO_READ;i++)
	{
		ap_com_buff_rx[i]=0;
	}

	WriteFile(AP_COM_PORT,ap_com_buff,BYTES_TO_WRITE,&dwBytesWritten,NULL);
	ReadFile(AP_COM_PORT,ap_com_buff_rx,BYTES_TO_READ,&dwBytesReaded,NULL);

	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"HW: check FAIL: returned: 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X\n",ap_com_buff_rx[0],ap_com_buff_rx[1],ap_com_buff_rx[2],ap_com_buff_rx[3],ap_com_buff_rx[4],ap_com_buff_rx[5],ap_com_buff_rx[6],ap_com_buff_rx[7]);
			fflush(__debug_log_file);
		}
	};

	if(ap_com_buff_rx[0]=='<')
	{
		if(ap_com_buff_rx[1]=='r')
		{
			if(ap_com_buff_rx[2]=='>')
			{
				if(ap_com_buff_rx[3]=='8') //check att
				{
					if(ap_com_buff_rx[4]==0)
					{
						return 0;
					}
					else
					{
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

APIOAPI int __cdecl ap_check_att(DWORD BytesReturned)
{
	ap_com_buff[0]='<';
	ap_com_buff[1]='w';
	ap_com_buff[2]='>';
	ap_com_buff[3]='0'; //att check
	ap_com_buff[4]=0xff & BytesReturned; //address
	ap_com_buff[5]=0;//data 1
	ap_com_buff[6]=0;//data 2
	ap_com_buff[7]=0;//data 3

	for(int i=0;i<BYTES_TO_READ;i++)
	{
		ap_com_buff_rx[i]=0;
	}

	WriteFile(AP_COM_PORT,ap_com_buff,BYTES_TO_WRITE,&dwBytesWritten,NULL);
	ReadFile(AP_COM_PORT,ap_com_buff_rx,BYTES_TO_READ,&dwBytesReaded,NULL);

	/*if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"HW: check ATT: address=0x%X returned: 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X\n",BytesReturned,ap_com_buff_rx[0],ap_com_buff_rx[1],ap_com_buff_rx[2],ap_com_buff_rx[3],ap_com_buff_rx[4],ap_com_buff_rx[5],ap_com_buff_rx[6],ap_com_buff_rx[7]);
			fflush(__debug_log_file);
		}
	};*/

	if(ap_com_buff_rx[0]=='<')
	{
		if(ap_com_buff_rx[1]=='r')
		{
			if(ap_com_buff_rx[2]=='>')
			{
				if(ap_com_buff_rx[3]=='0') //check att
				{
					if(ap_com_buff_rx[4]==0)
					{
						return 1;
					}
					else
					{
						return 0;
					}
				}
			}
		}
	}

	return 0;
}

APIOAPI int __cdecl ap_read_byte(char BytesReturned)
{
	ap_com_buff[0]='<';
	ap_com_buff[1]='w';
	ap_com_buff[2]='>';
	ap_com_buff[3]='4'; //read byte
	ap_com_buff[4]=0xff & BytesReturned; //address
	ap_com_buff[5]=0;//data 1
	ap_com_buff[6]=0;//data 2
	ap_com_buff[7]=0;//data 3

	for(int i=0;i<BYTES_TO_READ;i++)
	{
		ap_com_buff_rx[i]=0;
	}

	WriteFile(AP_COM_PORT,ap_com_buff,BYTES_TO_WRITE,&dwBytesWritten,NULL);
	Sleep(AP_COM_DELAY);
	ReadFile(AP_COM_PORT,ap_com_buff_rx,BYTES_TO_READ,&dwBytesReaded,NULL);

	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			//fprintf(__debug_log_file,"HW: read byte, address=0x%X, returned: 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X\n",0xff & BytesReturned,ap_com_buff_rx[0],ap_com_buff_rx[1],ap_com_buff_rx[2],ap_com_buff_rx[3],0xff & ap_com_buff_rx[4],ap_com_buff_rx[5],ap_com_buff_rx[6],ap_com_buff_rx[7]);
			fflush(__debug_log_file);
		}
	};

	if(ap_com_buff_rx[0]=='<')
	{
		if(ap_com_buff_rx[1]=='r')
		{
			if(ap_com_buff_rx[2]=='>')
			{
				if(ap_com_buff_rx[3]=='4') //read byte
				{
					return (int)ap_com_buff_rx[4];
				}
			}
		}
	}
	return 0;
}

APIOAPI int __cdecl ap_read_triple(char BytesReturned)
{
	ap_com_buff[0]='<';
	ap_com_buff[1]='w';
	ap_com_buff[2]='>';
	ap_com_buff[3]='6'; //read byte
	ap_com_buff[4]=0xff & BytesReturned; //address
	ap_com_buff[5]=0;//data 1
	ap_com_buff[6]=0;//data 2
	ap_com_buff[7]=0;//data 3

	for(int i=0;i<BYTES_TO_READ;i++)
	{
		ap_com_buff_rx[i]=0;
	}

	WriteFile(AP_COM_PORT,ap_com_buff,BYTES_TO_WRITE,&dwBytesWritten,NULL);
	ReadFile(AP_COM_PORT,ap_com_buff_rx,BYTES_TO_READ,&dwBytesReaded,NULL);

	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"HW: read word, address=0x%X, returned: 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X\n",0xff & BytesReturned,ap_com_buff_rx[0],ap_com_buff_rx[1],ap_com_buff_rx[2],ap_com_buff_rx[3],0xff & ap_com_buff_rx[4],ap_com_buff_rx[5],ap_com_buff_rx[6],ap_com_buff_rx[7]);
			fflush(__debug_log_file);
		}
	};

	if(ap_com_buff_rx[0]=='<')
	{
		if(ap_com_buff_rx[1]=='r')
		{
			if(ap_com_buff_rx[2]=='>')
			{
				if(ap_com_buff_rx[3]=='6') //read word
				{
					return (0xff&ap_com_buff_rx[4])+(0xff&ap_com_buff_rx[5]<<8)+(0xff&ap_com_buff_rx[6]<<16);
				}
			}
		}
	}
	MessageBox(0,"Read tripple error", "Ap USB",MB_OK);
	return 0;
}

APIOAPI int __cdecl ap_read_word(char BytesReturned)
{
	ap_com_buff[0]='<';
	ap_com_buff[1]='w';
	ap_com_buff[2]='>';
	ap_com_buff[3]='5'; //read byte
	ap_com_buff[4]=0xff & BytesReturned; //address
	ap_com_buff[5]=0;//data 1
	ap_com_buff[6]=0;//data 2
	ap_com_buff[7]=0;//data 3

	for(int i=0;i<BYTES_TO_READ;i++)
	{
		ap_com_buff_rx[i]=0;
	}

	WriteFile(AP_COM_PORT,ap_com_buff,BYTES_TO_WRITE,&dwBytesWritten,NULL);
	Sleep(AP_COM_DELAY);
	ReadFile(AP_COM_PORT,ap_com_buff_rx,BYTES_TO_READ,&dwBytesReaded,NULL);

	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"HW: read word, address=0x%X, returned: 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X\n",0xff & BytesReturned,ap_com_buff_rx[0],ap_com_buff_rx[1],ap_com_buff_rx[2],ap_com_buff_rx[3],0xff & ap_com_buff_rx[4],ap_com_buff_rx[5],ap_com_buff_rx[6],ap_com_buff_rx[7]);
			fflush(__debug_log_file);
		}
	};

	union
	{
		int    word;
  		struct
		{
			char byte1;
			char byte2;
		};
	} result;

	result.byte1=ap_com_buff_rx[4];
	result.byte2=ap_com_buff_rx[5];

	if(ap_com_buff_rx[0]=='<')
	{
		if(ap_com_buff_rx[1]=='r')
		{
			if(ap_com_buff_rx[2]=='>')
			{
				if(ap_com_buff_rx[3]=='5') //read word
				{
					return result.word;
					//return 0xffff & (ap_com_buff_rx[4]+ ap_com_buff_rx[5]<<8);
				}
			}
		}
	}
	return 0;
}

APIOAPI int __cdecl ap_write_word(int a1, char BytesReturned)
{
	ap_com_buff[0]='<';
	ap_com_buff[1]='w';
	ap_com_buff[2]='>';
	ap_com_buff[3]='2'; //write word
	ap_com_buff[4]=BytesReturned; //address
	ap_com_buff[6]=0xff & (a1>>8);//data 2
	ap_com_buff[5]=a1;//data 1
	
	ap_com_buff[7]=0;//data 3

	for(int i=0;i<BYTES_TO_READ;i++)
	{
		ap_com_buff_rx[i]=0;
	}

	WriteFile(AP_COM_PORT,ap_com_buff,BYTES_TO_WRITE,&dwBytesWritten,NULL);
	Sleep(AP_COM_DELAY);
	ReadFile(AP_COM_PORT,ap_com_buff_rx,BYTES_TO_READ,&dwBytesReaded,NULL);

	//MessageBox(0,"ap_write_word","Ap USB",MB_OK);
	if(AP_Debug)
	//if(1)
	{
	//if(1)
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"HW: write word\n");
			fprintf(__debug_log_file,"HW: write word, address=0x%X, data=0x%X returned: 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X\n",0xff & BytesReturned,0xFF & a1,ap_com_buff_rx[0],ap_com_buff_rx[1],ap_com_buff_rx[2],ap_com_buff_rx[3],0xff & ap_com_buff_rx[4],ap_com_buff_rx[5],ap_com_buff_rx[6],ap_com_buff_rx[7]);
			fflush(__debug_log_file);
		}
	};

	return ap_com_buff[6];

	if(ap_com_buff_rx[0]=='<')
	{
		if(ap_com_buff_rx[1]=='r')
		{
			if(ap_com_buff_rx[2]=='>')
			{
				if(ap_com_buff_rx[3]=='2') //write word
				{
					if(ap_com_buff_rx[5]==ap_com_buff[5])
					{
						return ap_com_buff[5];
					}
					else
					{
						char __buff[1024];
						sprintf(__buff,"HW: write word error, address=0x%X, data=0x%X returned: 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X\n",0xff & BytesReturned,a1,ap_com_buff_rx[0],ap_com_buff_rx[1],ap_com_buff_rx[2],ap_com_buff_rx[3],ap_com_buff_rx[4],ap_com_buff_rx[5],ap_com_buff_rx[6],ap_com_buff_rx[7]);
						MessageBox(0,__buff,"Ap USB",MB_OK);
						ExitProcess(1u);
					}
				}
			}
		}
	}
	MessageBox(0,"ap_write_word error 2","Ap USB",MB_OK);
	return 0;
}

APIOAPI int __cdecl ap_write_byte(int a1, int BytesReturned)
{

	ap_com_buff[0]='<';
	ap_com_buff[1]='w';
	ap_com_buff[2]='>';
	ap_com_buff[3]='1'; //write byte
	ap_com_buff[4]=0xff & BytesReturned; //address
	ap_com_buff[5]=0xff & a1;//data 1
	ap_com_buff[6]=0;//data 2
	ap_com_buff[7]=0;//data 3

	for(int i=0;i<BYTES_TO_READ;i++)
	{
		ap_com_buff_rx[i]=0;
	}

	WriteFile(AP_COM_PORT,ap_com_buff,BYTES_TO_WRITE,&dwBytesWritten,NULL);
	Sleep(AP_COM_DELAY);
	ReadFile(AP_COM_PORT,ap_com_buff_rx,BYTES_TO_READ,&dwBytesReaded,NULL);

	return ap_com_buff[5];

	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			//fprintf(__debug_log_file,"HW: write byte, address=0x%X, data=0x%X returned: 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X\n",0xff & BytesReturned,0xFF & a1,ap_com_buff_rx[0],ap_com_buff_rx[1],ap_com_buff_rx[2],ap_com_buff_rx[3],0xff & ap_com_buff_rx[4],ap_com_buff_rx[5],ap_com_buff_rx[6],ap_com_buff_rx[7]);
			//fflush(__debug_log_file);
		}
	};

	if(ap_com_buff_rx[0]=='<')
	{
		if(ap_com_buff_rx[1]=='r')
		{
			if(ap_com_buff_rx[2]=='>')
			{
				if(ap_com_buff_rx[3]=='1') //write byte
				{
					if(ap_com_buff_rx[4]==(0xff & a1))
					{
						return 0xff & a1;
					}
					else
					{
						MessageBox(0,"ap_write_byte error","Ap USB",MB_OK);
						ExitProcess(1u);
					}
				}
			}
		}
	}

	return 0;
}

APIOAPI int __cdecl ap_write_triple(int a1, char BytesReturned)
{
	ap_com_buff[0]='<';
	ap_com_buff[1]='w';
	ap_com_buff[2]='>';
	ap_com_buff[3]='3'; //write word
	ap_com_buff[4]=0xff & BytesReturned; //address
	ap_com_buff[5]=0xff & a1;//data 1
	ap_com_buff[6]=0xff&(a1>>8);//data 2
	ap_com_buff[7]=0xff&(a1>>16);//data 3

	for(int i=0;i<BYTES_TO_READ;i++)
	{
		ap_com_buff_rx[i]=0;
	}

	WriteFile(AP_COM_PORT,ap_com_buff,BYTES_TO_WRITE,&dwBytesWritten,NULL);
	ReadFile(AP_COM_PORT,ap_com_buff_rx,BYTES_TO_READ,&dwBytesReaded,NULL);

	if(AP_Debug)
	{
		if(__debug_log_success_opened)
		{
			fprintf(__debug_log_file,"HW: write tripple, address=0x%X, data=0x%X returned: 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X\n",0xff & BytesReturned,0xFF & a1,ap_com_buff_rx[0],ap_com_buff_rx[1],ap_com_buff_rx[2],ap_com_buff_rx[3],0xff & ap_com_buff_rx[4],ap_com_buff_rx[5],ap_com_buff_rx[6],ap_com_buff_rx[7]);
			fflush(__debug_log_file);
		}
	};

	if(ap_com_buff_rx[0]=='<')
	{
		if(ap_com_buff_rx[1]=='r')
		{
			if(ap_com_buff_rx[2]=='>')
			{
				if(ap_com_buff_rx[3]=='3') //write tripple
				{
					if((0xff&ap_com_buff_rx[4])==(0xff & (a1>>16)))
					{
						return 0xff & (a1>>16);
					}
					else
					{
						MessageBox(0,"ap_write_tripple error","Ap USB",MB_OK);
						ExitProcess(1u);
					}
				}
			}
		}
	}
	return 0;
}
