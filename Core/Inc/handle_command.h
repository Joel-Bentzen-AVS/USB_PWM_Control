/*
 * handle_command.h
 *
 *  Created on: Oct 13, 2022
 *      Author: JoelBentzen
 */

#ifndef INC_HANDLE_COMMAND_H_
#define INC_HANDLE_COMMAND_H_

#include "main.h"


typedef struct
{
	uint8_t CCR_Change_Request_Check;
	char CCR_Request_Text[39];
	char CCR_Change_Code[12];
	uint8_t Listening_For_CCR;
	uint8_t Message_Received;
	char Serial_Not_Understood[43];
	uint8_t is_command;
	uint16_t CCR_Request_Value;
	char Command_Not_Recognised[42];
	char New_CCR_Is[11];
	uint8_t New_CCR;
	uint8_t New_CCR_Length;

} Message_Handling;

void Serial_Init(Message_Handling* name);

void Handle_Message(uint8_t* Buf, uint8_t *Len, Message_Handling *Serial_Info);

void Line_Break();

void Error_Message(uint8_t* Message, uint16_t Len);

void Handle_Serial_Command(uint8_t* Buf, uint8_t *Len, Message_Handling *Serial_Info);

void Request_New_CCR(uint8_t* Buf, uint8_t* Len, Message_Handling *Serial_Info);

int Check_CCR_Valid();


#endif /* INC_HANDLE_COMMAND_H_ */
