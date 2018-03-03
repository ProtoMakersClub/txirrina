var express = require('express');
var app = express();

app.get('/getStatus', function (req, res) {
  res.json({
    success:true
  });
});

// app.post('/login')

app.listen(8000, function () {
  console.log('Example app listening on port 8000!');
});
