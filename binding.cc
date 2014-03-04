#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <node.h>
#include <node_version.h>
#include <node_buffer.h>
#include <uv.h>
#include <v8.h>
#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>
#include <nanomsg/pipeline.h>
#include <nanomsg/bus.h>
#include <nanomsg/pair.h>
#include <nanomsg/reqrep.h>

using namespace v8;

Handle<Value> Sleep(const Arguments& args) {
  HandleScope scope;

  if(args.Length() != 1)
  {
    ThrowException(Exception::TypeError(String::New("Must pass a variable")));
    return scope.Close(Undefined());
  }

  if(!args[0]->IsNumber())
  {
    ThrowException(Exception::TypeError(String::New("Must pass a integer value")));
    return scope.Close(Undefined());
  }

  int seconds = args[0]->Uint32Value();
  sleep(seconds);

  return scope.Close( Local<Value>::New(Integer::New(seconds)) );
}

Handle<Value> USleep(const Arguments& args) {
  HandleScope scope;

  if(args.Length() != 1)
  {
    ThrowException(Exception::TypeError(String::New("Must pass a variable")));
    return scope.Close(Undefined());
  }

  if(!args[0]->IsNumber())
  {
    ThrowException(Exception::TypeError(String::New("Must pass a integer value")));
    return scope.Close(Undefined());
  }

  int nanoseconds = args[0]->Uint32Value();
  usleep(nanoseconds);

  return scope.Close( Local<Value>::New(Integer::New(nanoseconds)) );
}

Handle<Value> Socket(const Arguments& args) {
  HandleScope scope;

  if(args.Length() != 2)
  {
    ThrowException(Exception::TypeError(String::New("Must pass two variables")));
    return scope.Close(Undefined());
  }

  if(!args[0]->IsNumber() || !args[1]->IsNumber())
  {
    ThrowException(Exception::TypeError(String::New("Must pass two integer values")));
    return scope.Close(Undefined());
  }

  int type = args[1]->Int32Value();

  int ret = nn_socket(args[0]->Int32Value(), type);
  if(type == NN_SUB)
    assert(nn_setsockopt (ret, NN_SUB, NN_SUB_SUBSCRIBE, "", 0) == 0);

  return scope.Close( Local<Value>::New(Integer::New(ret)) );
}

Handle<Value> Close(const Arguments& args) {
  HandleScope scope;

  if(args.Length() != 1)
  {
    ThrowException(Exception::TypeError(String::New("Must pass one variable")));
    return scope.Close(Undefined());
  }

  if(!args[0]->IsNumber())
  {
    ThrowException(Exception::TypeError(String::New("Must pass [integer]")));
    return scope.Close(Undefined());
  }

  int sp = args[1]->Int32Value();

  int ret = nn_close(sp);

  return scope.Close( Local<Value>::New(Integer::New(ret)) );
}

Handle<Value> SetSockOpt(const Arguments& args) {
  HandleScope scope;

  if(args.Length() != 4)
  {
    ThrowException(Exception::TypeError(String::New("Must pass four variables")));
    return scope.Close(Undefined());
  }

  if(!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()
    || !args[3]->IsNumber())
  {
    ThrowException(Exception::TypeError(String::New("Must pass [integer, integer, integer, integer]")));
    return scope.Close(Undefined());
  }

  int sock = args[0]->Int32Value();
  int level = args[1]->Int32Value();
  int option = args[2]->Int32Value();
  int optval = args[3]->Int32Value();

  int rc = nn_setsockopt (sock, level, option, (const void *)&optval, sizeof(optval));

  return scope.Close( Local<Value>::New(Integer::New(rc)) );
}

