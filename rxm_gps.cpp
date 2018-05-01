#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <cerrno>

using namespace std;

const int baud = 9600;

int main(){
	int device;
	char gpl_data[100];
	
	int setup = wiringPiSetup();
	cout<<"Setup: "<<setup<<endl;
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
		//serialPuts(device, "$PSRF103,00,01,01,00*00");
		//cout<<try_trans<<endl;
		incoming_size = serialDataAvail(device);
		cout<<"There is ["<<incoming_size<<"] characters for reading"<<endl;
		for(int i=0;i<=100;i++){
			char data;
			data = serialGetchar(device);
			gpl_data[i] = data;
		}
		for(int j=0; j<=100; j++){
			cout<<gpl_data[j];
		}
	}
	return 0;
}
