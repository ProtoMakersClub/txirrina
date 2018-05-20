var express = require('express');
var app = express();
var http = require('http');
var https = require('https');
var cors = require('cors');

var config = require('../server.config.js');

var Particle = require('particle-api-js');
var particle = new Particle();
var token;
console.log(config);
particle.login({username: config.particleUsername, password: config.particlePassword}).then(
  function(data) {
    token = data.body.access_token;
    console.log(token);
  },
  function (err) {
    console.log('Could not log in.', err);
  }
);

app.use(cors({origin:"http://192.168.1.34:8000"}));
app.use(cors({origin:"http://localhost:5000"}));

app.get('/ping',function (req, res) {
  var publishEventPr = particle.publishEvent({ name: 'test', data: {}, auth: token });

publishEventPr.then(
  function(data) {
    if (data.body.ok) { console.log("Event published succesfully") }
  },
  function(err) {
    console.log("Failed to publish event: " + err)
  }
);
  res.json({
    success:true
  });
});
app.get('/ireki', function (req, res) {
  http.get('http://192.168.1.31/ireki',function(data){
    res.json({
      success:true,
      data:data
    });

    });


});
app.get('/ireki-photon', function (req, res) {
  var fnPr = particle.callFunction({ deviceId: config.particleDevice, name: config.particleFunc, argument: 'ireki', auth: token });

    fnPr.then(
    function(data) {
      res.json({
        success:true,
        data:data
      });
      console.log('Function called succesfully:', data);
    }, function(err) {
      res.json({
        success:false,
        data:err
      });
      console.log('An error occurred:', err);
    });


});

// app.post('/login')

app.listen(5001, function () {
  console.log('Example app listening on port 5001!');
});
