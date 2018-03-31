//document.addEventListener("deviceready", onDeviceReady, false);

var serverURL;
const deiaTimeout = 3000;
function onDeviceReady() {

  console.log("DeviceReady triggered");
  $("#connected").hide();
  $("#disconnected").hide();
  $('.nori').hide();
  $('.js-btn-ireki').attr('disabled',true).click(sendIreki);
  var deiakRef = firebase.database().ref('deiak').limitToLast(1);
  var fireConfigRef = firebase.database().ref('config');
  var fireConfig;
//console.log(fireConfigRef);
  fireConfigRef.once('value', function(data) {
      fireConfig = data.val();
      //console.log(fireConfig);
      serverURL = "http://" + fireConfig.localIp + ":" + fireConfig.localPort;
      $('.js-btn-ireki').attr('disabled',false);
  });
  firebase.auth().onAuthStateChanged(function(user) {
   window.user = user; // user is undefined if no user signed in
   if(!user){
     var ui = new firebaseui.auth.AuthUI(firebase.auth());

     var uiConfig = {
       callbacks: {
         signInSuccess: function(currentUser, credential, redirectUrl) {
           // User successfully signed in.
           // Return type determines whether we continue the redirect automatically
           // or whether we leave that to developer to handle.
           return true;
         },
         uiShown: function() {
           // The widget is rendered.
           // Hide the loader.
           document.getElementById('loader').style.display = 'none';
         }
       },
       // Will use popup for IDP Providers sign-in flow instead of the default, redirect.
       signInFlow: 'popup',
       signInSuccessUrl: '/',
       signInOptions: [
         // Leave the lines as is for the providers you want to offer your users.
         firebase.auth.EmailAuthProvider.PROVIDER_ID,
       ],
       // Terms of service url.
       tosUrl: '/'
     };
     $('.js-btn-logout').addClass('hidden');
     ui.start('#firebaseui-auth-container', uiConfig);
   } else {
     $('.js-btn-logout').removeClass('hidden');
     $('#firebaseui-auth-container').addClass('hidden');
     $('#txirrina-app').removeClass('hidden');
   }
 });
  deiakRef.on('child_added', function(data) {

     var to = data.val().to;

     if($('.nori').hasClass('firstTime')){
       $('.nori').removeClass('firstTime');
     } else {
       $('.nori').text(to).data('ts',data.val().ts).fadeIn();
       setTimeout(function(){ $('.nori').fadeOut() }, deiaTimeout);
       console.log('DEIA');
       var audio = new Audio('/sounds/doorbell.wav');
       audio.play();
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
function signOut(){
  firebase.auth().signOut();
}
$(function () {
  onDeviceReady();

})
