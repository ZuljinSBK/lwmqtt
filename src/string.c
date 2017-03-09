#include <string.h>

#include "string.h"

/**
 * Return the length of the MQTTstring - C string if there is one, otherwise the length delimited string
 * @param mqttstring the string to return the length of
 * @return the length of the string
 */
int lwmqtt_strlen(lwmqtt_string_t mqttstring) {
  int rc = 0;

  if (mqttstring.cstring)
    rc = strlen(mqttstring.cstring);
  else
    rc = mqttstring.lenstring.len;
  return rc;
}

/**
 * Compares an MQTTString to a C string
 * @param a the MQTTString to compare
 * @param bptr the C string to compare
 * @return boolean - equal or not
 */
int lwmqtt_strcmp(lwmqtt_string_t *a, char *bptr) {
  int alen = 0, blen = 0;
  char *aptr;

  if (a->cstring) {
    aptr = a->cstring;
    alen = strlen(a->cstring);
  } else {
    aptr = a->lenstring.data;
    alen = a->lenstring.len;
  }
  blen = strlen(bptr);

  return (alen == blen) && (strncmp(aptr, bptr, alen) == 0);
}

/**
 * Writes a "UTF" string to an output buffer.  Converts C string to length-delimited.
 * @param pptr pointer to the output buffer - incremented by the number of bytes used & returned
 * @param string the C string to write
 */
void lwmqtt_write_c_string(unsigned char **pptr, const char *string) {
  int len = strlen(string);
  lwmqtt_write_int(pptr, len);
  memcpy(*pptr, string, len);
  *pptr += len;
}

void lwmqtt_write_string(unsigned char **pptr, lwmqtt_string_t mqttstring) {
  if (mqttstring.lenstring.len > 0) {
    lwmqtt_write_int(pptr, mqttstring.lenstring.len);
    memcpy(*pptr, mqttstring.lenstring.data, mqttstring.lenstring.len);
    *pptr += mqttstring.lenstring.len;
  } else if (mqttstring.cstring)
    lwmqtt_write_c_string(pptr, mqttstring.cstring);
  else
    lwmqtt_write_int(pptr, 0);
}

/**
 * @param mqttstring the MQTTString structure into which the data is to be read
 * @param pptr pointer to the output buffer - incremented by the number of bytes used & returned
 * @param enddata pointer to the end of the data: do not read beyond
 * @return 1 if successful, 0 if not
 */
int lwmqtt_read_lp_string(lwmqtt_string_t *mqttstring, unsigned char **pptr, unsigned char *enddata) {
  int rc = 0;

  /* the first two bytes are the length of the string */
  if (enddata - (*pptr) > 1) /* enough length to read the integer? */
  {
    mqttstring->lenstring.len = lwmqtt_read_int(pptr); /* increments pptr to point past length */
    if (&(*pptr)[mqttstring->lenstring.len] <= enddata) {
      mqttstring->lenstring.data = (char *)*pptr;
      *pptr += mqttstring->lenstring.len;
      rc = 1;
    }
  }
  mqttstring->cstring = NULL;

  return rc;
}