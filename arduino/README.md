JY-MCU-Game-of-life arduino code
===================

If you want to run this on your arduino I have 2 arduino libraries that you can use. Simply copy the 2 directories in the "libraries" directory to the "libraries" directory in your arduino installation directory and they will (after an IDE restart) be visible in the menu's. You can run the examples in each library or you can run the ht1632c_gameoflife project which combines the two libraries to the final project.

The source files of the arduino libraries and of the bare_code project are the same. I always change the bare_code ones, and then copy them to the arduino libraries. The sync_libraries.sh script is there to automate that.
