<h2>nanomsg.node: nanomsg node.js bindings</h2><br>
<h3>License: Beerware</h3>
<p>example usage:</p><br>
<code>
var nn = require('nanomsg.node')<br>
  , pub = nn.socket(nn.AF_SP, nn.NN_PUB)<br>
  , sub = nn.socket(nn.AF_SP, nn.NN_SUB);<br>
<br>
nn.bind(pub, "tcp://127.0.0.1:7788");<br>
nn.connect(sub, "tcp://127.0.0.1:7788");<br>
nn.usleep(4000); // 4 ms<br>
<br>
nn.send(pub, "Hello from nanomsg!");<br>
<br>
console.log("received: " + nn.recv(sub));<br>
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
//  SP address families.<br>
AF_SP, AF_SP_RAW<br>

//  Socket option levels: Negative numbers are reserved for transports,<br>
//    positive for socket types.<br>
NN_SOL_SOCKET<br>
<br>
//  Generic socket options (NN_SOL_SOCKET level).<br>
NODE_DEFINE_CONSTANT(target, );<br>
NN_LINGER, NN_SNDBUF, NN_RCVBUF, NN_SNDTIMEO<br>
NN_RCVTIMEO, NN_RECONNECT_IVL, NN_RECONNECT_IVL_MAX<br>
NN_SNDPRIO, NN_SNDFD, NN_RCVFD, NN_DOMAIN, NN_PROTOCOL<br>
NN_IPV4ONLY<br>
<br>
//  Send/recv options.<br>
NODE_DEFINE_CONSTANT(target, NN_DONTWAIT);<br>
<br>
//  Socket types<br>
NN_REQ, NN_REP<br>
NN_PAIR<br>
NN_PUSH, NN_PULL<br>
NN_PUB, NN_SUB<br>
NN_BUS<br>
<br>
