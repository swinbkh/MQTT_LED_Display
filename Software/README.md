# Software

# Things to note
* Any element can be updated independantly!
* Page 1 Subtext can only be a number, as this is always represented as a progress bar
* Newline characters are supported in the page title, but not subtext, as this automatically wraps
* Change the first few lines to your WiFi SSID, Password and MQTT server. 

# Interacting with the display

Any element can be updated independantly! This means if you only wish to change for example, Page 3 Icon, pubish to topic `cmnd/LED_Display/Display`:
```
{
  "PAGE3": {
    "ICON":"AC_UNIT"
  }
}
```
Newlines are not required, so long as it is valid JSON, it should be parsed correctly. 

Similarly for LEDs, they can be independantly controlled. The topic for LED updates is `cmnd/LED_Display/Hue`

```
{"LED1":{
    "H":120,
    "FLASH":1
},
"LED2":{
    "H":120,
    "FLASH":1
},
"LED3":{
    "H":120,
    "FLASH":1
},
"LED4":{
    "H":120,
    "FLASH":1
},
"LED5":{
    "H":120,
    "FLASH":1
}
}
```
Here, the Saturation values and Brightness values have not been passed through, so they will remain at their previous setting

# Supported data entry types
* OLED Display
  * Must include a page (String)
  * Title (String)
  * Subtext (String)
  * Icon (String, must be uppercase, with the characters identical to those found on [Material.io](https://material.io/resources/icons)
* LED Strip
  * Must include an LED number (String)
  * Hue (Float) 0-360
  * Saturation (Float) 0-100
  * Brightness (Float) 0-100
  * Flash (Bool/Int) 0/false, 1/true
  
  # Complete example
  
  This is the most that is possible. With this example, only the page texts/led values need changing
  
  ## MQTT OLED Topic: cmnd/LED_Display/Display
```
  {
  "PAGE1": {
    "TITLE": "PAGE 1 Title",
    "SUBTEXT": "50",
    "ICON":"AC_UNIT"
  },
  "PAGE2": {
    "TITLE": "Energy Usage",
    "SUBTEXT": "10KWH",
    "ICON":"TRENDING_UP"
  },
  "PAGE3": {
    "TITLE": "WiFi",
    "SUBTEXT": "WIFI SSID HERE",
    "ICON":"WIFI"
  },
  "PAGE4": {
    "TITLE": "Voltage",
    "SUBTEXT": "240V",
    "ICON":"REPORT"
  },
  "PAGE5": {
    "TITLE": "Unit Cost",
    "SUBTEXT": "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore.",
    "ICON":"ZOOM_OUT_MAP"
  }
  }

```


## MQTT RGB LED Topic: cmnd/LED_Display/Hue
```
{"LED1":{
    "H":0,
    "S":100,
    "V":100,
    "FLASH":true
},
"LED2":{
    "H":60,
    "S":100,
    "V":100,
    "FLASH":true
},
"LED3":{
    "H":120,
    "S":100,
    "V":100,
    "FLASH":true
},
"LED4":{
    "H":180,
    "S":100,
    "V":100,
    "FLASH":true
},
"LED5":{
    "H":240,
    "S":100,
    "V":100,
    "FLASH":true
}
}

```
