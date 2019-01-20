#include "MQTT_Packet_Writer.h"
#include <string.h>
#include <stdlib.h>
#include "eat_interface.h"

static int data_frame_init(data_packet_t* dataFrame)
{
	if(!dataFrame->data)
			return -1;
	dataFrame->totalSize = 0;
	dataFrame->nextPos =0;
	memset(dataFrame->data, '\0', sizeof(dataFrame->data));
	dataFrame->mqtt_response_handler = NULL;
	return 0;
}




data_packet_t*
write_data_packet(packet_type_t packetType, data_packet_t* dataFrame, char* dataPacket,
				  app_config_struct_t* appConfigStruct, mqtt_response_handler_t mqttRspHandler)
{
	int tempResult;
    size_t tempStringSize;
	tempResult = data_frame_init(dataFrame);
	
	if(tempResult<0)
		return NULL;
	
	if(packetType == NORMAL_PACKET)
	{
		if(dataFrame->data == NULL)
			return NULL;
		
		dataFrame->totalSize = strlen(dataPacket);
		dataFrame->nextPos = dataFrame->totalSize-1;
		strncpy(dataFrame->data, dataPacket, strlen(dataPacket));
		dataFrame->mqtt_response_handler = NULL;
		return dataFrame;
	}
	if(packetType == MQTT_CONN_PACKET)
	{
		int i=0;
		if(dataFrame->data == NULL)
			return NULL;
		
			dataFrame->data[0]= 0x10;
			dataFrame->nextPos =2;
			tempStringSize = strlen(appConfigStruct->protocolName);
			dataFrame->data[dataFrame->nextPos++]= (tempStringSize>>8)&0xFF;
			//eat_trace("High byte of password length is %x: \n", dataFrame->data[dataFrame->nextPos-1]);
			dataFrame->data[dataFrame->nextPos++] = tempStringSize & 0xFF;
			//eat_trace("Low byte of password length is %x: \n", dataFrame->data[dataFrame->nextPos-1]);
			strncpy(&dataFrame->data[dataFrame->nextPos], appConfigStruct->protocolName, tempStringSize);
			//for(i=0; i<tempStringSize; i++)
				//eat_trace("Password is %c ", dataFrame->data[dataFrame->nextPos+i]);
			//eat_trace("\n");
			dataFrame->nextPos +=tempStringSize;
			dataFrame->data[dataFrame->nextPos++] = appConfigStruct->protocolType;
			//eat_trace("Protocol Type  is %x: \n", dataFrame->data[dataFrame->nextPos-1]);
			dataFrame->data[dataFrame->nextPos++] = appConfigStruct->connectFlag;
			//eat_trace("Connect Flag is %x: \n", dataFrame->data[dataFrame->nextPos-1]);
			dataFrame->data[dataFrame->nextPos++] = ((appConfigStruct->keepAliveDurationSec)>>8)&0xFF;
			//eat_trace("High byte of KeepAlive is %x: \n", dataFrame->data[dataFrame->nextPos-1]);
			dataFrame->data[dataFrame->nextPos++] = (appConfigStruct->keepAliveDurationSec)&0xFF;
			//eat_trace("Low byte of KeepAlive is %x: \n", dataFrame->data[dataFrame->nextPos-1]);
			
			//Writing Client ID to the MQTT PACKET
			dataFrame->data[dataFrame->nextPos++] = (((short int)strlen(appConfigStruct->clientID)>>8)&0XFF);
			//eat_trace("High byte of Client ID length is %x: \n", dataFrame->data[dataFrame->nextPos-1]);
			dataFrame->data[dataFrame->nextPos++] = (strlen(appConfigStruct->clientID)&0xFF);
			//eat_trace("Low of Client ID length is %x: \n", dataFrame->data[dataFrame->nextPos-1]);
			tempStringSize = strlen(appConfigStruct->clientID);
			strncpy(&dataFrame->data[dataFrame->nextPos], appConfigStruct->clientID, tempStringSize);
			//for(i=0; i<tempStringSize; i++)
				//eat_trace("Client ID is %c ", dataFrame->data[dataFrame->nextPos+i]);
			//eat_trace("\n");
			dataFrame->nextPos += tempStringSize;
			
					
			//Writing WILL TOPIC to the MQTT Packet
			if(appConfigStruct->connectFlag&WILL_FLAG)
			{
				dataFrame->data[dataFrame->nextPos++] = (((short int)strlen(appConfigStruct->willTopic)>>8)&0xFF);
				//eat_trace("High byte of WILL FLAG Length is %x: \n", dataFrame->data[dataFrame->nextPos-1]);
				dataFrame->data[dataFrame->nextPos++] = (strlen(appConfigStruct->willTopic)&0xFF);
				//eat_trace("Low byte of WILL FLAG Length is %x: \n", dataFrame->data[dataFrame->nextPos-1]);
				tempStringSize = strlen(appConfigStruct->willTopic);
				strncpy(&dataFrame->data[dataFrame->nextPos],appConfigStruct->willTopic, tempStringSize);
				//for(i=0; i<tempStringSize; i++)
				//eat_trace("Will Topic is %c ", dataFrame->data[dataFrame->nextPos+i]);
				//eat_trace("\n");
				dataFrame->nextPos += tempStringSize;
			}
			//Writing WILL MESSAGE to the MQTT PACKET
			if(appConfigStruct->connectFlag&WILL_RETAIN)
			{
				dataFrame->data[dataFrame->nextPos++] = (((short int)strlen(appConfigStruct->willMessage)>>8)&0XFF);
				//eat_trace("High byte of Will Retain Length is %x: \n", dataFrame->data[dataFrame->nextPos-1]);
				dataFrame->data[dataFrame->nextPos++] = (strlen(appConfigStruct->willMessage)&0XFF);
				//eat_trace("Low byte of WILL Retain Length is %x: \n", dataFrame->data[dataFrame->nextPos-1]);
				tempStringSize = strlen(appConfigStruct->willMessage);
				strncpy(&dataFrame->data[dataFrame->nextPos], appConfigStruct->willMessage, tempStringSize);
				//for(i=0; i<tempStringSize; i++)
				//eat_trace("Will Message is %c ", dataFrame->data[dataFrame->nextPos+i]);
				//eat_trace("\n");
				dataFrame->nextPos += tempStringSize;
			}
			
			//Writing UserName to the MQTT PACKET
			if(appConfigStruct->connectFlag&USERNAME_FLAG)
			{
				dataFrame->data[dataFrame->nextPos++] = (((short int)strlen(appConfigStruct->MqttUsername)>>8)&0XFF);
				//eat_trace("High byte of Username lenght is %x: \n", dataFrame->data[dataFrame->nextPos-1]);
				dataFrame->data[dataFrame->nextPos++] = (strlen(appConfigStruct->MqttUsername)&0xFF);
				//eat_trace("Low byte of Username Length is %x: \n", dataFrame->data[dataFrame->nextPos-1]);
				tempStringSize = strlen(appConfigStruct->MqttUsername);
				strncpy(&dataFrame->data[dataFrame->nextPos], appConfigStruct->MqttUsername, tempStringSize);
				//for(i=0; i<tempStringSize; i++)
				//eat_trace("User Name is %c ", dataFrame->data[dataFrame->nextPos+i]);
				//eat_trace("\n");
				dataFrame->nextPos += tempStringSize;
			}
			
			//Writing Password to the MQTT PACKET
			if(appConfigStruct->connectFlag&PASSWORD_FLAG)
			{
				dataFrame->data[dataFrame->nextPos++] = (((short int)strlen(appConfigStruct->MqttPassword)>>8)&0XFF);
				//eat_trace("High byte of Password Length is %x: \n", dataFrame->data[dataFrame->nextPos-1]);
				dataFrame->data[dataFrame->nextPos++] = (strlen(appConfigStruct->MqttPassword)&0xFF);
				//eat_trace("Low byte of Password Length is %x: \n", dataFrame->data[dataFrame->nextPos-1]);
				tempStringSize = strlen(appConfigStruct->MqttPassword);
				strncpy(&dataFrame->data[dataFrame->nextPos], appConfigStruct->MqttPassword, tempStringSize);
				//for(i=0; i<tempStringSize; i++)
				//eat_trace("Password is %c ", dataFrame->data[dataFrame->nextPos+i]);
				//eat_trace("\n");
				dataFrame->nextPos += tempStringSize;
			}
			
			//Forming the complete data Frame for MQTT PACKET
			dataFrame->data[1] = (unsigned char)(dataFrame->nextPos-2)&0xFF;
			//eat_trace("Remaining Length is %x: \n", dataFrame->data[1]);
			dataFrame->totalSize = dataFrame->nextPos+1;
			dataFrame->mqtt_response_handler = mqttRspHandler;
			//Debug Statement should be commented off after use.
			//eat_trace("The Data frame is :%s \n", dataFrame->data);
			//eat_trace("CONNECT FRAME Count:%d\n",dataFrame->nextPos);
			//eat_trace("\r\n");
			
			return dataFrame;

	} //END OF MQTT CONNECT FRAME
	if(packetType == MQTT_PUBLISH_PACKET)
	{
		unsigned char Temp = 0x30;
		dataFrame->nextPos =2;
		
		if(appConfigStruct->qosLevel == QosLevel1)
			Temp = Temp|QosLevel0;
		else if(appConfigStruct->qosLevel == QosLevel2)
			Temp = Temp|QosLevel1;
		
		//Check Retain Bit
		if(appConfigStruct->retain==1)
			Temp = Temp|RETAIN;
		//Check DUP bit
		if(appConfigStruct->dup==1)
			Temp = Temp|DUP;
		//Publis Topic is written to the Packet
		dataFrame->data[dataFrame->nextPos++] = (((short int)strlen(appConfigStruct->publishTopic)>>8)&0xFF);
		dataFrame->data[dataFrame->nextPos++] = (strlen(appConfigStruct->publishTopic)&0xFF);
		tempStringSize = strlen(appConfigStruct->publishTopic);
		strncpy(&dataFrame->data[dataFrame->nextPos],appConfigStruct->publishTopic, tempStringSize);
		dataFrame->nextPos += tempStringSize;
		
		
		//qosLevel written to the packet
		if(appConfigStruct->qosLevel == QosLevel1 || appConfigStruct->qosLevel == QosLevel2)
		{
			dataFrame->data[dataFrame->nextPos++] = 0x00;
			dataFrame->data[dataFrame->nextPos++] = 0x0A;
		}
		
		//Publish Data is written to the Packet
		tempStringSize = strlen(dataPacket);
		strncpy(&dataFrame->data[dataFrame->nextPos], dataPacket, tempStringSize);
		dataFrame->nextPos += tempStringSize;
		
		if(((dataFrame->nextPos-1) >=0) && ((dataFrame->nextPos-1)<=255))
		{
			dataFrame->data[1] = (dataFrame->nextPos-2)&0xFF;
		}
		else {
			eat_trace("Packet length is out of range\n");
			return NULL;
		}
		    dataFrame->totalSize = dataFrame->nextPos+1;
			dataFrame->mqtt_response_handler = mqttRspHandler;
			//Debug Statement should be commented off after use.
			eat_trace("The Data frame is :%s \n", dataFrame->data);
			eat_trace("PUBLISH FRAME Count:%d\n",dataFrame->nextPos);
			eat_trace("\r\n");
		return dataFrame;
	} //END OF MQTT PUBLISH PACKET
	
	
	if(packetType == MQTT_SUBSCRIBE_PACKET)
	{
		unsigned char Temp=0;
		dataFrame->data[0] = 0x82;
		dataFrame->nextPos = 2;
		
		//Writing Packet ID to the MQTT Packet
		dataFrame->data[dataFrame->nextPos++] = (((short int)appConfigStruct->packetID>>8)&0XFF);
		dataFrame->data[dataFrame->nextPos++] = appConfigStruct->packetID & 0xFF;
		
		//Writing length of Topic to the MQTT Packet
		dataFrame->data[dataFrame->nextPos++] = (((short int)strlen(appConfigStruct->subscribeTopic)>>8)&0xFF);
		dataFrame->data[dataFrame->nextPos++] = (strlen(appConfigStruct->subscribeTopic)&0xFF);
		
		//Write Subscribe Topic to the MQTT Packet
		tempStringSize = strlen(appConfigStruct->subscribeTopic);
		strncpy(&dataFrame->data[dataFrame->nextPos],appConfigStruct->subscribeTopic, tempStringSize);
		dataFrame->nextPos += tempStringSize;
		
		if(appConfigStruct->qosLevel == QosLevel1)
			Temp = Temp|SUBQoSLEVEL0;
		else if(appConfigStruct->qosLevel == QosLevel2)
			Temp = Temp|SUBQoSLEVEL1;
		
		//qosLevel written if the qosLevel is greater than 0
		dataFrame->data[dataFrame->nextPos] = Temp;
		
		dataFrame->data[1] = (dataFrame->nextPos-2)&0xFF;
		
		dataFrame->totalSize = dataFrame->nextPos+1;
		dataFrame->mqtt_response_handler = mqttRspHandler;
		//Debug Statement should be commented off after use.
		eat_trace("The Data frame is :%s \n", dataFrame->data);
		eat_trace("SUBSCRIBE FRAME Count:%d\n",dataFrame->nextPos);
		eat_trace("\r\n");
		return dataFrame;
	} //END OF MQTT SUBSCRIBE PACKET
	
	
	if(packetType == MQTT_PING_PACKET)
	{
		dataFrame->data[dataFrame->nextPos++] = 0xC0;
		dataFrame->data[dataFrame->nextPos] = 0x00;
		
		dataFrame->totalSize = 2;
		dataFrame->mqtt_response_handler = mqttRspHandler;
		
		//Debug Statements can be commented off after testing
		eat_trace("The Data frame is :%s \n", dataFrame->data);
		eat_trace("PING FRAME Count:%d\n",dataFrame->nextPos);
		eat_trace("\r\n");
		return dataFrame;
	} //END OF MQTT PING PACKET
	if(packetType == MQTT_UNSUBSCRIBE_PACKET)
	{
		dataFrame->data[0] = 0xA2;
		dataFrame->nextPos = 2;
		
		//Writing packet ID for the packet
		dataFrame->data[dataFrame->nextPos++] = (((short int)appConfigStruct->packetID>>8)&0xFF);
		dataFrame->data[dataFrame->nextPos++] = appConfigStruct->packetID & 0xFF;
		
		//Writing lenght of the unsubcribe topic
		dataFrame->data[dataFrame->nextPos++] = (((short int)strlen(appConfigStruct->subscribeTopic)>>8)&0xFF);
		dataFrame->data[dataFrame->nextPos++] = (strlen(appConfigStruct->subscribeTopic)&0xFF);
		
		//Writing Topic to the Frame
		tempStringSize = strlen(appConfigStruct->subscribeTopic);
		strncpy(&dataFrame->data[dataFrame->nextPos],appConfigStruct->subscribeTopic, tempStringSize);
		dataFrame->nextPos += tempStringSize;
		
		dataFrame->data[1] = (dataFrame->nextPos-2)&0xFF;
		dataFrame->totalSize = dataFrame->nextPos;
		dataFrame->mqtt_response_handler = mqttRspHandler;
		
		//Debug Statements can be commented off after testing
		eat_trace("The Data frame is :%s \n", dataFrame->data);
		eat_trace("UNSUBSCRIBE FRAME Count:%d\n",dataFrame->nextPos);
		eat_trace("\r\n");
		return dataFrame;
	} //END OF MQTT UNSUBSCRIBE PACKET
	if(packetType == MQTT_DISCONN_PACKET)
	{
		dataFrame->data[dataFrame->nextPos++] = 0xE0;
		dataFrame->data[dataFrame->nextPos] = 0x00;
		
		dataFrame->totalSize = 2;
		dataFrame->mqtt_response_handler = NULL;
		
		//Debug Statements can be commented off after testing.
		eat_trace("The Data frame is :%s \n", dataFrame->data);
		eat_trace("DISCONNECT FRAME Count:%d\n",dataFrame->nextPos);
		eat_trace("\r\n");
		return dataFrame;
	} //MQTT_DISCONN_PACKET
	
	
}

