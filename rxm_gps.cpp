#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <wiringSerial.h>
#include <cerrno>

using namespace std;

const int baud = 9600;

int main(){
	int device;
	char gpl_data[10];
	
	if((device = serialOpen("/dev/ttyS0", baud)) < 0){
		cout<<"The serial device couldn't be opened."<<endl;
		return 0;
	}
	else{
		cout<<"Clearing Buffer to begin transmission."<<endl;
		sleep(1);
		serialFlush(device);
		sleep(1);
		cout<<"Ready to begin communication to GPS Module."<<endl;
		int incoming_size = serialDataAvail(device);
		sleep(1);
		cout<<"There is ["<<incoming_size<<"] characters for reading"<<endl;
		//if(serialGetchar(device) == "$GPGLL")
		
	}
	return 0;
}
