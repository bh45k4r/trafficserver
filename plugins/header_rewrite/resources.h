/*
  Licensed to the Apache Software Foundation (ASF) under one
  or more contributor license agreements.  See the NOTICE file
  distributed with this work for additional information
  regarding copyright ownership.  The ASF licenses this file
  to you under the Apache License, Version 2.0 (the
  "License"); you may not use this file except in compliance
  with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/
//////////////////////////////////////////////////////////////////////////////////////////////
//
// Implement the classes for the various types of hash keys we support.
//
#ifndef __RESOURCES_H__
#define __RESOURCES_H__ 1

#include <string>
#include <ts/ts.h>
#include <ts/remap.h>

#include "lulu.h"


#define TS_REMAP_PSEUDO_HOOK TS_HTTP_LAST_HOOK // Ugly, but use the "last hook" for remap instances.

enum ResourceIDs {
  RSRC_NONE = 0,
  RSRC_SERVER_RESPONSE_HEADERS = 1,
  RSRC_SERVER_REQUEST_HEADERS = 2,
  RSRC_CLIENT_REQUEST_HEADERS = 4,
  RSRC_CLIENT_RESPONSE_HEADERS = 8,
  RSRC_RESPONSE_STATUS = 16,
};


///////////////////////////////////////////////////////////////////////////////
// Resources holds the minimum resources required to process a request.
//
class Resources
{
public:
  explicit Resources(TSHttpTxn txnptr, TSCont contptr)
    : txnp(txnptr), contp(contptr), bufp(NULL), hdr_loc(NULL), client_bufp(NULL), client_hdr_loc(NULL),
      resp_status(TS_HTTP_STATUS_NONE), _rri(NULL), changed_url(false), _ready(false)
  {
    TSDebug(PLUGIN_NAME_DBG, "Calling CTOR for Resources (InkAPI)");
  }

  Resources(TSHttpTxn txnptr, TSRemapRequestInfo *rri) :
    txnp(txnptr), contp(NULL),
    bufp(NULL), hdr_loc(NULL), client_bufp(NULL), client_hdr_loc(NULL), resp_status(TS_HTTP_STATUS_NONE),
      _rri(rri), changed_url(false), _ready(false)
  {
    TSDebug(PLUGIN_NAME_DBG, "Calling CTOR for Resources (RemapAPI)");
    TSDebug(PLUGIN_NAME, "rri: %p", _rri);
  }

  ~Resources() { destroy(); }

  void gather(const ResourceIDs ids, TSHttpHookID hook);
  bool ready() const { return _ready; }

  TSHttpTxn txnp;
  TSCont contp;
  TSMBuffer bufp;
  TSMLoc hdr_loc;
  TSMBuffer client_bufp;
  TSMLoc client_hdr_loc;
  TSHttpStatus resp_status;
  TSRemapRequestInfo *_rri;
  bool changed_url;

private:
  void destroy();
  DISALLOW_COPY_AND_ASSIGN(Resources);

  bool _ready;
};


#endif // __RESOURCES_H
