#ifndef __MQTT_PACKET_WRITER_H__
#define __MQTT_PACKET_WRITER_H__


//****************************Connection Packet Variables***********************************
#define USERNAME_FLAG  0x01<<7
#define PASSWORD_FLAG  0x01<<6
#define WILL_RETAIN    0x01<<5
#define WILL_QOS1      0x01<<4
#define WILL_QOS0      0x01<<3
#define WILL_FLAG      0x01<<2
#define CLEAN_SESSION  0x01<<1

//***************************Publish Packet Variables*************************************/
#define RETAIN    0x01<<0
#define QosLEVEL0 0x01<<1
#define QosLEVEL1 0x01<<2
#define DUP       0x01<<3

//*************************Subscribe Packet Variables***********************************/
#define SUBQoSLEVEL0 0x01<<0
#define SUBQoSLEVEL1 0x01<<1

#define SERVER_CONN_PACKET_SIZE 100
#define SERVER_PUB_PACKET_SIZE	512
#define SERVER_SUB_PACKET_SIZE	100
#define SERVER_PING_PACKET_SIZE	50
#define SERVER_UNSUB_PACKET_SIZE 100
#define SERVER_DISCONN_PACKET_SIZE 100

#define CONN_ACK_FRAME_SIZE		100
#define PING_ACK_FRAME_SIZE		50
#define PUB_ACK_FRAME_SIZE		100
#define SUB_ACK_FRAME_SIZE		256
#define UNSUB_ACK_FRAME_SIZE	100
#define DISCONN_ACK_FRAME_SIZE	100

typedef enum{
 QosLevel0,
 QosLevel1,
 QosLevel2
}QosLevel;

typedef enum{
	NORMAL_PACKET=0,
	MQTT_CONN_PACKET=1,
	MQTT_PUBLISH_PACKET=2,
	MQTT_SUBSCRIBE_PACKET =3,
	MQTT_PING_PACKET=4,
	MQTT_UNSUBSCRIBE_PACKET=5,
	MQTT_DISCONN_PACKET=6
}packet_type_t;

typedef int(*mqtt_response_handler_t)(char *buffer, unsigned short buffLen);

typedef struct data_packet_{
	char *data;				//Pointer to data Buffer where the data is to be stored
	int totalSize;			//total size of the data stored in the buffer
	int nextPos;				//next position of the buffer to be written
	mqtt_response_handler_t mqtt_response_handler;
}data_packet_t;



typedef struct app_config_struct_{
	char MqttUsername[20];
	char MqttPassword[20];
	unsigned char connectFlag;
	char clientID[15];
	char willTopic[20];
	char willMessage[20];
	char protocolName[10];
	unsigned char protocolType;
	char publishTopic[20];
	char subscribeTopic[20];
	QosLevel qosLevel;
	int retain;
	int dup;
	unsigned short packetID;
	unsigned short keepAliveDurationSec;
}app_config_struct_t;



/*==============================FUNCTION DECLARATIONS===================================================*/
/********************************************************************************************************
@function : void set_keepalive_duration(app_config_struct_t*, unsigned short )

@Description : This function is used to set the keep alive duration in seconds for mqtt connection.
@parameter	 :	app_config_struct_t* -> [in] -> This is address of the configuration structure used for storing 
												various parameters of the server
				unsigned short		      	 -> [in] -> this is the keepalive duration value to be set on mqtt server 
				 
@return			void 
**********************************************************************************************************/

void 
set_keepalive_duration(app_config_struct_t*, unsigned short );


/********************************************************************************************************
@function : void set_subscribe_topic(app_config_struct_t*, char* )

@Description : This function is used to set the topic to which the device will subscribe data from.
@parameter	 :	app_config_struct_t* -> [in] -> This is address of the configuration structure used for storing 
												various parameters of the server
				char*			-> [in] -> address of the subscribe topic to the MQTT server 
				 
@return			void 
**********************************************************************************************************/

void 
set_subscribe_topic(app_config_struct_t*, char* );


/********************************************************************************************************
@function : void set_publish_topic(app_config_struct_t*, char* )

@Description : This function is used to set the topic to which the device will publish data.
@parameter	 :	app_config_struct_t* -> [in] -> This is address of the configuration structure used for storing 
												various parameters of the server
				char*			-> [in] -> address of the publish topic to the MQTT server 
				 
@return			void 
**********************************************************************************************************/

void 
set_publish_topic(app_config_struct_t*, char* );


/********************************************************************************************************
@function : void set_mqtt_protocol_type(app_config_struct_t*, unsigned char )

@Description : This function is used to store the will Message if any to send in mqtt connect packet
@parameter	 :	app_config_struct_t* -> [in] -> This is address of the configuration structure used for storing 
												various parameters of the server
				unsigned char			-> [in] -> store protocol type to be send to the MQTT server the 	
												   the value is between 1 to 4				 
@return			void 
**********************************************************************************************************/

