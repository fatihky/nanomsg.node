var nn = require('./build/Release/nanomsg')
  , pub = nn.socket(nn.AF_SP, nn.NN_PUB)
  , sub = nn.socket(nn.AF_SP, nn.NN_SUB);

nn.bind(pub, "tcp://127.0.0.1:7788");
nn.connect(sub, "tcp://127.0.0.1:7788");
nn.usleep(4000); // 4 ms

nn.send(pub, "Hello from nanomsg!");

console.log("received: " + nn.recv(sub));