Handle<Value> GetSockOpt(const Arguments& args) {
  HandleScope scope;

  if(args.Length() != 3)
  {
    ThrowException(Exception::TypeError(String::New("Must pass three variables")));
    return scope.Close(Undefined());
  }

  if(!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber())
  {
    ThrowException(Exception::TypeError(String::New("Must pass [integer, integer, integer]")));
    return scope.Close(Undefined());
  }

  int sock = args[0]->Int32Value();
  int level = args[1]->Int32Value();
  int option = args[2]->Int32Value();

  int optval;
  size_t optvallen;
  nn_getsockopt(sock, level, option, (void *)&optval, &optvallen);

  return scope.Close( Local<Value>::New(Integer::New(optval)) );
}


Handle<Value> Bind(const Arguments& args) {
  HandleScope scope;

  if(args.Length() != 2)
  {
    ThrowException(Exception::TypeError(String::New("Must pass two variables")));
    return scope.Close(Undefined());
  }

  if(!args[0]->IsNumber() || !args[1]->IsString())
  {
    ThrowException(Exception::TypeError(String::New("Must pass [integer string]")));
    return scope.Close(Undefined());
  }

  String::Utf8Value address(args[1]);

  int ret = nn_bind(args[0]->Int32Value(), *address);

  return scope.Close( Local<Value>::New(Integer::New(ret)) );
}

Handle<Value> Connect(const Arguments& args) {
  HandleScope scope;

  if(args.Length() != 2)
  {
    ThrowException(Exception::TypeError(String::New("Must pass two variables")));
    return scope.Close(Undefined());
  }

  if(!args[0]->IsNumber() || !args[1]->IsString())
  {
    ThrowException(Exception::TypeError(String::New("Must pass [integer string]")));
    return scope.Close(Undefined());
  }

  String::Utf8Value address(args[1]);

  int ret = nn_connect(args[0]->Int32Value(), *address);

  return scope.Close( Local<Value>::New(Integer::New(ret)) );
}

Handle<Value> Send(const Arguments& args) {
  HandleScope scope;

  if(args.Length() != 2)
  {
    ThrowException(Exception::TypeError(String::New("Must pass two variables")));
    return scope.Close(Undefined());
  }

  if(!args[0]->IsNumber() || !args[1]->IsString())
  {
    ThrowException(Exception::TypeError(String::New("Must pass [integer string]")));
    return scope.Close(Undefined());
  }

  String::Utf8Value message(args[1]);
  unsigned char *buf = (unsigned char *)nn_allocmsg (message.length(), 0);
  if(buf == NULL) // Out Of Memory
    return scope.Close( Local<Value>::New(Integer::New(-1)) );

  memcpy(buf, *message, message.length());

  int rc = nn_send (args[0]->Int32Value(), &buf, NN_MSG, 0);

  return scope.Close( Local<Value>::New(Integer::New(rc)) );
}

Handle<Value> Recv(const Arguments& args) {
  HandleScope scope;

  if(args.Length() != 1)
  {
    ThrowException(Exception::TypeError(String::New("Must pass one variable")));
    return scope.Close(Undefined());
  }

  if(!args[0]->IsNumber())
  {
    ThrowException(Exception::TypeError(String::New("Must pass [integer]")));
    return scope.Close(Undefined());
  }

  String::Utf8Value message(args[1]);
  unsigned char *buf;

  int len = nn_recv (args[0]->Int32Value(), &buf, NN_MSG, 0);

  return scope.Close( Local<Value>::New(String::New((const char *)buf, len)) );
}

Handle<Value> Errno(const Arguments& args) {
  HandleScope scope;

  if(args.Length() > 0)
  {
    ThrowException(Exception::TypeError(String::New("This function does not takes arguments")));
    return scope.Close(Undefined());
  }

  int err = nn_errno();

  return scope.Close( Local<Value>::New(Integer::New(err)) );
}

Handle<Value> StrErr(const Arguments& args) {
  HandleScope scope;

  if(args.Length() != 1)
  {
    ThrowException(Exception::TypeError(String::New("Must pass one variable")));
    return scope.Close(Undefined());
  }

  if(!args[0]->IsNumber())
  {
    ThrowException(Exception::TypeError(String::New("Must pass [integer]")));
    return scope.Close(Undefined());
  }

  int errnum = args[0]->Int32Value();

  const char *errstr = nn_strerror(errnum);

  return scope.Close( Local<Value>::New(String::New(errstr)) );
}


