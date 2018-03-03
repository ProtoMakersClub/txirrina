//document.addEventListener("deviceready", onDeviceReady, false);

const serverURL = "192.168.1.5";

function onDeviceReady() {
  console.log("DeviceReady triggered");
  setInterval(function () {
    $.ajax({
      url: serverURL + "/getStatus",
      type: 'GET',
      success: function(data){
        $("#connected").show();
        $("#disconnected").hide();
      },
      error: function(data) {
        $("#connected").hide();
        $("#disconnected").show();
      }
    });
  }, 1000);
}

$(function () {
  onDeviceReady();
})
