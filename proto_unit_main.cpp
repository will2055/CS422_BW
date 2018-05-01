/* Enviromental Sensor Source Code
*	Copyright @ April 12, 2018;
*		Author: Brandon Williams, brandon.w2055@gmail.com
*
*/
#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <wiringPiI2C.h>

using namespace std;

class CarbonDioxide{
	private:
		const uint8_t dev_id = 0x70;
		const uint8_t dev_addr = 0b000001100;
		const uint8_t dev_data = 0x00;
		uint8_t sensor_data[7];
		uint8_t crc;
		void initCRC(){
			uint8_t byte_sum = 0x00;
			byte_sum = dev_addr + dev_data + dev_data + dev_data + dev_data;
			crc = byte_sum;
			crc += (byte_sum/(0x0100));
			crc = 0xFF - crc;
		}
		uint8_t send_frame[6]={dev_addr,dev_data,dev_data,
			dev_data,dev_data,crc};
	public:
		int mics_fd,read_res;
		int voc, carbon_dioxide;
		int openComm(){
			if((mics_fd=wiringPiI2CSetup(0x70)) < 0){
				//cout<<"Error Occured in setting up Dev_ID: 0x70"<<endl;
				return 1;
			}
			else{
				if((read_res = read(mics_fd, sensor_data, 8)) != 8){
					cout<<"There could be an issue reading from the file."<<endl;
					cout<<"Read returned: "<<read_res<<endl;
					return 2;
				}
				else{
					voc = (int)sensor_data[0];
					carbon_dioxide = (int)sensor_data[1];
					int resistor_msb = (int)sensor_data[2];
					int resistor_mid = (int)sensor_data[3];
					int resistor_lsb = (int)sensor_data[4];
					int status = (int)sensor_data[5];
					int sensor_crc = (int)sensor_data[6];
					return 0;
				}
			}
		}
		int vocSample(){
			return voc;
		}
		int carbonDioxideSample(){
			return carbon_dioxide;
		}
		
};

class Linx_GPS{
	private:
		int device;
		char gps_data[100];
		int setup;
		int baud = 9600;
	public:
		int openGPS_Serial(){
			if((device = serialOpen("/dev/ttyS0", baud)) < 0){
				//cout<<"The serial device couldn't be opened."<<endl;
				return 1;
			}
			else{
				sleep(1);
				serialFlush(device);
				sleep(1);
				bool exit;
				while(exit!=true){
					if(serialGetchar(device) == '$'){
						gps_data[0]= '$';
						for(int i=1; i<100; i++){
							char next_value;
							next_value = serialGetchar(device);
							if(next_value == '\n'){
								exit = true;
								break;
							}
							else{
								gps_data[i] = next_value;
							}
						}
					}
					exit=true;
				}
			}
		}

};

class Particulate{
	
	public:
		int device;
		char pm_data[8];
		int setup;
		int baud = 57600;
	private:
		int openPM_Serial(){
			if((device = serialOpen("/dev/ttyUSB0", baud)) < 0){
				return 1;
			}
			else{
				serialFlush(device);
				for (int i=0; i < 8; i++){
					pm_data[i] = (int)serialGetchar(device);
				}
				return pm_data;
			}
		}
};
int main(){
	
	/*
	 *  Initialize Classes
	 */
	CarbonDioxide mics;
	Linx_GPS linx;
	Particulate pm;

	/*
	 * Initialize main loop
	 */
	for(int a=0; a<100; a++){
		cout<<"Sample: "<<a<<endl;
		/*
		 * Linx GPS
		 */
		 
		char gps_arr[100];
		gps_arr = linx.openGPS_Serial();
		for(int i=0; i<100; i++){
			cout<<gps_arr[i];
		}
		cout<<endl;
		
		/*
		 * PM2.5 & PM10
		 */
		int pm_data[8];
		pm_data = pm.openPM_Serial();
		cout<<pm_data[4]<<", "<<pm_data[5]<<", "<<pm_data[6]<<", "<<pm_data[7]<<endl;
		
		/*
		 * Carbon Dioxide Sensor
		 */
		 
		int mics_res = mics.openComm();
		int voc_data;
		int carDio_data;
		if(mics_res == 0){
			voc_data = mics.vocSample();
			carDio_data = mics.carbonDioxideSample();
			cout<<voc_data<<","<<carDio_data<<endl;
		}
		if(mics_res == 1){
			cout<<"Error occured int the opening of the I2C device"<<endl;
		}
		if(mics_res == 2){
			cout<<"Error occured in reading from the I2C device"<<endl;
		}
		
		cout<<endl;
	}
	return 0;
}