static void Init (Handle<Object> target) {

  //  Utility functions
  target->Set(String::NewSymbol("sleep"),
      FunctionTemplate::New(Sleep)->GetFunction());

  target->Set(String::NewSymbol("usleep"),
      FunctionTemplate::New(USleep)->GetFunction());

  //  Socket functions
  target->Set(String::NewSymbol("socket"),
      FunctionTemplate::New(Socket)->GetFunction());

  target->Set(String::NewSymbol("close"),
      FunctionTemplate::New(Close)->GetFunction());

  target->Set(String::NewSymbol("setsockopt"),
      FunctionTemplate::New(SetSockOpt)->GetFunction());

  target->Set(String::NewSymbol("getsockopt"),
      FunctionTemplate::New(GetSockOpt)->GetFunction());

  target->Set(String::NewSymbol("connect"),
      FunctionTemplate::New(Connect)->GetFunction());

  target->Set(String::NewSymbol("bind"),
      FunctionTemplate::New(Bind)->GetFunction());

  target->Set(String::NewSymbol("send"),
      FunctionTemplate::New(Send)->GetFunction());

  target->Set(String::NewSymbol("recv"),
      FunctionTemplate::New(Recv)->GetFunction());

  target->Set(String::NewSymbol("errno"),
      FunctionTemplate::New(Errno)->GetFunction());

  target->Set(String::NewSymbol("strerror"),
      FunctionTemplate::New(StrErr)->GetFunction());

  //  SP address families.
  NODE_DEFINE_CONSTANT(target, AF_SP);
  NODE_DEFINE_CONSTANT(target, AF_SP_RAW);

  //  Socket option levels: Negative numbers are reserved for transports,
  //    positive for socket types.
  NODE_DEFINE_CONSTANT(target, NN_SOL_SOCKET);

  //  Generic socket options (NN_SOL_SOCKET level).
  NODE_DEFINE_CONSTANT(target, NN_LINGER);
  NODE_DEFINE_CONSTANT(target, NN_SNDBUF);
  NODE_DEFINE_CONSTANT(target, NN_RCVBUF);
  NODE_DEFINE_CONSTANT(target, NN_SNDTIMEO);
  NODE_DEFINE_CONSTANT(target, NN_RCVTIMEO);
  NODE_DEFINE_CONSTANT(target, NN_RECONNECT_IVL);
  NODE_DEFINE_CONSTANT(target, NN_RECONNECT_IVL_MAX);
  NODE_DEFINE_CONSTANT(target, NN_SNDPRIO);
  NODE_DEFINE_CONSTANT(target, NN_SNDFD);
  NODE_DEFINE_CONSTANT(target, NN_RCVFD);
  NODE_DEFINE_CONSTANT(target, NN_DOMAIN);
  NODE_DEFINE_CONSTANT(target, NN_PROTOCOL);
  NODE_DEFINE_CONSTANT(target, NN_IPV4ONLY);

  //  Send/recv options.
  NODE_DEFINE_CONSTANT(target, NN_DONTWAIT);

  //  Socket types
  NODE_DEFINE_CONSTANT(target, NN_REQ);
  NODE_DEFINE_CONSTANT(target, NN_REP);
  NODE_DEFINE_CONSTANT(target, NN_PAIR);
  NODE_DEFINE_CONSTANT(target, NN_PUSH);
  NODE_DEFINE_CONSTANT(target, NN_PULL);
  NODE_DEFINE_CONSTANT(target, NN_PUB);
  NODE_DEFINE_CONSTANT(target, NN_SUB);
  NODE_DEFINE_CONSTANT(target, NN_BUS);
}

// Register the initialization function.
NODE_MODULE(nanomsg, Init)
