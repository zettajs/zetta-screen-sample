var readline = require('readline');
var serialport = require('serialport');
var SerialPort = serialport.SerialPort;

var rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});


var serialPort = new SerialPort('/dev/tty.usbmodem1411', {
  baudRate: 9600,
  parser: serialport.parsers.readline('\r')
});

var ask = function() {
  rl.question('> ', function(answer) {
    serialPort.write(answer + '\n', function() {
      ask();
    });
  });
};

serialPort.on('open', function() {
  serialPort.on('data', function(data) {
    if (data === 'ready') {
      ask();
    }
  })
});
