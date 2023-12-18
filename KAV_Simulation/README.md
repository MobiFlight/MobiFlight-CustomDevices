# Kav Simulations

## Custom Device MobiFlight Integration

This repository is the working directory for all files relating to the natural integeration of the Kav Simulations devices, with MobiFlight connector.

The root directory contains all the required files for the custom firmware, and the subdirectory, `connectory_files` is where all files required for the desktop software are.

- - -

This custom device type supports the FCU, EFIS, Battery, RAD/TCAS and Rudder displays from Kav Simulations.

In the desktop connectory software, simply create a MobiFlight module of type 'Kav Mega' (or pico, or whichever board you're using), and then choose the appropriate display from the device type list. Lastly, define the pins for Data, CS and CLK, and connect your display(s) accordingly.

<img src="https://cdn.shopify.com/s/files/1/0736/3588/9464/files/1_Board_Install.jpg?v=1702907003" alt="Install Step 1" width ="400"/>
<br />
<img src="https://cdn.shopify.com/s/files/1/0736/3588/9464/files/2_Device_Select.jpg?v=1702907003" alt="Install Step 2" width ="400"/>
<br/>
<img src="https://cdn.shopify.com/s/files/1/0736/3588/9464/files/3_Device_Pin_Select.jpg?v=1702907004" alt="Install Step 1" width ="400"/>
