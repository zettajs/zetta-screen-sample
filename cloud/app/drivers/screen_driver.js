var ScreenDriver = module.exports = function(serialPort, name) {
  this._serialPort = serialPort;
  this.type = 'screen';
  this.name = name;
  this.state = 'ready';
};

ScreenDriver.prototype.init = function(config) {
  config
    .when('ready', { allow: 'change' })
    .map('change', this.change, [ { name: 'text', type: 'text' } ]);
};

ScreenDriver.prototype.change = function(text, cb) {
  this._serialPort.write(text + '\n', cb);
};