void 
set_mqtt_user_pass(app_config_struct_t* appConfigStruct, char* userName  ,char* passWord)
{
	size_t tempStringSize=0;
	tempStringSize = strlen(userName);
	strncpy(appConfigStruct->MqttUsername, userName, tempStringSize);
	tempStringSize = strlen(passWord);
	strncpy(appConfigStruct->MqttPassword, passWord, tempStringSize);
	
	//Debug Statement
	eat_trace("The set MQTT UserName is %s\n",appConfigStruct->MqttUsername);
	eat_trace("The set MQTT Password is %s\n",appConfigStruct->MqttPassword);
}

void 
set_mqtt_connectflag(app_config_struct_t* appConfigStruct, unsigned char connectFlag )
{
	appConfigStruct->connectFlag = connectFlag;
	//Debug Statement
	eat_trace("The set connect Flag is is %x\n",appConfigStruct->connectFlag);
}

void 
set_mqtt_client_id(app_config_struct_t* appConfigStruct, char* clientID )
{
	size_t tempStringSize;
	tempStringSize = strlen(clientID);
	strncpy(appConfigStruct->clientID, clientID, tempStringSize);
	//Debug Statement
	eat_trace("The set client ID is %s\n",appConfigStruct->clientID);
}

void 
set_mqtt_will_topic(app_config_struct_t* appConfigStruct, char* willTopic )
{
	size_t tempStringSize;
	tempStringSize = strlen(willTopic);
	strncpy(appConfigStruct->willTopic, willTopic, tempStringSize);
	//Debug Statement
	eat_trace("The set willTopic is %s\n",appConfigStruct->willTopic);
}

