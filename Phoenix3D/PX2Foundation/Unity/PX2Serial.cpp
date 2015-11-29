// PX2Serial.cpp

#include "PX2Serial.hpp"
using namespace PX2;

#ifdef LINUX
int fd, res;
struct termios oldtio, newtio;
#endif

//---------------------------------------------------------------------------
bool Serial::Init(const char *comport_in, int baudrate)
{
	bool flag = true;

#ifdef WIN32
	DCB dcb1;

	sprintf(comport, comport_in);

	if (flag_opened == 1)
		this->Close();

	hcom = CreateFile(comport,
		GENERIC_READ | GENERIC_WRITE,	// 读写
		0,								// 独占
		NULL,
		OPEN_EXISTING, 
		NULL,							// 同步
		NULL);

	// 获得当前串口配置信息
	GetCommState(hcom, &dcb1);

	dcb1.BaudRate = baudrate;

	dcb1.fParity = 1;//0
	dcb1.Parity = NOPARITY;//for AI_Motor
	//dcb1.Parity=EVENPARITY;
	dcb1.StopBits = ONESTOPBIT;//for AI_Motor
	dcb1.ByteSize = 8;//for AI Motor
	dcb1.fNull = FALSE;//
	//dcb1.EvtChar=STX;
	flag = (TRUE==SetCommState(hcom, &dcb1));

	if (flag == true)
	{
		GetCommMask(hcom, &mask);
		//mask=EV_RXCHAR|EV_RXFLAG;
		mask = EV_RXCHAR;
		flag = (TRUE == SetCommMask(hcom, mask));
	}

	if (flag == true)
	{
		GetCommTimeouts(hcom, &ctmo);
		ctmo.ReadIntervalTimeout = 1000;//0;//30;//SICKLMS偱偼6ms
		ctmo.ReadTotalTimeoutMultiplier = 5000;//=1;//0928
		//ctmo.ReadTotalTimeoutConstant=1000;//=6;//0928

		flag = (TRUE == SetCommTimeouts(hcom, &ctmo));
	}

	// 清空缓冲区，为读写串口做准备
	PurgeComm(hcom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR |
		PURGE_RXCLEAR);
	flag_opened = 1;
#endif


#ifdef LINUX
	// fd = open(comport_in, O_RDWR | O_NOCTTY ); 
	fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY);
	if (fd < 0) {
		perror(comport_in);
		//printf("init() error. %s is not opend. fd=%d\n", comport_in, fd);//debug
		return(false);
	}
	//printf("init() com opened\n");//debug
	flag_opened = 1;

	tcgetattr(fd, &oldtio);

	bzero(&newtio, sizeof(newtio));

	tcflag_t baud;
	if (baudrate == 9600)baud = B9600;
	if (baudrate == 38400)baud = B38400;
	if (baudrate == 57600)baud = B57600;

	// newtio.c_cflag = baud | CRTSCTS | CS8 | CLOCAL | CREAD;
	newtio.c_cflag = baud | IGNPAR | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;

	/* set input mode (non-canonical, no echo,...) */
	newtio.c_lflag = 0;
#endif

	return(flag);
}
//---------------------------------------------------------------------------
void Serial::Clear()
{
#ifdef WIN32
	PurgeComm(hcom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR |
		PURGE_RXCLEAR);
#endif

#ifdef LINUX
	tcflush(fd, TCIFLUSH);
#endif

}
//---------------------------------------------------------------------------
bool Serial::Close()
{
	if (flag_opened != 1)return false;
	flag_opened = 0;

#ifdef WIN32
	return (TRUE == CloseHandle(hcom));
#endif

#ifdef LINUX
	::close(fd);
	return true;
#endif
}
//---------------------------------------------------------------------------
int Serial::Receive(char *buf_ptr, int size)
{
	unsigned long byte, event;
	byte = 0;
	bool flag = false;

#ifdef WIN32
	while (!flag)
	{
		WaitCommEvent(hcom, &event, NULL);
		if (event | EV_RXCHAR)
		{
			if (ReadFile(hcom, buf_ptr, size, &byte, NULL))
				flag = true;
			break;
		}
		byte = 0;
	}
#endif

#ifdef LINUX
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);
	byte = read(fd, buf_ptr, size);
#endif

	return byte;
}
//---------------------------------------------------------------------------
bool Serial::Receive2(char *buf_ptr, int size)
{
	unsigned long byte;
	bool flag = false;

#ifdef WIN32
	//WaitCommEvent(hcom,&event,NULL);
	if (ReadFile(hcom, buf_ptr, size, &byte, NULL))
		flag = true;
#endif

	return flag;
}
//---------------------------------------------------------------------------
int Serial::Send(const char *buf_ptr, int size)
{
	unsigned long byte;
	if (flag_opened != 1)
	{
		printf("send() error. port Not opend\n");//debug
		printf("flag_opened=%d\n", flag_opened);//debug
		return -1;
	}

#ifdef WIN32
	if (WriteFile(hcom, buf_ptr, size, &byte, NULL))
	{
		return byte;
	}
	else return -1;
#endif

#ifdef LINUX
	byte = write(fd, buf_ptr, size);
	if (byte == 0)
	{
		//printf("write error byte 0\n");//debug
		return -1;
	}
	//printf("write byte=%d\n",byte);
	return (byte);
#endif

}
//---------------------------------------------------------------------------

