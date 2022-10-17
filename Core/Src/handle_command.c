/*
 * handle_command.c
 *
 *  Created on: Oct 13, 2022
 *      Author: JoelBentzen
 */
#include "handle_command.h"
#include "usbd_cdc_if.h"
#include "math.h"

extern uint16_t New_CCR_Public;


void Serial_Init(Message_Handling* name){
	name->CCR_Change_Request_Check = 0;
	memcpy(name->CCR_Request_Text, "Please enter new CCR value (0-65535):\r", 39);
	memcpy(name->CCR_Change_Code, "change.CCR\r", 12);
	name->Listening_For_CCR = 0;
	name->Message_Received = 0;
	memcpy(name->Serial_Not_Understood, "I'm sorry, I don't understand your request.", 43);
	name->is_command = 0;
	name->CCR_Request_Value = 0;
	memcpy(name->Command_Not_Recognised, "I'm sorry, I don't recognise that command.", 42);
	memcpy(name->New_CCR_Is, "New CCR is:", 11);
	name->New_CCR = 0;
	name->New_CCR_Length = 5;
}


void Handle_Message(uint8_t* Buf, uint8_t *Len, Message_Handling *Serial_Info){
	Serial_Info->is_command = memcmp(Buf, "#", 1);
	if (Serial_Info->is_command == 0) {
		Handle_Serial_Command(Buf, Len, Serial_Info);
	}
	else if ((Serial_Info->is_command != 0) && (Serial_Info->Listening_For_CCR != 1)) {
		Error_Message((uint8_t*)Serial_Info->Serial_Not_Understood, sizeof(Serial_Info->Serial_Not_Understood));
		Serial_Info->Message_Received = 0;
	}
	else if ((Serial_Info->Listening_For_CCR == 1) && (Serial_Info->Message_Received = 1)){
		Request_New_CCR(Buf, Len, Serial_Info);
	}
}


void Handle_Serial_Command(uint8_t* Buf, uint8_t *Len, Message_Handling *Serial_Info){
	if (Serial_Info->Listening_For_CCR == 0) {
		Serial_Info->CCR_Change_Request_Check = memcmp(Buf+1, Serial_Info->CCR_Change_Code, sizeof(Serial_Info->CCR_Change_Code));
		if (Serial_Info->CCR_Change_Request_Check == 0){
			CDC_Transmit_FS((uint8_t*)Serial_Info->CCR_Request_Text, sizeof(Serial_Info->CCR_Request_Text)-1);
			Serial_Info->Listening_For_CCR = 1;
		}
		else {
			Error_Message((uint8_t*)Serial_Info->Command_Not_Recognised, sizeof(Serial_Info->Command_Not_Recognised)-1);
		}
	}
	Serial_Info->Message_Received = 0;
	memset(Buf, '\0', *Len);
}


void Request_New_CCR(uint8_t* Buf, uint8_t* Len, Message_Handling *Serial_Info){
	memcpy(&Serial_Info->CCR_Request_Value, Buf, 16);
	if ((Serial_Info->CCR_Request_Value >= 0) && (Serial_Info->CCR_Request_Value <= 65535) && (Serial_Info->Message_Received == 1)){
		memcpy(&Serial_Info->New_CCR, &Serial_Info->CCR_Request_Value, 16);
		//New_CCR_Public = Serial_Info->New_CCR[5];
		memcpy(&New_CCR_Public, &Serial_Info->New_CCR, 16);
		Line_Break();
		CDC_Transmit_FS((uint8_t*)Serial_Info->New_CCR_Is, sizeof(Serial_Info->New_CCR_Is));
		Line_Break();
		CDC_Transmit_FS(&Serial_Info->New_CCR, 5);
		Line_Break();
	}
	Serial_Info->Message_Received = 0;
	Serial_Info->Listening_For_CCR = 0;
}


void Line_Break(){
	char line_break[2];
	memcpy(line_break, "\r", 2);
	CDC_Transmit_FS((uint8_t*)line_break, sizeof(line_break)-1);
}


void Error_Message(uint8_t* Message, uint16_t Len){
	CDC_Transmit_FS(Message, Len);
}