void 
set_mqtt_will_message(app_config_struct_t* appConfigStruct, char* willMessage)
{
	size_t tempStringSize;
	tempStringSize = strlen(willMessage);
	strncpy(appConfigStruct->willMessage, willMessage, tempStringSize);
	//Debug Statement
	eat_trace("The set willMessage is %s\n",appConfigStruct->willMessage);
	
}

void 
set_mqtt_protocol_name(app_config_struct_t* appConfigStruct, char* protocolName )
{
	size_t tempStringSize;
	tempStringSize = strlen(protocolName);
	strncpy(appConfigStruct->protocolName, protocolName, tempStringSize);
	//Debug Statement
	eat_trace("The set protocol Name is %s\n",appConfigStruct->protocolName);

}

void 
set_mqtt_protocol_type(app_config_struct_t* appConfigStruct, unsigned char protocolType )
{
	appConfigStruct->protocolType = protocolType;
	//Debug Statement
	eat_trace("The set protocol Type is %c\n",appConfigStruct->protocolType);
}


void 
set_publish_topic(app_config_struct_t* appConfigStruct, char* publishTopic)
{
	size_t tempStringSize;
	tempStringSize = strlen(publishTopic);
	strncpy(appConfigStruct->publishTopic, publishTopic, tempStringSize);
	//Debug Statement
	eat_trace("The set publish Topic is %s\n",appConfigStruct->publishTopic);
}

