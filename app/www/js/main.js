//document.addEventListener("deviceready", onDeviceReady, false);

var serverURL;
const deiaTimeout = 3000;
function onDeviceReady() {

  console.log("DeviceReady triggered");
  $("#connected").hide();
  $("#disconnected").hide();

  $('.js-btn-ireki').addClass('tx-icon-key--disabled').attr('disabled',true).click(sendIreki);

  var notificationSoundEnabled = localStorage.getItem('notificationSoundEnabled');
  //console.log(notificationSoundEnabled);
  $('.js-sw-notification-sound').attr('checked',(notificationSoundEnabled === 'true')).click(handleSwitchNotificationSound);
  //$('.js-sw-notification-sound').data('checked',notificationSoundEnabled).click(handleSwitchNotificationSound);

  var statusRef = firebase.database().ref('status');
  var deiakRef = firebase.database().ref('deiak').limitToLast(1);
  var fireConfigRef = firebase.database().ref('config');
  var fireConfig;


//console.log(fireConfigRef);
  fireConfigRef.once('value', function(data) {
      fireConfig = data.val();
      //console.log(fireConfig);
      serverURL = "http://" + fireConfig.localIp + ":" + fireConfig.localPort;
      $('.js-btn-ireki').removeClass('tx-icon-key--disabled').attr('disabled',false);
      //Hide loader
      $('#app-loader').addClass('hidden');
      //Config loaded, show app.
      $('#txirrina-app').removeClass('hidden');
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
           document.getElementById('app-loader').style.display = 'none';
         }
       },
       // Will use popup for IDP Providers sign-in flow instead of the default, redirect.
       signInFlow: 'redirect',
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
     $('#app-loader').hadClass('hidden');
   } else {
     $('.js-btn-logout').removeClass('hidden');
     $('#firebaseui-auth-container').addClass('hidden');

   }

 });
 statusRef.on('child_changed', function(data) {
   console.log(data.val());
 });
  deiakRef.on('child_added', function(data) {

     var to = data.val().to;

     if($('.nori').hasClass('firstTime')){
       $('.nori').removeClass('firstTime');
     } else {
       $('.nori').text(to).data('ts',data.val().ts).fadeIn();
       $('.tx-icon-bell').addClass('tx-icon-bell--ring animated shake').addClass('shake');
       setTimeout(function(){
         $('.nori').fadeOut();
         $('.tx-icon-bell').removeClass('tx-icon-bell--ring animated shake').removeClass('shake');;

       }, deiaTimeout);
       //console.log(localStorage.getItem('notificationSoundEnabled'));
       if(localStorage.getItem('notificationSoundEnabled') === "true"){
         var audio = new Audio('/sounds/doorbell.wav');
         audio.play();

       }

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
      err.or: function(data) {
        //$("#disconnected").removeClass('hidden');
        $("#disconnected").fadeIn();

      }
    });
  }, 1000);*/
}
function handleSwitchNotificationSound(ev){

  var value = $(ev.currentTarget).is(":checked");

  if(value === true){
    $('.tx-icon-bell').removeClass('tx-icon-bell--mute');

  } else {
    $('.tx-icon-bell').addClass('tx-icon-bell--mute');
    //var value = $('tx-icon-bell').is(":checked");
  }
  $('.tx-icon-bell').attr('checked',value);
  localStorage.setItem('notificationSoundEnabled',value);
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
      $('.tx-icon-key').addClass('tx-icon-key--ok');
      setTimeout(function(){
        $('.tx-icon-key').removeClass('tx-icon-key--ok');
        },2000);
    },
    error: function(data) {
      console.log(data);
      var data = {
        message: 'Error al abrir.',
        timeout: 2000,
        actionHandler: handler,
        actionText: 'Undo'
      };
      $('.tx-icon-key').addClass('tx-icon-key--error');
      setTimeout(function(){
        $('.tx-icon-key').removeClass('tx-icon-key--error');
        },2000);
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
