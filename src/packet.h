/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *    Xiang Rong - 442039 Add makefile to Embedded C client
 *******************************************************************************/

#ifndef LWMQTT_PACKET_H
#define LWMQTT_PACKET_H

enum errors { MQTTPACKET_BUFFER_TOO_SHORT = -2, MQTTPACKET_READ_ERROR = -1 };

enum msgTypes {
  CONNECT = 1,
  CONNACK,
  PUBLISH,
  PUBACK,
  PUBREC,
  PUBREL,
  PUBCOMP,
  SUBSCRIBE,
  SUBACK,
  UNSUBSCRIBE,
  UNSUBACK,
  PINGREQ,
  PINGRESP,
  DISCONNECT
};

/**
 * Bitfields for the MQTT header byte.
 */
typedef union {
  unsigned char byte; /**< the whole byte */
  struct {
    unsigned int retain : 1; /**< retained flag bit */
    unsigned int qos : 2;    /**< QoS value, 0, 1 or 2 */
    unsigned int dup : 1;    /**< DUP flag bit */
    unsigned int type : 4;   /**< message type nibble */
  } bits;
} lwmqtt_header_t;

int lwmqtt_serialize_ack(unsigned char *buf, int buflen, unsigned char type, unsigned char dup,
                         unsigned short packetid);
int lwmqtt_deserialize_ack(unsigned char *packettype, unsigned char *dup, unsigned short *packetid, unsigned char *buf,
                           int buflen);

int lwmqtt_packet_len(int rem_len);
int lwmqtt_packet_encode(unsigned char *buf, int length);
int lwmqtt_packet_decode(int (*getcharfn)(unsigned char *, int), int *value);
int lwmqtt_packet_decode_buf(unsigned char *buf, int *value);

int lwmqtt_read_int(unsigned char **pptr);
char lwmqtt_read_char(unsigned char **pptr);
void lwmqtt_write_char(unsigned char **pptr, char c);
void lwmqtt_write_int(unsigned char **pptr, int anInt);

#endif  // LWMQTT_PACKET_H