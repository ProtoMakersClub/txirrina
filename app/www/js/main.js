//document.addEventListener("deviceready", onDeviceReady, false);

var serverURL;
const deiaTimeout = 3000;
function onDeviceReady() {
  console.log("DeviceReady triggered");
  $("#connected").hide();
  $("#disconnected").hide();
  $('.nori').hide();
  $('.js-btn-ireki').click(sendIreki);
  var deiakRef = firebase.database().ref('deiak').limitToLast(1);
  var fireConfigRef = firebase.database().ref('config');
  var fireConfig;
  fireConfigRef.once('value', function(data) {
      fireConfig = data.val();
      serverURL = "http://" + fireConfig.localIp + ":" + fireConfig.localPort;
  });

  deiakRef.on('child_added', function(data) {

     var to = data.val().to;
     if($('.nori').hasClass('firstTime')){
       $('.nori').removeClass('firstTime');
     } else {
       $('.nori').text(to).data('ts',data.val().ts).fadeIn();
       setTimeout(function(){ $('.nori').fadeOut() }, deiaTimeout);
     }

   });

  /*setInterval(function () {
    $.ajax({
      url: serverURL + "/getStatus",
      type: 'GET',
      success: function(data){
        //$("#connected").removeClass('hidden');
        $("#connected").fadeIn();

        //$('.spinner').addClass('hidden');
      },
      error: function(data) {
        //$("#disconnected").removeClass('hidden');
        $("#disconnected").fadeIn();

      }
    });
  }, 1000);*/
}
function sendIreki(ev){
  var handler = function(event) {
    showSnackbarButton.style.backgroundColor = '';
  };
  var snackbarContainer = document.querySelector('#demo-snackbar-example');
  $.ajax({
    url: serverURL + "/ireki",
    type: 'GET',
    success: function(data){
      console.log(data);
      var data = {
        message: 'Abrir OK.',
        timeout: 2000,
        actionHandler: handler,
        actionText: 'Undo'
      };
      snackbarContainer.MaterialSnackbar.showSnackbar(data);
    },
    error: function(data) {
      console.log(data);
      var data = {
        message: 'Error al abrir.',
        timeout: 2000,
        actionHandler: handler,
        actionText: 'Undo'
      };

      snackbarContainer.MaterialSnackbar.showSnackbar(data);
    }
  });

}
$(function () {
  onDeviceReady();

})
