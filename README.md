# Door_Locker


[![Watch the video](https://i.imgur.com/y5taztC.png)](https://www.youtube.com/watch?v=jElXmJj1bMY&feature=youtu.be)


<h2>Desciption : </h2>
<p>Door Locker Security System consists of two ECU’s. The first ECU called HMI responsible for interfacing with the user and the second ECU called control ECU which is responsible for the system operations and control. In the project I implemented the following drivers Keypad, LCD, DC Motor, UART, Timer, I2C and External EEPROM.
</p>  
  
<h2>Door Locker Steps:</h2>
1-Please enter your password <br/>
2-Please Re-enter your password matched to move to step 3 not matched back to step 1<br/>
3-Main Caption: <br/>
  &nbsp;a- +Open Door<br/>
 &nbsp;b- -Change password <br/>
4-Open Door:<br/>
 &nbsp; a-if matched rotate motor waiting 60 min than close door.<br/>
&nbsp;  b-wrong password if 2 times alarm led will flashing for 60 min.<br/>
5-Change password go to step 1.
  