void 
set_mqtt_protocol_type(app_config_struct_t*, unsigned char );
/********************************************************************************************************
@function : void set_mqtt_protocol_name(app_config_struct_t*, char* )

@Description : This function is used to store the protocol name to send in mqtt connect packet
			   (This is dependent on the MQTT server used)	
@parameter	 :	app_config_struct_t* -> [in] -> This is address of the configuration structure used for storing 
												various parameters of the server
				char*			     -> [in] -> address of the protocol Name  if any to be send to the MQTT server 
				 
@return			void 
**********************************************************************************************************/

void 
set_mqtt_protocol_name(app_config_struct_t*, char* );

/********************************************************************************************************
@function : void set_mqtt_will_message(app_config_struct_t*, char* )

@Description : This function is used to store the will Message if any to send in mqtt connect packet
@parameter	 :	app_config_struct_t* -> [in] -> This is address of the configuration structure used for storing 
												various parameters of the server
				char*			     -> [in] -> store will Message if any to be send to the MQTT server 
				 
@return			void 
**********************************************************************************************************/

void 
set_mqtt_will_message(app_config_struct_t*, char* );
/********************************************************************************************************
@function : void set_mqtt_will_topic(app_config_struct_t*, char* )

@Description : This function is used to store the will Topic if any to send in mqtt connect packet
@parameter	 :	app_config_struct_t* -> [in] -> This is address of the configuration structure used for storing 
												various parameters of the server
				 char*			     -> [in] -> store will Topic if any to be send to the MQTT server 
				 
@return			void 
**********************************************************************************************************/

void 
set_mqtt_will_topic(app_config_struct_t*, char* );
/********************************************************************************************************
@function : void set_mqtt_client_id(app_config_struct_t*, char*)

@Description : This function is used to set the client ID of the device used for further server comm.
@parameter	 : app_config_struct_t* -> [in] -> This is address of the configuration structure used for storing 
												various parameters of the server
				char* 			    -> [in] -> pointer to client ID string to be stored in the device
					 
@return			void 
**********************************************************************************************************/

void 
set_mqtt_client_id(app_config_struct_t*, char* );

/********************************************************************************************************
@function : void set_mqtt_connectflag(app_config_struct_t*, unsigned char )

@Description : This function is used to set the connection flag used for Connection packet
@parameter	 :	app_config_struct_t* -> [in] -> This is address of the configuration structure used for storing 
												various parameters of the server
				unsigned char		-> [in] -> store connection flag to be used in connect packet of mqtt
					 
@return			void 
**********************************************************************************************************/

void 
set_mqtt_connectflag(app_config_struct_t*, unsigned char );

/********************************************************************************************************
@function : void set_mqtt_user_pass(app_config_struct_t*, const char* , const char* )

@Description : This Function sets username and password for the mqtt server used in connect packet
@parameter	 :	app_config_struct_t* -> [in] -> This is address of the configuration structure used for storing 
												various parameters of the server
				char *		  -> [in] -> address of the username to be used for the mqtt server
				char*		  -> [in] -> address of the password to be used for mqtt connect
@return			void 
**********************************************************************************************************/

void 
set_mqtt_user_pass(app_config_struct_t*, char* ,char* );

/********************************************************************************************************
@function : char* write_data_packet(packet_type_t, data_packet_t*, char* )

@Description : This Function initializes the data buffer and writes the data to the buffer as per the 
				packet type passed on and returns the address to the buffer on successful write
				else returns null;
@parameter	 :	packet_type_t -> [in] -> This is enum for the type of packet to be formed
				data_packet_t* -> [in] -> Address of the data structure for which the data to be serialiized
				char *		  -> [in] -> address of the Data Buffer from which the packet has to be written,
										 can pass NULL value in case of connect, ping or disconnect packet;
@return 		char*		  -> [out] -> returns the address of the data buffer on successful write
										  else returns NULL
**********************************************************************************************************/

data_packet_t*
write_data_packet(packet_type_t, data_packet_t*, char*, app_config_struct_t*,mqtt_response_handler_t);

/********************************************************************************************************
@function : int assign_static_buffer_to_mqtt_packet(char*, data_packet_t*);

@Description : This Function allocates global static buffer to the mqtt packet to be written and processed.
@parameter	 :	
				data_packet_t* -> [in] -> Address of the data structure for which the data to be serialiized
				char *		  -> [in] -> address of the Data Buffer which would be used for further processing
										 of mqtt packets.
@return 		int		  -> [out] -> returns 0 in case of successful assignment else returns -1
**********************************************************************************************************/
int 
assign_static_buffer_to_mqtt_packet(char*, data_packet_t*);



#endif
