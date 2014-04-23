var readline = require('readline');
var SerialPort = require('serialport').SerialPort;

var rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});


var serialPort = new SerialPort('/dev/tty.usbmodem1411', { baudRate: 9600 });

var ask = function() {
  rl.question('> ', function(answer) {
    serialPort.write(answer + '\n', function() {
      ask();
    });
  });
};

serialPort.on('open', function() {
  setTimeout(function() {
    ask();
  }, 5000);
});
