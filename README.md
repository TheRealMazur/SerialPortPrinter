# SerialPortPrinter
Small programm made for sending receipt printing commands to **emar printo 57t** receipt printer.

It also shows receipt data preview in a table.

Data must be provided in a **.rct** file containing a JSON text structure.

Example file content:

```javascript
{
  "data": [
    "2 ; 0",
    "1 ; $l Order1 CR 1 CR A / 13.00 / 13.00",
    "2 ; $l Order2 CR 2 CR A / 12.00 / 24.00",
    "1 ; 1 $b 37.00 / Credit Card",
    "1; $e Restaurant Name CR 0 / 37.00",
    "28; 1 $z"
  ],
  "preview": {
    "1": {
      "name": "Order1",
      "amount": "1",
      "price": "13.00",
      "total": "13.00"
    },
    "2": {
      "name": "Order2",
      "amount": "2",
      "price": "12.00",
      "total": "24.00"
    }
  }
}
```

The programm automatically converts **"CR"** to **"\r"** inside the commads.

It also calculates the chechsum for every command and wraps the commands into a structure defined in the printers protocol:

**ESC P \<data goes here\> \<checksum> ESC \\**

Where **ESC** is ASCII code **0x1b**
