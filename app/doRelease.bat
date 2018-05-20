jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1 -keystore ../../txirrina.keystore platforms\android\app\build\outputs\apk\release\app-release-unsigned.apk txirrina
mv platforms\android\app\build\outputs\apk\release\Txirrina.apk platforms\android\app\build\outputs\apk\release\Txirrina.apk.old -f
d:\Android\sdk\build-tools\27.0.3\zipalign.exe -v 4 platforms\android\app\build\outputs\apk\release\app-release-unsigned.apk platforms\android\app\build\outputs\apk\release\Txirrina.apk
