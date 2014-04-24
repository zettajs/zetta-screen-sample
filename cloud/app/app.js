var ScreenApp = module.exports = function() {
  this.name = 'screen-app';
};

ScreenApp.prototype.init = function(zetta) {
  zetta.observe('type="screen"')
    .subscribe(function(screen) {
      zetta.expose(screen);
    });
};