void 
set_subscribe_topic(app_config_struct_t* appConfigStruct, char* subscribeTopic)
{
	size_t tempStringSize;
	tempStringSize = strlen(subscribeTopic);
	strncpy(appConfigStruct->subscribeTopic, subscribeTopic, tempStringSize);
	//Debug Statement
	eat_trace("The set subscribe Topic is %s\n",appConfigStruct->subscribeTopic);
}

void 
set_keepalive_duration(app_config_struct_t* appConfigStruct, unsigned short keepAliveDuration)
{
	appConfigStruct->keepAliveDurationSec = keepAliveDuration;
	//Debug Statement
	eat_trace("The set keep Alive Duration is %d\n",appConfigStruct->keepAliveDurationSec);
}


void 
set_qoslevel(app_config_struct_t* appConfigStruct, QosLevel qosLevel)
{
	appConfigStruct->qosLevel = qosLevel;
	//Debug Statement
	eat_trace("The set qosLevel is %d\n", appConfigStruct->qosLevel);
}

void 
set_dup_retain_flag(app_config_struct_t* appConfigStruct, int dupFlag, int retainFlag)
{
	appConfigStruct->dup = dupFlag;
	appConfigStruct->retain = retainFlag;
	
	//Debug Statement
	eat_trace("The set dup and retain value is %d & %d", appConfigStruct->dup, appConfigStruct->retain);
	
}

void 
set_packet_id(app_config_struct_t* appConfigStruct, unsigned short packetID)
{
	appConfigStruct->packetID = packetID;
	
	//Debug Statement
	eat_trace("The set up packet ID is %d", appConfigStruct->packetID);
}

//WRITE FUNCTIONS TO SET THE QOS LEVEL, DUP, RETAIN AND PACKET ID
