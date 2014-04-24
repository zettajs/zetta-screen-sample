var EventEmitter = require('events').EventEmitter;
var readline = require('readline');
var util = require('util');
var serialport = require('serialport');
var ScreenDriver = require('../drivers/screen_driver');

var ScreenScout = module.exports = function() {
  EventEmitter.call(this);
  this._serialPort = null;
  this.drivers = [];
};
util.inherits(ScreenScout, EventEmitter);

ScreenScout.prototype.init = function(next) {
  this._serialPort = new serialport.SerialPort('/dev/tty.usbmodem1411', {
    baudRate: 9600,
    parser: serialport.parsers.readline('\r')
  });

  var self = this;
  this._serialPort.on('open', function() {
    self._serialPort.on('data', function(data) {
      if (data === 'ready') {
        self.emit('discover', ScreenDriver, self._serialPort, 'touch-screen');
      }
    })
  });

  next();
};

/*var ask = function() {
  rl.question('> ', function(answer) {
    serialPort.write(answer + '\n', function() {
      ask();
    });
  });
};*/

