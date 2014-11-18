Wiegand
=======

A C++ Wiegand decoding library, targeting the Arduino platform.

This library was developed after searching to find a suitable Arduino Wiegand library and being unsatisfied with the general results of the search.  As typically happens in such a situation, I proceeded to create a solution that meets my needs.

Motivation
----------

Many projects documented on the internet are only planning to see a very small number of RFID cards (less than 10, typically), leading to a complete lack of need to worry about properly decoding the parity information.  Basically, with a very small pool of possible correct bit sequences, a bit error will just be discarded as a non-authorized token.  In my case, I needed to deal with a very large number of possible users (several thousand), so I was not willing to compromise data integrity at the risk of getting the user token incorrect.

After writing the initial library basics, the code was put into use *without* the parity checking, to be updated later.  Fortunately, the interrupt based method for triggering the bit sequences was very reliable, even with a lot of other application code running on my Arduino.  As such, I have not felt the need to revisit this code and add the parity checking (nor to excise the rather hackish 64-bit integer manipulation) after year and nearly 4 months of reliable service.

Similar Projects
----------------

* [Pagemac Wiegand for Arduino](http://www.pagemac.com/azure/arduino_wiegand.php): This source provided my best inspiration, although I couldn't get over the idea of using up an entire byte in the buffer for each incoming bit.  Daniel's [HID RFID Data Formats](http://www.pagemac.com/azure/data_formats.php) page is a very informative resource with some great infographics to describe the format.

* [MonkeyBoard Wiegand Converter](http://www.monkeyboard.org/tutorials/82-protocol/24-wiegand-converter) ( [GitHub](https://github.com/monkeyboard/Wiegand-Protocol-Library-for-Arduino) ): Also a good description of how it works, although some of the writeup is specific to thier hardware.  Uses a non-RFID reading wiegand device, demonstrating other ways Wiegand can be used.

* [Francesco Uggetti's Arduino Mega Multi-Wiegand Library](http://uggettif.blogspot.com/2014/01/access-control-with-arduino-mega.html) ( [GitHub](https://github.com/ugge75/Wiegand-Protocol-Library-for-Arduino-MEGA-2560) ):  This library is expanded to be able to use multiple readers on the Mega.  My library would need some small edits to do this.

