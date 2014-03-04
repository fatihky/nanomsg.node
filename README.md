<h2>nanomsg.node: nanomsg node.js bindings</h2><br>
<br>
<h3>License: Beerware</h3><br>
<p>example usage:</p><br>
<code>
var nn = require('nanomsg.node')
  , pub = nn.socket(nn.AF_SP, nn.NN_PUB)
  , sub = nn.socket(nn.AF_SP, nn.NN_SUB);

nn.bind(pub, "tcp://127.0.0.1:7788");
nn.connect(sub, "tcp://127.0.0.1:7788");
nn.usleep(4000); // 4 ms

nn.send(pub, "Hello from nanomsg!");

console.log("received: " + nn.recv(sub));
</code>
<br>
<h3>Functions:</h3><br>
nn.sleep(seconds), sleep in seconds<br>
nn.usleep(microseconds), sleep in microseconds(1 second = 1000000 microseconds)<br>
nn.socket(sp_type, sock_type), get socket<br>
nn.close(socket), close socket<br>
nn.bind(socket, address): bind to address<br>
nn.connect(socket, address): connect to address<br>
nn.send(socket, message): send message to socket(message is binary safe)<br>
nn.recv(socket): recv message from socket(return binary safe message string)<br>
nn.errno(): get errno<br>
nn.strerror(errno): get error string<br>
nn.setsockopt(socket, level, option, optval): set socket option(all arguments must be integer)<br>
nn.getsockopt(socket, level, option): get socket option(all arguments must be integer)<br>
<br>
<h3>Constants:</h3><br>
//  SP address families.
AF_SP, AF_SP_RAW

//  Socket option levels: Negative numbers are reserved for transports,
//    positive for socket types.
NN_SOL_SOCKET

//  Generic socket options (NN_SOL_SOCKET level).
NODE_DEFINE_CONSTANT(target, );
NN_LINGER, NN_SNDBUF, NN_RCVBUF, NN_SNDTIMEO
NN_RCVTIMEO, NN_RECONNECT_IVL, NN_RECONNECT_IVL_MAX
NN_SNDPRIO, NN_SNDFD, NN_RCVFD, NN_DOMAIN, NN_PROTOCOL
NN_IPV4ONLY

//  Send/recv options.
NODE_DEFINE_CONSTANT(target, NN_DONTWAIT);

//  Socket types
NN_REQ, NN_REP
NN_PAIR
NN_PUSH, NN_PULL
NN_PUB, NN_SUB
NN_BUS
