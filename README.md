# Keypad_Doorlock
Here we are creating a digital door lock system

<h3> The keypad doorlock have following features: </h3>

<p>
<ul>
<li>Toggle Solenoid lock based on correct password entered or not</li>
<li>Changing password('A' on Keypad) if required, only if correct password is entered<br>Using EEPROM to store the password(changed), so that even if the power is switched off, password is not lost</li>
<li>Backspace('B' on keypad) if user want to delete the entered password</li>
<li>If maximum number of attemps exceeded, then a 40 sec countdown locks the system</li>
</ul>
</p>

<h3>Future features to be added: <h3>
<p>
<ul>
<li>Adding admin mode('C' on keypad). Even if someone changes password the admin password is always valid</li>
<li>With the help of sensors, buzz for tempering </li>
<li>Help button('D' on keypad) OR 'D' can be used to make password visible for a while</li>
<li>Showing '*' in LCD display instread of actual digits</li>
</ul>
</p>
