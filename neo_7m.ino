#include "./TinyGPS++.h"
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>

// GPS 클래스 생성
TinyGPSPlus gps;

// sd카드
Sd2Card card;
SdVolume volume;
SdFile root;

File myFile;

char file_name[13];
char data_date[11];
char data_location[11];
char data_time[12];

void setup(){
	Serial.begin(115200);           // 컴퓨터와 통신하기 위한 Serial
	Serial1.begin(9600);            // GPS Serial

	// SD CARD CS핀 = 9
	if (!SD.begin(9)) {
		Serial.println("initialization failed!");
		return;
	}


}

void loop(){
	// GPS 가 연결되면 정보 불러오기
	while (Serial1.available() > 0)

		if (gps.encode(Serial1.read())){

			// 시간 & 날짜 정보
			if (gps.date.isValid()){
				// 각 시간을 불러와서 변수에 저장
				unsigned char data_month = gps.date.month();
				unsigned char data_day = gps.date.day();
				unsigned int data_year = gps.date.year();

				// 각 변수들을 하나의 문장으로 형성
				sprintf(file_name,"%04d%02d%02d.txt", data_year, data_month, data_day);
				sprintf(data_date,"%04d/%02d/%02d", data_year, data_month, data_day);

				// SD 카드에 txt 파일 생성
				myFile = SD.open(file_name, FILE_WRITE);

				if (myFile){
					myFile.println(data_date);
					//Serial.print(data_date);
				} 
			}
			// 위치 정보
			if (gps.location.isValid()){

				float data_locationLat = gps.location.lat();
				float data_locationLng = gps.location.lng();

				Serial.print(data_locationLat);
				Serial.print("/");
				Serial.print(gps.location.lat(), 6);

			}

			// 시간 정보
			if (gps.time.isValid()) {
				unsigned char data_timeHour = gps.time.hour();
				unsigned char data_timeMin = gps.time.minute();
				unsigned char data_timeSec = gps.time.second();
				unsigned char data_timeCSec = gps.time.centisecond();

				sprintf(data_time,"%02d:%02d/%02d.%02d", data_timeHour, data_timeMin, data_timeSec, data_timeCSec);
				Serial.println(data_time);

			}


		}



	myFile.close();
	delay(1000);
}
