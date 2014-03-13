
/******************************************************************************

  Filename:		webserver.h
  Description:	Webserver app for the WiShield 2.0

 ******************************************************************************

  TCP/IP stack and driver for the WiShield 2.0 wireless devices

  Copyright(c) 2009 Async Labs Inc. All rights reserved.

  This program is free software; you can redistribute it and/or modify it
  under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc., 59
  Temple Place - Suite 330, Boston, MA  02111-1307, USA.

   Author               Date        Comment
  ---------------------------------------------------------------
   AsyncLabs		05/29/2009  Initial version
   P. Ciuffetti		03/12/2014  Modified web page and added WiShield led control

 *****************************************************************************/

#include "uip.h"
#include <string.h>
#include "webserver.h"
#include "config.h"

static int handle_connection(struct webserver_state *s);

void webserver_init(void) {
  uip_listen(HTONS(80));
}

void webserver_appcall(void) {
  struct webserver_state *s = &(uip_conn->appstate);
  
  if(uip_connected()) {
    PSOCK_INIT(&s->p, s->inputbuf, sizeof(s->inputbuf));
  }
  
  handle_connection(s);
}

#define ISO_nl     0x0a
#define ISO_space  0x20
#define ISO_slash  0x2f
#define ISO_T      0x54

const char http_get[5] = {0x47, 0x45, 0x54, 0x20, }; // "GET "

unsigned short fill_buf(void* blk) {
  unsigned short webpage_len;
  
  webpage_len = (strlen_P(webpage)>uip_mss())?uip_mss():strlen_P(webpage);
  
  memcpy_P(uip_appdata, webpage, webpage_len);
  return webpage_len;
}

static int handle_connection(struct webserver_state *s) {
  PSOCK_BEGIN(&s->p);
  
  /* the incoming GET request will have the following format:
   * GET / HTTP/1.1 ....
   * we have to parse this string to determine the resource being requested
   * if the requested resource is not the root webpage ('/') then,
   * GET /<resource name> HTTP/1.1 ....
   * we should parse the specific resource and react appropriately
   */
  
  // read incoming data until we read a space character
  PSOCK_READTO(&s->p, ISO_space);
  
  // parse the data to determine if it was a GET request
  if(strncmp(s->inputbuf, http_get, 4) != 0) {
    PSOCK_CLOSE_EXIT(&s->p);
  }
  
  // continue reading until the next space character
  PSOCK_READTO(&s->p, ISO_space);
  
  // determine the requested resource
  // in this case, we check if the request was for the '/' root page
  // AKA index.html
  if(s->inputbuf[0] != ISO_slash) {
    // request for unknown webpage, close and exit
    PSOCK_CLOSE_EXIT(&s->p);
  }
  
  PSOCK_SEND_STR(&s->p, "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\r\n");
  PSOCK_SEND_STR(&s->p, "<HTML>\r\n<HEAD>\r\n\t<META http-equiv=\"Content-type\" content=\"text/html;charset=UTF-8\">\r\n\t<TITLE>Arduino WiShield 2.0</TITLE>\r\n");
  PSOCK_SEND_STR(&s->p, "\t<STYLE type=\"text/css\">\r\n\t\tP,H1 {text-align: center;}\r\n\t</STYLE>\r\n</HEAD>\r\n<BODY>\r\n");
  PSOCK_SEND_STR(&s->p, "\t<H1>Hello World!! I am an Arduino with a WiShield 2.0</H1>\r\n\t<P><A href=\"T\">Toggle LED</A></P>\r\n");
  
  if(s->inputbuf[1] != ISO_space) {
    // request for a resource
    if(s->inputbuf[1] != ISO_T) {
      // not supported
      PSOCK_SEND_STR(&s->p, "\t<P>Request not supported</P>\r\n");
    } else {
      // Toggle WiShield 2.0 Led
      PSOCK_SEND_STR(&s->p, "\t<P>TOGGLE LED command received</P>\r\n");
      digitalWrite(9, !digitalRead(9));
    }
  }
  
  PSOCK_SEND_STR(&s->p, "</BODY>\r\n</HTML>");
  PSOCK_CLOSE(&s->p);
  PSOCK_END(&s->p);
}
